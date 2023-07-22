#include "vector"
#include "Math/ComplexArray/ComplexArray.h"

class TimeRecBrute {
public:
    TimeRecBrute(int oversampleFactor, std::vector<int> patternExpected);
    int RunAlgorithm(ComplexArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize);

private:
    void changeParameters(int oversampleFactor, std::vector<int> patternExpected);
    int oversampleFactor;
    int phyPreambleSizeInBits, phyHeaderSizeInBits, phyTrailerSizeInBits;
    uint16_t preambleExpected = 0x55AA, trailerExpected = 0xAA55;

    int Sampler(ComplexArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize, bool &samplerError);
    uint32_t CombineBits(std::vector<int> bits, int numOfBits, int offset);
    bool ValidateBuffer(ComplexArray &input, int inputOffset, int inputProcessSize, int &sampleLatencyIndex, int &numOfBits, bool isPiErrorExists);

};