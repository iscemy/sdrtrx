#include "Math/ComplexArray/ComplexArray.h"
#pragma once
class IConvolve {
    public:
    virtual ComplexArray DoConvolve(ComplexArray &input, ComplexArray &filter) = 0;
    virtual void DoConvolve(ComplexArray &input, ComplexArray &filter, ComplexArray &output) = 0;
    virtual void DoConvolve(ComplexArray &input, ComplexArray &filter, ComplexArray &output, int inputConvolveSize) = 0;
    virtual int GetResultingSize(int input, int filter) = 0;
};