#include "Math/Convolve/BasicConvolve/BasicConvolve.h"

RealArray BasicConvolve::DoConvolve(RealArray &input, RealArray &filter){
    int filterSize = filter.GetElementSize();
    int inputSize = input.GetElementSize();
    int resultSize = GetResultingSize(inputSize, filterSize), resultIndex = 0;

    float temp;

    RealArray result(resultSize);

    for(int i = 0; i < filterSize - 1; i++) {
        temp = 0;
        for(int k = 0; k < i + 1; k++) {
            temp += input[i - k] * filter[k];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

    for(int i = 0; i < inputSize - filterSize + 1; i++) {
        temp = 0;
        for(int k = 0; k < filterSize; k++) {
            temp += input[i + k] * filter[filterSize - k - 1];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

    for(int i = inputSize - filterSize + 1; i < inputSize; i++) {
        temp = 0;
        for(int k = 0; k < filterSize - (i - inputSize + filterSize); k++) {
            temp += input[i + k] * filter[filterSize - k - 1];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

    result.SetSize(resultSize);

    return result;
}
//TODO no copy, give reference
void BasicConvolve::DoConvolve(RealArray &input, RealArray &filter, RealArray &output){
    output = DoConvolve(input, filter);
}

void BasicConvolve::DoConvolve(RealArray &input, RealArray &filter, RealArray &result, int inputConvolveSize) {
    int filterSize = filter.GetElementSize();
    int inputSize = inputConvolveSize;
    int resultIndex = 0;

    float temp;


    for(int i = 0; i < filterSize - 1; i++) {
        temp = 0;
        for(int k = 0; k < i + 1; k++) {
            temp += input[i - k] * filter[k];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

    for(int i = 0; i < inputSize - filterSize + 1; i++) {
        temp = 0;
        for(int k = 0; k < filterSize; k++) {
            temp += input[i + k] * filter[filterSize - k - 1];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

    for(int i = inputSize - filterSize + 1; i < inputSize; i++) {
        temp = 0;
        for(int k = 0; k < filterSize - (i - inputSize + filterSize); k++) {
            temp += input[i + k] * filter[filterSize - k - 1];
        }
        result[resultIndex] = temp;
        resultIndex++;
    }

}

int BasicConvolve::GetResultingSize(int input, int filter){
    return input + filter - 1;
}