#include "Math/Convolve/IConvolve.h"
#include "Math/RealArray/RealArray.h"

#pragma once

class BasicConvolve : public IConvolve {
    public:
    virtual RealArray DoConvolve(RealArray &input, RealArray &filter);
    virtual void DoConvolve(RealArray &input, RealArray &filter, RealArray &output);
    virtual void DoConvolve(RealArray &input, RealArray &filter, RealArray &output, int inputConvolveSize);
    virtual int GetResultingSize(int input, int filter);
    
};