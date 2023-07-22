#include "Math/FFTW/FFTW.h"
#include "fftw3.h"
#include "Utilities/log.h"
FFTW::FFTW(int size)
{
    // inBuffer = new fftwf_complex[size];
    // outBuffer = new fftwf_complex[size];

    inBuffer  = (fftwf_complex*) fftw_malloc(sizeof(fftwf_complex) * size);
    outBuffer = (fftwf_complex*) fftw_malloc(sizeof(fftwf_complex) * size);
    forwardPlan = fftwf_plan_dft_1d(size, inBuffer, outBuffer,
                                    FFTW_FORWARD, FFTW_ESTIMATE);
    backwardPlan = fftwf_plan_dft_1d(size, inBuffer, outBuffer,
                                    FFTW_BACKWARD, FFTW_ESTIMATE);      

    planComplexNumberSize = size;                        
}

void FFTW::DoForwardFFT(ComplexArray &input, ComplexArray &output) {
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
    fftwf_execute(forwardPlan);
    output.SetBuffer((float*)outBuffer, planComplexNumberSize, 2);
    
    output.SwapHalfs();
}
void FFTW::DoBackwardFFT(ComplexArray &input, ComplexArray &output) {
    input.GetBuffer((float*)inBuffer, planComplexNumberSize * 2);
    fftwf_execute(backwardPlan);
    output.SetBuffer((float*)outBuffer, planComplexNumberSize, 2);
    output.SwapHalfs();
}

FFTW::~FFTW()
{
}