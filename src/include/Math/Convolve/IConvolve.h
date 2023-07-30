#include "Math/RealArray/RealArray.h"
#pragma once
class IConvolve {
    public:
    virtual RealArray DoConvolve(RealArray &input, RealArray &filter) = 0;
    virtual void DoConvolve(RealArray &input, RealArray &filter, RealArray &output) = 0;
    virtual void DoConvolve(RealArray &input, RealArray &filter, RealArray &output, int inputConvolveSize) = 0;
    virtual int GetResultingSize(int input, int filter) = 0;
};