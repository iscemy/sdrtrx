#include "Receivers/BPSKReceiver/BPSKReceiver.h"
#include "CarrierRecovery/Preprocess/SquarePreprocess.h"
#include "Math/Convolve/BasicConvolve/BasicConvolve.h"
#include "Math/FFTW/FFTW.h"
#include "CarrierRecovery/FFTFreqCoarse/FFTFreqRecovery.h"
#include "CarrierRecovery/PLLPhaseFreq/DualPll/DuallPll.h"
#include "Modulator/BasicModulator/BasicModulator.h"
#include "Samplers/SamplerBrute/SamplerBrute.h"

#include "vector"
#include <chrono>

using namespace std::chrono;

int BPSKReceiver::Receive(uint8_t *pBuffer, int bufferSize) {
    bufferSize = bufferSize;
    pBuffer = pBuffer;

    return 0;
}

BPSKReceiver::BPSKReceiver(IRadio *pRadio) {
    this->pRadio = pRadio;
    
}

void BPSKReceiver::mainThread() {
    float fs = 1e6, f0 = 70e3;
    int resultingSize;
    
    SquarePreprocess sqrpreprocess(fs, f0, 200);
    BasicConvolve conv;
    
    intermediateFreqSignal0.LateInit(intermediateFreqSignalSize);
    intermediateFreqSignal1.LateInit(intermediateFreqSignalSize);
    moduleReceiveBuffer.LateInit(intermediateFreqSignalSize);
    
    zeroBuffer.LateInit(zeroBufferSize);

    for(int i = 0; i < zeroBufferSize; i++) {
        zeroBuffer[i] = 0;
    }

    resultingSize = sqrpreprocess.GetOutputSize(intermediateFreqSignalSize);
    FFTW fft(resultingSize * 2);
    FFTFreqRecovery fftrec(&fft);
    RealArray rp(resultingSize), carrierRecovered(resultingSize);

    float recoveredFreqCoarse;
    DualPll pllRecoverer(fs, f0);
    BasicModulator downconvert(fs, 0, 4e4);
    RealArray baseband(downconvert.getOuputSize(resultingSize));

    std::vector<int> pattern = {-1,1,-1,1,-1,1,-1,1};
    TimeRecBrute sampler(30, pattern);
    std::vector<int> output;
    std::vector<std::pair<int,int>> indices;
    std::vector<std::pair<int,int>> indices_rp;

    SetCurrentProcessingBuffer(&intermediateFreqSignal0);
    SetCurrentReceiveBuffer(&moduleReceiveBuffer);
    SetPreviousProcessingBuffer(&intermediateFreqSignal1);

    int samplesLeftFromPreviousProcessing = 0;
    milliseconds t0, t1;

    while(1) {
        RealArray *pProcessCurrent = GetCurrentProcessingBuffer();
        RealArray *pProcessPrev = GetPreviousProcessingBuffer();
        RealArray *pReceiveBuffer = GetCurrentReceiveBuffer();
        pReceiveBuffer->SetSize(intermediateFreqSignalSize);

        pRadio->Receive(*pReceiveBuffer);

        t0 = duration_cast< milliseconds >(system_clock::now().time_since_epoch());

        pReceiveBuffer->SetAllImaginaryTo(0);   
        pReceiveBuffer->Normalize();

        if(samplesLeftFromPreviousProcessing > 0) {
            //copying left over and newly received buffer to current process buffer 
            pProcessCurrent->PartialCopyTwoArrays(*pProcessPrev, pProcessPrev->GetElementSize() - samplesLeftFromPreviousProcessing, samplesLeftFromPreviousProcessing,
                                                    *pReceiveBuffer, 0, pReceiveBuffer->GetElementSize());
            // pProcessCurrent->PrintContentsForPython();
        } else {
            //swapping receive and process buffer
            SetCurrentProcessingBuffer(pReceiveBuffer);
            SetCurrentReceiveBuffer(pProcessCurrent);
            pProcessCurrent->PartialCopyTwoArrays(zeroBuffer, 0, zeroBuffer.GetElementSize(),
                                                                *pReceiveBuffer, 0, pReceiveBuffer->GetElementSize());
        }

        rp.GetNonZeroIndexes(indices);
        
        sqrpreprocess.DoPreprocess(&conv, *GetCurrentProcessingBuffer(), rp);
        rp.ScaleWith(400.0);
        


        // rp.PrintContentsForPython();
        fftrec.processBuffer(rp, f0, 10e3, recoveredFreqCoarse, fs);
        pllRecoverer.RunAlgorithm(rp, carrierRecovered, recoveredFreqCoarse);
        // carrierRecovered.PrintContentsForPython();
        RealArray carrierRecovered_offseted(carrierRecovered, 100);
        downconvert.RunAlgorithm(*GetCurrentProcessingBuffer(), carrierRecovered_offseted, baseband);
        baseband.Normalize();
        // baseband.GetNonZeroIndexes(indices);

        // baseband.PrintContentsForPython();

        int numberOfIndices = indices.size();

        if(indices.back().second > (baseband.GetElementSize()*0.99f)) {
            numberOfIndices--;
            samplesLeftFromPreviousProcessing = baseband.GetElementSize() - indices.back().first + 50 * 20;
            indices.erase(indices.end());
        } else {
            samplesLeftFromPreviousProcessing = 0;
        }
        for(size_t i = 0; i < indices.size(); i++){
            int bytes = sampler.RunAlgorithm(baseband, output, indices[i].first, indices[i].second - indices[i].first);
            if(bytes > 0) {
                // printf("birkac byte aldik hatasiz \n");
                for(auto &received_byte : output) {
                    printf("%x ", received_byte);
                }
                printf("\n");
            } else {
                printf("ins paket kesildigindendir\n");
                // baseband.PrintContentsForPython();
                // pReceiveBuffer->PrintContentsForPython();
            }

        }
        SwapBuffers();
        indices.clear();
        indices_rp.clear();
        t1 = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        printf("t1 - t0 %ldms\n", (t1.count()-t0.count()));
    }
}

// void BPSKReceiver::ProcessFrame(RealArray &input, std::vector<uint8_t> &output, int inputOffest, int inputSize) {

// }

// void BPSKReceiver::ProcessFrame(RealArray &input, RealArray &preprocessed, std::vector<uint8_t> &output, int inputOffest, int inputSize) {

// }

RealArray * BPSKReceiver::GetCurrentProcessingBuffer() {
    return CurrentProcessBuffer;
}

RealArray * BPSKReceiver::GetPreviousProcessingBuffer() {
    return PreviousProcessBuffer;
}

RealArray *BPSKReceiver::GetCurrentReceiveBuffer() {
    return ReceiveBuffer;
}

void BPSKReceiver::SetCurrentProcessingBuffer(RealArray *buffer) {
    CurrentProcessBuffer = buffer;
}

void BPSKReceiver::SetPreviousProcessingBuffer(RealArray *buffer) {
    PreviousProcessBuffer = buffer;
}

void BPSKReceiver::SetCurrentReceiveBuffer(RealArray *buffer) {
    ReceiveBuffer = buffer;
}

void BPSKReceiver::SwapBuffers() {
    RealArray *tmp;
    tmp = CurrentProcessBuffer;
    CurrentProcessBuffer = PreviousProcessBuffer;
    PreviousProcessBuffer = tmp;

}