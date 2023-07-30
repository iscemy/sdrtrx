#include "Math/FFTW/FFTW.h"
#include "fftw3.h"
#include "Utilities/log.h"
FFTW::FFTW(int size)
{
    // inBuffer = new fftwf_complex[size];
    // outBuffer = new fftwf_complex[size];

    inBuffer  = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    outBuffer = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
    forwardPlan = fftw_plan_dft_1d(size, inBuffer, outBuffer,
                                    FFTW_FORWARD, FFTW_ESTIMATE);
    backwardPlan = fftw_plan_dft_1d(size, inBuffer, outBuffer,
                                    FFTW_BACKWARD, FFTW_ESTIMATE);      

    planComplexNumberSize = size;                        
}

void FFTW::DoForwardFFT(RealArray &input, RealArray &output) {
    int inputSize = input.GetElementSize(), fftBufferSize = planComplexNumberSize;
    int fftBufferOffset = (fftBufferSize - inputSize)/2;
    if(inputSize <= fftBufferSize) {
        if(inputSize != fftBufferSize) {
            LOG2CONSOLE("fft buffer size and input size not equal. this will cause reduced performance due to extra memory copies");
            memset(inBuffer, 0, fftBufferOffset);
            memset(inBuffer + fftBufferOffset + inputSize, 0, fftBufferOffset);
        }
        input.GetBuffer((float*)(inBuffer+fftBufferOffset), (planComplexNumberSize-fftBufferOffset) * 2);
    } else {
        LOG2CONSOLE("fft buffer size is smaller than fft input size,")
        return;
    } 
    fftw_execute(forwardPlan);
    output.SetBuffer((float*)outBuffer, planComplexNumberSize, 2);
    
    output.SwapHalfs();
}
void FFTW::DoBackwardFFT(RealArray &input, RealArray &output) {
    input.GetBuffer((float*)inBuffer, planComplexNumberSize * 2);
    fftw_execute(backwardPlan);
    output.SetBuffer((float*)outBuffer, planComplexNumberSize, 2);
    output.SwapHalfs();
}

FFTW::~FFTW()
{
}