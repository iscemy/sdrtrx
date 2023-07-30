#include "Transmitters/BPSKTransmitter/BPSKTransmitter.h"
#include "PulseShapeFunctions/srrc.h"
#include "cstring"
#include "unistd.h"
#include "cmath"
#define debug
//todo implement queue on tx_buffer
void BPSKTransmitter::mainThread() {
#ifdef debug
    pConv = new BasicConvolve();
    beta = 1;
    oversamplingFactor = 30;
    P = 30;
    toffset = 0;
    fs = 1e6;
#endif
    temp.LateInit(oversamplingFactor * maxDataBitSizePerTransmit);
    pulseShapedSignal.LateInit(oversamplingFactor * maxDataBitSizePerTransmit);
    RealArray pulseShaper;
    PulseShaping::s_rrc(oversamplingFactor, beta, P, toffset, pulseShaper);

    int read_size, signal_size;
    // while(1) {
        usleep(100); // TODO implement signaling
        read_size = tx_buffer_write_index - tx_buffer_read_index;
        temp.SetSize(read_size * oversamplingFactor * 8);
        signal_size = PulseShape(pulseShaper, tx_buffer + tx_buffer_read_index, read_size);
        // pulseShapedSignal.PrintContentsForPython();
        pulseShapedSignal.SetSize(signal_size);
        UpConvertPulseShaped(pulseShapedSignal, 70e3);
        // pulseShapedSignal.PrintContentsForPython();
        pRadio->Transmit(pulseShapedSignal);
    // }
}

void BPSKTransmitter::Transmit(uint8_t *buffer, int size) {
    tx_buffer_mutex.lock();
    if(tx_buffer_write_index + size > tx_buffer_size) {
        tx_buffer_write_index = 0;
        tx_buffer_read_index = 0;
        tx_buffer_overrun_cnt++;
    }
    memcpy(tx_buffer+tx_buffer_write_index, buffer, size);
    tx_buffer_write_index += size;
    tx_buffer_mutex.unlock();
}

BPSKTransmitter::BPSKTransmitter(IRadio *pRadio) {
    tx_buffer_overrun_cnt = 0;
    tx_buffer_write_index = 0;
    this->pRadio = pRadio;
}

int BPSKTransmitter::PulseShape(RealArray &pulseShaper, uint8_t *pData, int size) {
    //OverSampling
    for(int i = 0; i < size; i++) {
        for(int k = 0; k < 8; k++){
            if (pData[i] & (1<<(7-k)))
                temp[oversamplingFactor * ((8*i + k) + 1)] = 1.0f;
            else 
                temp[oversamplingFactor * ((8*i + k) + 1)] = -1.0f;
        }
    }
    int oversampledDataSize = oversamplingFactor * (8*(size) + 2);
    
    pConv->DoConvolve(temp, pulseShaper, pulseShapedSignal, oversampledDataSize);
    return pConv->GetResultingSize(temp.GetElementSize(), pulseShaper.GetElementSize());
}

void BPSKTransmitter::UpConvertPulseShaped(RealArray &signalToUpConvert, float freq) {
    #if 0
    int size = signalToUpConvert.GetElementSize();
    float imag, real;
    float tpfcfs = 2.0f * M_PI * freq / fs;
    for(int i = 0; i < size; i++) {
        imag = signalToUpConvert[i] * sinf32(tpfcfs * ((float) i));
        real = signalToUpConvert[i] * cosf32(tpfcfs * ((float) i));
        signalToUpConvert[i] = std::complex<float>(real, imag);
    }
    #else

    signalToUpConvert = signalToUpConvert;
    freq = freq;

    #endif

}