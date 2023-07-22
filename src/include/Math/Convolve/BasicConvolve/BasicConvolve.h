#include "Math/Convolve/IConvolve.h"
#include "Math/ComplexArray/ComplexArray.h"

#pragma once

class BasicConvolve : public IConvolve {
    public:
    virtual ComplexArray DoConvolve(ComplexArray &input, ComplexArray &filter);
    virtual void DoConvolve(ComplexArray &input, ComplexArray &filter, ComplexArray &output);
    virtual void DoConvolve(ComplexArray &input, ComplexArray &filter, ComplexArray &output, int inputConvolveSize);
    virtual int GetResultingSize(int input, int filter);
    
};