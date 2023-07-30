#include "Math/RealArray/RealArray.h"

#pragma once
class IFFT {
    public:
    virtual void DoForwardFFT(RealArray &input, RealArray &output) = 0;
    virtual void DoBackwardFFT(RealArray &input, RealArray &output) = 0;

};