#include "Math/RealArray/RealArray.h"

#pragma once

class IFilter {
public:
    virtual void RunFilter(RealArray &input, RealArray &output, bool fixFirOffset = false) = 0;
    virtual int GetOutputSize(int inputSize) = 0;
};