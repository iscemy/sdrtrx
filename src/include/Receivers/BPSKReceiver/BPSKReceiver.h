#include "RadioInterfaces/IRadio.h"

class BPSKReceiver {
public:
    BPSKReceiver(IRadio *pRadio);
    int Receive(uint8_t *pBuffer, int bufferSize);
    void mainThread();
private:
    IRadio *pRadio;
    std::vector<uint8_t> decodedBits;
    ComplexArray intermediateFreqSignal0, intermediateFreqSignal1, moduleReceiveBuffer, zeroBuffer, partialProcessBuffer;
    int receiverState;
    static const int intermediateFreqSignalSize = 8000, zeroBufferSize = 1000;

    void MergeBuffers(ComplexArray &buffer1, int index1, int size1, ComplexArray &buffer2, int index2, int size2, ComplexArray &bufferOut, int size);
    void ProcessFrame(ComplexArray &input, std::vector<uint8_t> &output, int inputOffest, int inputSize);
    void ProcessFrame(ComplexArray &input, ComplexArray &preprocessed, std::vector<uint8_t> &output, int inputOffest, int inputSize);

    ComplexArray *GetCurrentProcessingBuffer();
    ComplexArray *GetPreviousProcessingBuffer();
    ComplexArray *GetCurrentReceiveBuffer();

    void SetCurrentProcessingBuffer(ComplexArray *buffer);
    void SetPreviousProcessingBuffer(ComplexArray *buffer);
    void SetCurrentReceiveBuffer(ComplexArray *buffer);

    void SwapBuffers();

    ComplexArray *CurrentProcessBuffer, *ReceiveBuffer, *PreviousProcessBuffer;

    static const int receiveBufferSize = 1024 * 1024;
    
    float receiveBuffer[receiveBufferSize];
};