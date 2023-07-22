#include "Math/IFFT.h"
#include "fftw3.h"
class FFTW : public IFFT
{
private:
    /* data */
    //currently all operations done in internal buffers, extra copying is done, maybe optimize this in future
    fftwf_complex *inBuffer, *outBuffer;
    fftwf_plan forwardPlan, backwardPlan;
    int planComplexNumberSize;
public:
    void DoForwardFFT(ComplexArray &input, ComplexArray &output);
    void DoBackwardFFT(ComplexArray &input, ComplexArray &output);
    
    FFTW(int size);
    ~FFTW();

};


