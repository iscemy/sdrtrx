#include "Samplers/SamplerBrute/SamplerBrute.h"
#include "algorithm"
#include "Utilities/log.h"
/*
    16bit preamble for sync     0 1 0 1 0 1 0 1 1 0 1 0 1 0 1 0 55AA
    16bit sizeBytes                  little endian
    ...
    ... data
    ... data
    ...
    16bit delimeter for sync    1 0 1 0 1 0 1 0 0 1 0 1 0 1 0 1 -- AA55
*/

TimeRecBrute::TimeRecBrute(int oversampleFactor, std::vector<int> patternExpected) {
    changeParameters(oversampleFactor, patternExpected);
    phyPreambleSizeInBits = 16;
    phyHeaderSizeInBits = 16;
    phyTrailerSizeInBits = 16;
}

//TODO: implement AGC, or moving normalizing

int TimeRecBrute::RunAlgorithm(ComplexArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize) {
    std::vector<int> sampledTemp;
    int sampleLatencyIndex, numOfBits;
    bool samplerError, isBufferValid = false, isPiErrorExists = false;
    output.clear();

    isBufferValid = ValidateBuffer(input, inputOffset, inputProcessSize, sampleLatencyIndex, numOfBits, false);
    if(!isBufferValid) {
        isBufferValid = ValidateBuffer(input, inputOffset, inputProcessSize, sampleLatencyIndex, numOfBits, true);
        isPiErrorExists = true;
        if(!isBufferValid)
            return 0;
    }

    Sampler(input, sampledTemp, sampleLatencyIndex + oversampleFactor * (phyHeaderSizeInBits + phyPreambleSizeInBits), numOfBits * oversampleFactor, samplerError);

    if(((int)sampledTemp.size()) != numOfBits) {
        LOG2CONSOLE("datasampling error");
        return 0;
    }

    if(!isPiErrorExists) {
        for(int i = 0; i < numOfBits;) {
            output.push_back(CombineBits(sampledTemp, 8, i));
            i += 8;
        }
    } else {
        for(int i = 0; i < numOfBits;) {
            output.push_back((uint8_t)~CombineBits(sampledTemp, 8, i));
            i += 8;
        }
    }
    return output.size();
}

bool TimeRecBrute::ValidateBuffer(ComplexArray &input, int inputOffset, int inputProcessSize, int &sampleLatencyIndex, int &numOfBits, bool isPiErrorExists) {
    std::vector<int> temp, patternExpected;
    uint16_t preamble, trailer, numOfBytes;
    int patternSize = phyPreambleSizeInBits;
    std::vector<float>correlationArray;
    float correlationval;
    int bit;

    
    uint16_t preambleExpected_local = preambleExpected, trailerExpected_local = trailerExpected;

    if(isPiErrorExists) {
        preambleExpected_local = ~preambleExpected_local;
        trailerExpected_local = ~trailerExpected_local;
    }

    for(int i = 0; i < phyPreambleSizeInBits; i++) {
        bit = ((preambleExpected_local & (1<<(phyHeaderSizeInBits-i-1))) == (1<<(phyHeaderSizeInBits-i-1))) * 2 - 1;
        patternExpected.push_back(bit);
    }

    for(int i = inputOffset; i < oversampleFactor * 3 + inputOffset; i++) {
        correlationval = 0;
        for(int j = 0; (j < patternSize) && ((i + j * oversampleFactor) < (inputOffset + inputProcessSize)); j++) {
            correlationval += input[i + j * oversampleFactor].real() * patternExpected[j];
        }
        correlationArray.push_back(correlationval);
    }

    sampleLatencyIndex = std::distance(correlationArray.begin(),std::max_element(correlationArray.begin(), correlationArray.end())) + inputOffset;

    std::vector<int> sampledTemp;
    bool samplerError = false;
    Sampler(input, sampledTemp, sampleLatencyIndex, oversampleFactor * (phyHeaderSizeInBits + phyPreambleSizeInBits), samplerError);


    if(((int)sampledTemp.size()) != (phyPreambleSizeInBits + phyHeaderSizeInBits))  {
        LOG2CONSOLE("header size mismatch");
        return false;
    }
     
    preamble = CombineBits(sampledTemp, 16, 0);
    numOfBytes = CombineBits(sampledTemp, 16, 16);

    if(isPiErrorExists) {
        numOfBytes = (uint16_t)~(uint16_t)numOfBytes;
    }


    if(preamble != preambleExpected_local) {
        LOG2CONSOLE("preamble error");
        return false;
    }
    numOfBits = numOfBytes * 8;
    if(numOfBytes > 70) // todo parametric
        return false;
    Sampler(input, sampledTemp, sampleLatencyIndex + oversampleFactor * (phyHeaderSizeInBits + phyPreambleSizeInBits + numOfBits),
      phyTrailerSizeInBits * oversampleFactor, samplerError);

    if(samplerError){
        LOG2CONSOLE("trailer smapler error");
        return false;
    }

    if(((int)sampledTemp.size()) != phyTrailerSizeInBits) {
        LOG2CONSOLE("trailer size mismatch");
        return false;
    }

    trailer = CombineBits(sampledTemp, 16, 0);

    if(trailer != trailerExpected_local) {
        LOG2CONSOLE("trailer mismatch");
        return false;
    }

    return true;
}

int TimeRecBrute::Sampler(ComplexArray &input, std::vector<int> &output, int inputOffset, int inputProcessSize, bool &samplerError) {
    output.clear();
    samplerError = false;
    for(int i = inputOffset; i < inputOffset + inputProcessSize;  i += oversampleFactor) {
        if(((input[i].real() > 0.1) && (input[i].real() > 0)) || ((input[i].real() < -0.1) && (input[i].real() < 0))) // ???
            output.push_back(input[i].real() > 0.1);
        else {
            //output.clear();
            samplerError = true;
            //frame contains errors 
            break;
        }
    }
    return output.size();
}

void TimeRecBrute::changeParameters(int oversampleFactor, std::vector<int> patternExpected) {
    this->oversampleFactor = oversampleFactor;
    patternExpected = patternExpected;
}

uint32_t TimeRecBrute::CombineBits(std::vector<int> bits, int numOfBits, int offset) {
    uint32_t result = 0;
    if(numOfBits > 32){
        LOG2CONSOLE("too many bits");
        return 0;
    }


    for(int i = 0; i < numOfBits; i++) {
        result |= (bits[-i + offset + numOfBits - 1] << (i));
    }
    

    return result;
}