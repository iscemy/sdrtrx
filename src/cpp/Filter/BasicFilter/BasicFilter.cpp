#include "Filter/BasicFilter/BasicFilter.h"

void BasicFilter::RunFilter(ComplexArray &input, ComplexArray &output){
    pConv->DoConvolve(input, taps, output);
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
void BasicFilter::SetFilterTaps(ComplexArray &taps) {
    isTapsSet = true;
    this->taps = taps;
}
