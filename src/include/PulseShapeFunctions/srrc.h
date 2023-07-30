#include "Math/RealArray/RealArray.h"
#include "cmath"
#pragma once 

namespace PulseShaping {


    void s_rrc(float syms, float beta, float P, float t_off, RealArray &output) {

        int size = 2 * (syms*P+1e-6+t_off);
        output = RealArray(size);
        const float pi = M_PI;
        int i = 0;
        for(float k = -syms * P + t_off; k < syms*P+t_off; k+=1.0f) {
            k+=1e-6;
            output[i] = 4.0f*beta/sqrt(P)*(cos((1.0f+beta)*pi*k/P)+sin((1.0f-beta)*pi*k/P)/(4.0f*beta*k/P))/(pi*(1.0f-16.0f*(beta*k/P) * (beta*k/P)));
            if(std::isinf(output[i]))
                printf("%d\n", i);
            i++;
        }

    }

}