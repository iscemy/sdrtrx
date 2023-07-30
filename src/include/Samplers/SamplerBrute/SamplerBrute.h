#include "vector"
#include "Math/RealArray/RealArray.h"

class TimeRecBrute {
public:
    TimeRecBrute(int oversampleFactor, std::vector<int> patternExpected);
    int RunAlgorithm(RealArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize);

private:
    void changeParameters(int oversampleFactor, std::vector<int> patternExpected);
    int oversampleFactor;
    int phyPreambleSizeInBits, phyHeaderSizeInBits, phyTrailerSizeInBits;
    uint16_t preambleExpected = 0x55AA, trailerExpected = 0xAA55;

    int Sampler(RealArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize, bool &samplerError);
    uint32_t CombineBits(std::vector<int> bits, int numOfBits, int offset);
    bool ValidateBuffer(RealArray &input, int inputOffset, int inputProcessSize, int &sampleLatencyIndex, int &numOfBits, bool isPiErrorExists);

};