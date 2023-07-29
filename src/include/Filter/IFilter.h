#include "Math/ComplexArray/ComplexArray.h"

#pragma once

class IFilter {
public:
    virtual void RunFilter(ComplexArray &input, ComplexArray &output, bool fixFirOffset = false) = 0;
    virtual int GetOutputSize(int inputSize) = 0;
};