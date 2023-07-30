#include "Math/IFFT.h"
#include "fftw3.h"
class FFTW : public IFFT
{
private:
    /* data */
    //currently all operations done in internal buffers, extra copying is done, maybe optimize this in future
    fftw_complex *inBuffer, *outBuffer;
    fftw_plan forwardPlan, backwardPlan;
    int planComplexNumberSize;
public:
    void DoForwardFFT(RealArray &input, RealArray &output);
    void DoBackwardFFT(RealArray &input, RealArray &output);
    
    FFTW(int size);
    ~FFTW();

};


