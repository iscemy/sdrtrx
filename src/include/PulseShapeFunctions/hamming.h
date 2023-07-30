#include "Math/RealArray/RealArray.h"
#include <cmath>
namespace PulseShaping {

    void hamming(RealArray &output) {
        int N = output.GetElementSize();

        for(int i = 0; i < N; i++) {
            output[i] = 0.54 - 0.46 * std::cos(2.0 * M_PI * ((float) i) / ((float)N));
        }

    }
    
}