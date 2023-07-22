#include "Math/ComplexArray/ComplexArray.h"
#include <mutex>
#include "Math/Convolve/BasicConvolve/BasicConvolve.h"
#include "RadioInterfaces/IRadio.h"

class BPSKTransmitter {
public:
    BPSKTransmitter(IRadio *pRadio);
    void mainThread();
    void Transmit(uint8_t *buffer, int size);
    void ConfigureTransmitter();
private:
    static const int tx_buffer_size = 1024*4;
    static const int maxDataByteSizePerTransmit = 60;
    static const int maxDataBitSizePerTransmit = maxDataByteSizePerTransmit * 8;

    uint8_t tx_buffer[tx_buffer_size];
    std::mutex tx_buffer_mutex;
    int tx_buffer_overrun_cnt;
    int tx_buffer_write_index, tx_buffer_read_index;
    ComplexArray temp, pulseShapedSignal;

    IRadio *pRadio;
    IConvolve *pConv;
    int oversamplingFactor;
    float toffset, P, beta, fs;

    int PulseShape(ComplexArray &pulseShaper, uint8_t *pData, int size);
    void UpConvertPulseShaped(ComplexArray &signalToUpConvert, float freq);
};