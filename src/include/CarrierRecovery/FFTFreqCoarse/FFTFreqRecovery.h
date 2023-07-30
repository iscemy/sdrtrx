#include "Math/IFFT.h"

class FFTFreqRecovery {
public:
    FFTFreqRecovery(IFFT *fft); 
    float processBuffer(RealArray &Array, float f0, float df0, float &freq, float fs);
private:
    IFFT *pFFT;
};