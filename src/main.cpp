#include <iostream>
#include "Math/ComplexArray/ComplexArray.h"
#include "PulseShapeFunctions/hamming.h"
#include "Math/Convolve/BasicConvolve/BasicConvolve.h"
#include "RadioInterfaces/TestRadioFile/TestRadioFile.h"
#include "Utilities/libnpy.hpp"
#include "CarrierRecovery/FFTFreqCoarse/FFTFreqRecovery.h"
#include "Math/FFTW/FFTW.h"
#include "CarrierRecovery/PLLPhaseFreq/DualPll/DuallPll.h"
#include "Modulator/BasicModulator/BasicModulator.h"
#include "Samplers/SamplerBrute/SamplerBrute.h"
extern "C" {
#include "FilterDesign/remez/remez.h"
}
#include "CarrierRecovery/Preprocess/SquarePreprocess.h"
#include "Receivers/BPSKReceiver/BPSKReceiver.h"
#include "Transmitters/BPSKTransmitter/BPSKTransmitter.h"
#include "RadioInterfaces/TestRadioUDP/TestRadioUDP.h"

#ifdef PLUTO
#include "RadioInterfaces/Pluto/Pluto.h"
#endif

using namespace std;

//TODO:optimize ComplexArray assignment operations
#define USE_HARDWARE


int main(int, char**){
    setbuf(stdout, 0);
    IRadio *pRadio;


    // ComplexArray receiveBuffer(8000);

    // pluto->Receive(receiveBuffer);

    // // receiveBuffer.SetAllImaginaryTo(0);
    // receiveBuffer.PrintContentsForPython();

    // return 0;

#ifdef BUILDX86
    TestRadioUDP tudp_rx("127.0.0.2", 1235, "127.0.0.1", 1234);
    pRadio = &pRadio;
#else
    #ifdef USE_HARDWARE
        Pluto *pluto = Pluto::GetInstance();
        pluto->SetRxParameters(433.7e6, 1e6, 1e6, 13);
        pluto->SetTxParameters(433.7e6, 1e6, 1e6, 13);
        pluto->InitializeDevice();
        pRadio = pluto;
    #else
        TestRadioUDP tudp_rx("10.0.1.10", 1235, "10.0.1.1", 1234);
        pRadio = &tudp_rx;
    #endif
#endif
    BPSKReceiver bpsk_rx(pRadio);

    // TestRadioFile tr("/home/cem//received_data0.npy");
    // BPSKReceiver bpsk_rx(&tr);
    
    // BPSKTransmitter btx(&tudp_tx);
    // uint8_t buffer[] = {0x55, 0x55, 0xe1, 0xff, 0xa5, 0x00, 0xaa, 0x12};
    // btx.Transmit(buffer, sizeof(buffer));
    // btx.mainThread();

    bpsk_rx.mainThread();

    return 0;
}
