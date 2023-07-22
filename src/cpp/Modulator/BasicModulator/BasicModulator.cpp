#include "Modulator/BasicModulator/BasicModulator.h"
#include "Filter/BasicFilter/BasicFilter.h"
#include "Math/Convolve/BasicConvolve/BasicConvolve.h"


BasicModulator::BasicModulator(float fsHz, float filterFreqHz, float filterBWHz){ 
    ChangeParameters(fsHz, filterFreqHz, filterBWHz);
}

void BasicModulator::RunAlgorithm(ComplexArray &interfreq, ComplexArray &carrier, ComplexArray &baseband){ 
    ComplexArray temp = carrier * interfreq;
    pFilter->RunFilter(temp, baseband);
}

void BasicModulator::ChangeParameters(float fsHz, float filterFreqHz, float filterBWHz){ 
    this->fsHz = fsHz;
    this->filterFreqHz = filterFreqHz;
    this->filterBWHz = filterBWHz;
    CreateFilter();
}

int BasicModulator::getOuputSize(int inputSize) {
    return pFilter->GetOutputSize(inputSize);
}

void BasicModulator::CreateFilter() {
    //todo create filter
    #if 1
    //for test purposes

    BasicFilter* pFilt = new BasicFilter(new BasicConvolve());
    ComplexArray taps(50);
    float tap_values[50] = {-1.29780457e-04, -4.16168084e-04, -7.87375111e-04, -1.29124619e-03,
                        -1.94827159e-03, -2.73719969e-03, -3.58506086e-03, -4.36357077e-03,
                        -4.89317168e-03, -4.95505188e-03, -4.31046417e-03, -2.72567076e-03,
                        1.63189134e-18,  4.00607547e-03,  9.34615817e-03,  1.59695984e-02,
                        2.37122249e-02,  3.22978544e-02,  4.13511720e-02,  5.04213347e-02,
                        5.90144372e-02,  6.66319358e-02,  7.28113812e-02,  7.71654573e-02,
                        7.94154017e-02,  7.94154017e-02,  7.71654573e-02,  7.28113812e-02,
                        6.66319358e-02,  5.90144372e-02,  5.04213347e-02,  4.13511720e-02,
                        3.22978544e-02,  2.37122249e-02,  1.59695984e-02,  9.34615817e-03,
                        4.00607547e-03,  1.63189134e-18, -2.72567076e-03, -4.31046417e-03,
                        -4.95505188e-03, -4.89317168e-03, -4.36357077e-03, -3.58506086e-03,
                        -2.73719969e-03, -1.94827159e-03, -1.29124619e-03, -7.87375111e-04,
                        -4.16168084e-04, -1.29780457e-04};
    taps.SetBufferReal(tap_values, 50);

    pFilt->SetFilterTaps(taps);
    pFilter = pFilt;
    #endif
}