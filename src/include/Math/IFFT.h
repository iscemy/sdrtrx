#include "Math/ComplexArray/ComplexArray.h"

#pragma once
class IFFT {
    public:
    virtual void DoForwardFFT(ComplexArray &input, ComplexArray &output) = 0;
    virtual void DoBackwardFFT(ComplexArray &input, ComplexArray &output) = 0;

};