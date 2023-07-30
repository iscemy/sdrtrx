#include "Math/RealArray/RealArray.h"
#pragma once
class IRadio {
    public:
    virtual int Receive(RealArray &data) = 0;
    virtual int Transmit(RealArray &data) = 0;
};