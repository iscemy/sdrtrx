#include "Filter/IFilter.h"
#include "Math/Convolve/IConvolve.h"

class BasicFilter : public IFilter {
public:
/*
todo generate its own filter taps
types band pass, low pass / number of taps, 
for band pass center freq, bw
for low pass bw,

filter - parent class
    RunFilter Convolve  
bandpass, lowpass, notch, -> child classes
    methods to set the parameters that used to generate taps
*/
    BasicFilter(IConvolve *pConvolve);
    void SetFilterTaps(ComplexArray &taps);
    void RunFilter(ComplexArray &input, ComplexArray &output, bool fixFirOffset = false);
    int GetOutputSize(int inputSize);

private:
    bool isTapsSet = false;
    ComplexArray taps;
    IConvolve *pConv;
};