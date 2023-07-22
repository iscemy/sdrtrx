#include "RadioInterfaces/Pluto/Pluto.h"
#include "Utilities/log.h"

struct iio_context *Pluto::ctx = nullptr;
struct iio_channel *Pluto::rx0_i = nullptr;
struct iio_channel *Pluto::rx0_q = nullptr;
struct iio_channel *Pluto::tx0_i = nullptr;
struct iio_channel *Pluto::tx0_q = nullptr;
struct iio_buffer  *Pluto::rxbuf = nullptr;
struct iio_buffer  *Pluto::txbuf = nullptr;
Pluto *Pluto::pInstance = nullptr;

int Pluto::Receive(ComplexArray &data) {
    data = data;
    ptrdiff_t p_inc;
    int16_t *receivedDataMapped;
    int nbytes_rx =  iio_buffer_refill(rxbuf);

    
    if (nbytes_rx < 0) { 
        LOG2CONSOLE("Error refilling buf");
        return -1;
    }
	
    p_inc = iio_buffer_step(rxbuf);
    

    if(p_inc != 4) {
        LOG2CONSOLE("increment size is not right");
        return -2;
    }

    receivedDataMapped = (int16_t *)iio_buffer_first(rxbuf, rx0_i);
    data.SetBuffer(receivedDataMapped, nbytes_rx / (sizeof(int16_t) * 2) , p_inc / 2);

    return nbytes_rx;
}

int Pluto::Transmit(ComplexArray &data) {
    data = data;
    return 0;
}

Pluto::Pluto() {
    bool result = true;
    ctx = iio_create_default_context();
    if(ctx == nullptr)
        return;
    result &= get_ad9361_stream_dev(TX, &tx);
    result &= get_ad9361_stream_dev(RX, &rx);
    if(result)
        isContextReceived = true;
    else 
        isContextReceived = false;
}

Pluto::~Pluto() {
    
	if (rxbuf) { iio_buffer_destroy(rxbuf); }
	if (txbuf) { iio_buffer_destroy(txbuf); }

	
	if (rx0_i) { iio_channel_disable(rx0_i); }
	if (rx0_q) { iio_channel_disable(rx0_q); }
	if (tx0_i) { iio_channel_disable(tx0_i); }
	if (tx0_q) { iio_channel_disable(tx0_q); }

	
	if (ctx) { iio_context_destroy(ctx); }
	
}

Pluto * Pluto::GetInstance() {
    if(pInstance == nullptr) {
        pInstance = new Pluto;
    }

    return pInstance;

}

int Pluto::wr_ch_lli(struct iio_channel *chn, const char* what, long long val) {
	return iio_channel_attr_write_longlong(chn, what, val);
}

int Pluto::wr_ch_float(struct iio_channel *chn, const char* what, double val) {
	return iio_channel_attr_write_double(chn, what, val);
}

int Pluto::wr_ch_str(struct iio_channel *chn, const char* what, const char* str) {
	return iio_channel_attr_write(chn, what, str);
}

char* Pluto::get_ch_name(const char* type, int id) {
	snprintf(tmpstr, sizeof(tmpstr), "%s%d", type, id);
	return tmpstr;
}

/* returns ad9361 phy device */
struct iio_device* Pluto::get_ad9361_phy(void) {
	struct iio_device *dev =  iio_context_find_device(ctx, "ad9361-phy");
	return dev;
}

/* finds AD9361 streaming IIO devices */
bool Pluto::get_ad9361_stream_dev(enum iodev d, struct iio_device **dev) {
	switch (d) {
	case TX: *dev = iio_context_find_device(ctx, "cf-ad9361-dds-core-lpc"); return *dev != NULL;
	case RX: *dev = iio_context_find_device(ctx, "cf-ad9361-lpc");  return *dev != NULL;
	default: return false;
	}
}

/* finds AD9361 streaming IIO channels */
bool Pluto::get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn) {
	*chn = iio_device_find_channel(dev, get_ch_name("voltage", chid), d == TX);
	if (!*chn)
		*chn = iio_device_find_channel(dev, get_ch_name("altvoltage", chid), d == TX);
	return *chn != NULL;
}

/* finds AD9361 phy IIO configuration channel with id chid */
bool Pluto::get_phy_chan(enum iodev d, int chid, struct iio_channel **chn) {
	switch (d) {
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("voltage", chid), false); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("voltage", chid), true);  return *chn != NULL;
	default: return false;
	}
}

/* finds AD9361 local oscillator IIO configuration channels */
bool Pluto::get_lo_chan(enum iodev d, struct iio_channel **chn) {
	switch (d) {
	 // LO chan is always output, i.e. true
	case RX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("altvoltage", 0), true); return *chn != NULL;
	case TX: *chn = iio_device_find_channel(get_ad9361_phy(), get_ch_name("altvoltage", 1), true); return *chn != NULL;
	default: return false;
	}
}

bool Pluto::cfg_ad9361_streaming_ch(struct stream_cfg *cfg, enum iodev type, int chid) {
	struct iio_channel *chn = NULL;

	// Configure phy and lo channels
	printf("* Acquiring AD9361 phy channel %d\n", chid);
	if (!get_phy_chan(type, chid, &chn)) {	return false; }
	wr_ch_str(chn, "rf_port_select",     cfg->rfport);
	wr_ch_lli(chn, "rf_bandwidth",       cfg->bw_hz);
	wr_ch_lli(chn, "sampling_frequency", cfg->fs_hz);
	wr_ch_float(chn, "hardwaregain", cfg->hwgain);

	// Configure LO channel
	printf("* Acquiring AD9361 %s lo channel\n", type == TX ? "TX" : "RX");
	if (!get_lo_chan(type, &chn)) { return false; }
	wr_ch_lli(chn, "frequency", cfg->lo_hz);
	return true;
}

int Pluto::SetRxParameters(float lo_hz, float sample_rate_sps, float bandwidth_hz, float hwgain_hz) {
    if(!isContextReceived) return -2;
    rxcfg.bw_hz = bandwidth_hz;
    rxcfg.fs_hz = sample_rate_sps;
    rxcfg.hwgain = hwgain_hz;
    rxcfg.lo_hz = lo_hz;
    rxcfg.rfport = "A_BALANCED";
    if(cfg_ad9361_streaming_ch(&rxcfg, RX, 0)) {
        isRxParametersSet = true;
        return 0;
    } else return -1;
}

int Pluto::SetTxParameters(float lo_hz, float sample_rate_sps, float bandwidth_hz, float hwgain_hz) {
    if(!isContextReceived) return -2;
    txcfg.bw_hz = bandwidth_hz;
    txcfg.fs_hz = sample_rate_sps;
    txcfg.hwgain = hwgain_hz;
    txcfg.lo_hz = lo_hz;
    txcfg.rfport = "A";
    if(cfg_ad9361_streaming_ch(&txcfg, TX, 0)) {
        isTxParametersSet = true;
        return 0;
    }
    else return -1;
}

int Pluto::InitStreamingChannels() {
    bool ret = true;
    ret &= get_ad9361_stream_ch(RX, rx, 0, &rx0_i);
	ret &= get_ad9361_stream_ch(RX, rx, 1, &rx0_q);
	ret &= get_ad9361_stream_ch(TX, tx, 0, &tx0_i);
	ret &= get_ad9361_stream_ch(TX, tx, 1, &tx0_q);

    if(ret) return 0;
    else return -1;
}

int Pluto::InitializeDevice() {
    if(!isContextReceived) return -3;
    if((!isRxParametersSet) || (!isTxParametersSet)) {
        LOG2CONSOLE("pluto parameters not set");
        return -1;
    }

    if(InitStreamingChannels() != 0) {
        LOG2CONSOLE("pluto cannot get channels");
        return -2;
    }

    iio_channel_enable(rx0_i);
	iio_channel_enable(rx0_q);
	iio_channel_enable(tx0_i);
	iio_channel_enable(tx0_q);

	rxbuf = iio_device_create_buffer(rx, TxRxBufferSize, false);
	if (!rxbuf) {
		LOG2CONSOLE("Could not create RX buffer");
	}
	txbuf = iio_device_create_buffer(tx, TxRxBufferSize, false);
	if (!txbuf) {
		LOG2CONSOLE("Could not create TX buffer");
	}

    isDeviceReady = true;

    return 0;
}

