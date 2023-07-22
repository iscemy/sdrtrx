#include "Math/ComplexArray/ComplexArray.h"
#pragma once
class IRadio {
    public:
    virtual int Receive(ComplexArray &data) = 0;
    virtual int Transmit(ComplexArray &data) = 0;
};