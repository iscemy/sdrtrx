#include "RadioInterfaces/IRadio.h"
#include <iio.h>

class Pluto : public IRadio{
public:
    int Receive(ComplexArray &data);
    int Transmit(ComplexArray &data);
    static Pluto *GetInstance();
    int SetRxParameters(float lo_hz, float sample_rate_sps, float bandwidth_hz, float hwgain_hz);
    int SetTxParameters(float lo_hz, float sample_rate_sps, float bandwidth_hz, float hwgain_hz);
    int InitializeDevice();
    ~Pluto();
private:
    enum iodev { RX, TX };
    struct stream_cfg {
        long long bw_hz; // Analog banwidth in Hz
        long long fs_hz; // Baseband sample rate in Hz
        long long lo_hz; // Local oscillator frequency in Hz
        long long hwgain;
        const char* rfport; // Port name
    };

    static Pluto *pInstance;

    bool isRxParametersSet = false, isTxParametersSet = false, isDeviceReady = false;
    bool isContextReceived = false;
    char tmpstr[300];
    static struct iio_context *ctx;
    static struct iio_channel *rx0_i;
    static struct iio_channel *rx0_q;
    static struct iio_channel *tx0_i;
    static struct iio_channel *tx0_q;
    static struct iio_buffer  *rxbuf;
    static struct iio_buffer  *txbuf;

    struct iio_device *tx;
	struct iio_device *rx;

	// RX and TX sample counters
	size_t nrx = 0;
	size_t ntx = 0;

	// Stream configurations
	struct stream_cfg rxcfg;
	struct stream_cfg txcfg;
    
    int InitStreamingChannels();

    int wr_ch_lli(struct iio_channel *chn, const char* what, long long val);
    int wr_ch_float(struct iio_channel *chn, const char* what, double val);
    int wr_ch_str(struct iio_channel *chn, const char* what, const char* str);
    char* get_ch_name(const char* type, int id);
    struct iio_device* get_ad9361_phy(void);
    bool get_ad9361_stream_dev(enum iodev d, struct iio_device **dev);
    bool get_ad9361_stream_ch(enum iodev d, struct iio_device *dev, int chid, struct iio_channel **chn);
    bool get_phy_chan(enum iodev d, int chid, struct iio_channel **chn);
    bool get_lo_chan(enum iodev d, struct iio_channel **chn);
    bool cfg_ad9361_streaming_ch(struct stream_cfg *cfg, enum iodev type, int chid);

    Pluto();

    Pluto(Pluto &other) = delete;
    void operator=(const Pluto &) = delete;
    const int TxRxBufferSize = 8000;
};