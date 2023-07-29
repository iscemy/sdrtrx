#include "Receivers/BPSKReceiver/BPSKReceiver.h"
#include "CarrierRecovery/Preprocess/SquarePreprocess.h"
#include "Math/Convolve/BasicConvolve/BasicConvolve.h"
#include "Math/FFTW/FFTW.h"
#include "CarrierRecovery/FFTFreqCoarse/FFTFreqRecovery.h"
#include "CarrierRecovery/PLLPhaseFreq/DualPll/DuallPll.h"
#include "Modulator/BasicModulator/BasicModulator.h"
#include "Samplers/SamplerBrute/SamplerBrute.h"
#include "Filter/BasicFilter/BasicFilter.h"
#include "Utilities/log.h"
#include "vector"
// #include <chrono>

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
    float maxReceivedAbsVal, minReceivedAbsVal, recoveredFreqCoarse;
    int resultingSize;
    
    SquarePreprocess sqrpreprocess(fs, f0, 200);
    BasicConvolve conv;

    BasicFilter highPassFilter(&conv);
    ComplexArray filtertaps(lowPassFilterSize);
    filtertaps.SetBufferReal(lowPassFilterTaps, lowPassFilterSize);
    highPassFilter.SetFilterTaps(filtertaps);

    resultingSize = sqrpreprocess.GetOutputSize(intermediateFreqSignalSize);
    FFTW fft(resultingSize * 2);
    FFTFreqRecovery fftrec(&fft);
    ComplexArray rp(resultingSize), carrierRecovered(resultingSize);

    DualPll pllRecoverer(fs, f0);
    BasicModulator downconvert(fs, 0, 4e4);
    ComplexArray baseband(downconvert.getOuputSize(resultingSize));

    std::vector<int> pattern = {-1,1,-1,1,-1,1,-1,1};
    TimeRecBrute sampler(oversampleFactor, pattern);
    int samplesLeftFromPreviousProcessing = 0;

    receiveBuffer.LateInit(intermediateFreqSignalSize);
    processBuffer.LateInit(intermediateFreqSignalSize);
    highPassFiltered.LateInit(intermediateFreqSignalSize);
    bool firstRun = true;
    std::vector<std::pair<int,int>> indices, indices_baseband;
    std::vector<int> output;

    ComplexArray partialProcessBuffer(intermediateFreqSignalSize);

    while(1) {

        pRadio->Receive(receiveBuffer);
        t0 = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
        receiveBuffer.SetAllImaginaryTo(0);

        receiveBuffer.Normalize(minReceivedAbsVal, maxReceivedAbsVal);
        // float minMaxRatio = ((float)maxReceivedAbsVal) /((float)minReceivedAbsVal);

        // printf("tdiff %lld \n", t1.count() - t0.count());
        if((maxReceivedAbsVal < receiverNoiseFloor)) {
            PrintCycleTime();
            continue;
        }
        // printf("samplesLeftFromPreviousProcessing %d \n",samplesLeftFromPreviousProcessing);

        if(!firstRun) {
            int offset = receiveBufferPrev.GetElementSize() - intermediateFreqSignalSize * 0.05 - samplesLeftFromPreviousProcessing;
            if(offset < 0)
                offset = 0;
            processBuffer.PartialCopyTwoArrays(receiveBufferPrev, offset,
                                        intermediateFreqSignalSize * 0.05 + samplesLeftFromPreviousProcessing,
                                        receiveBuffer, 0, receiveBuffer.GetElementSize());
        } else {
            processBuffer = receiveBuffer;
            firstRun = false;
        }

        highPassFilter.RunFilter(processBuffer, highPassFiltered);
        
        volatile float receivedMaxVal = receiveBuffer.GetMaxValAbs();
        volatile float filteredMaxVal = highPassFiltered.GetMaxValAbs();

        if(filteredMaxVal / receivedMaxVal < 0.5) { //todo more solid value
            receiveBufferPrev = receiveBuffer;
            PrintCycleTime();
            continue;
        }
        
        highPassFiltered.GetNonZeroIndexes(indices, 200);

        checkIndicesForInvalidOnes(indices);

        int numberOfIndices = indices.size();

        if(numberOfIndices > 0) {
            if(indices.back().second > (highPassFiltered.GetElementSize()*0.95f)) {
                numberOfIndices--;
                samplesLeftFromPreviousProcessing = highPassFiltered.GetElementSize() - indices.back().first + 50 * 20;
                indices.erase(indices.end());
            } else {
                samplesLeftFromPreviousProcessing = 0;
            }
        } else {
            receiveBufferPrev = receiveBuffer;
            PrintCycleTime();
            continue;
        }

        for(std::pair<int, int> index : indices) {
            partialProcessBuffer.CopyFromAnotherArray(highPassFiltered, index.first, index.second - index.first);
            sqrpreprocess.DoPreprocess(&conv, partialProcessBuffer, rp);
            rp.ScaleWith(600.0);

            // fftrec.processBuffer(rp, f0, 10e3, recoveredFreqCoarse, fs); //add windowing function
            recoveredFreqCoarse = 70e3; 
            pllRecoverer.RunAlgorithm(rp, carrierRecovered, recoveredFreqCoarse);

            downconvert.RunAlgorithm(partialProcessBuffer, carrierRecovered, baseband);
            baseband.Normalize();
            
            baseband.GetNonZeroIndexes(indices_baseband, oversampleFactor);
            

            for(size_t i = 0; i < indices_baseband.size(); i++){
                    int bytes = sampler.RunAlgorithm(baseband, output, indices_baseband[i].first, indices_baseband[i].second - indices_baseband[i].first);
                    if(bytes > 0) {
                        // printf("birkac byte aldik hatasiz \n");
                        for(auto &received_byte : output) {
                            printf("%x ", received_byte);
                        }
                        printf("\n"); 
                    } else {
                        printf("ins paket kesildigindendir %f %d\n", recoveredFreqCoarse, indices_baseband[i].first);

                        
                        // baseband.PrintContentsForPython();
                        // rp.PrintContentsForPython();
                        // partialProcessBuffer.PrintContentsForPython();
                        // carrierRecovered.PrintContentsForPython();
                        
                        // baseband.PrintContentsForPython();
                        // pReceiveBuffer->PrintContentsForPython();
                    }

            }

            indices_baseband.clear();

        }   



        receiveBufferPrev = receiveBuffer;
        indices.clear();
        PrintCycleTime();
    }
}

void BPSKReceiver::checkIndicesForInvalidOnes(std::vector<std::pair<int,int>> indices) {
    for(auto &index : indices) {
        if(index.first < 10) {
            LOG2CONSOLE("indices not looks right");
        }
    }

}

void BPSKReceiver::PrintCycleTime() {
    t1 = duration_cast< milliseconds >(system_clock::now().time_since_epoch());
    long long int tdiffms = (t1 - t0).count();
    if(tdiffms > 8) {

    }
    
}