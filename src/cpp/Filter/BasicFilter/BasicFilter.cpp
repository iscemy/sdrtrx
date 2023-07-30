#include "Filter/BasicFilter/BasicFilter.h"

void BasicFilter::RunFilter(RealArray &input, RealArray &output, bool FixFirOffset){
    pConv->DoConvolve(input, taps, output);
    if(FixFirOffset) {
        int offset = (taps.GetElementSize() - 1) / 2;
        for(int i = offset; i < output.GetElementSize() - offset; i++) {
            output[i] = output[i+offset];
        }
        output.SetSize(output.GetElementSize() - offset);
    }
}

int BasicFilter::GetOutputSize(int inputSize){
    if(isTapsSet)
        return pConv->GetResultingSize(inputSize, taps.GetElementSize());
    else 
        return -1;

}

BasicFilter::BasicFilter(IConvolve *pConvolve) {
    this->pConv = pConvolve;
}
void BasicFilter::SetFilterTaps(RealArray &taps) {
    isTapsSet = true;
    this->taps = taps;
}
