#include "RadioInterfaces/IRadio.h"

#include <chrono>

class BPSKReceiver {


public:
    BPSKReceiver(IRadio *pRadio);
    int Receive(uint8_t *pBuffer, int bufferSize);
    void mainThread();
private:
    IRadio *pRadio;
    std::vector<uint8_t> decodedBits;
    RealArray receiveBuffer, processBuffer, receiveBufferPrev, highPassFiltered;
    int receiverState;
    static const int intermediateFreqSignalSize = 8000, zeroBufferSize = 1000;

    RealArray *CurrentProcessBuffer, *ReceiveBuffer, *PreviousProcessBuffer;

    static const int receiveBufferSize = 1024 * 1024;
    
    int oversampleFactor = 30;
    float receiverNoiseFloor = 10;

    static const int lowPassFilterSize = 19;
    float lowPassFilterTaps[lowPassFilterSize] = {
        0.023498584007711640,
        0.013349966070291007,
        -0.002602384124422894,
        -0.023294272392655184,
        -0.046942153013936172,
        -0.071244916281039058,
        -0.093667951914802122,
        -0.111770057962719646,
        -0.123526432657048726,
        0.872399236537242273,
        -0.123526432657048726,
        -0.111770057962719646,
        -0.093667951914802122,
        -0.071244916281039058,
        -0.046942153013936172,
        -0.023294272392655184,
        -0.002602384124422894,
        0.013349966070291007,
        0.023498584007711640,
    };

    void checkIndicesForInvalidOnes(std::vector<std::pair<int,int>> indices);

    void PrintCycleTime();

    std::chrono::milliseconds t0, t1;
};