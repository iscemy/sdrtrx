#include "CarrierRecovery/Preprocess/SquarePreprocess.h"
extern "C" {
    #include "FilterDesign/remez/remez.h"
}
SquarePreprocess::SquarePreprocess(float fs, float f0, int numOfTaps)
{
    f0 = f0;
    numOfTaps = sizeof(taps) / sizeof(float);
    fs = fs;

    filterTaps.LateInit(sizeof(taps) / sizeof(float));
    filterTaps.SetBufferReal(taps, sizeof(taps) / sizeof(float));
    this->numOfTaps = numOfTaps;
    // double *taps_temp = new double[numOfTaps];
    // int numband = 3, type = BANDPASS, griddensity = 16;
    // double *bands = new double[numband * 2];
    // double *des = new double[numband * 2];
    // double *weight = new double[numband];

    // float * filterTaps = new float[numOfTaps];

    // bands[0] = 0;
    // bands[1] = (f0-1000.0)/fs/2.0;
    // bands[2] = (f0)/fs/2.0;
    // bands[3] = (f0+2000.0)/fs/2.0;
    // bands[4] = (f0+2000.0 + 1000.0)/fs/2.0;
    // bands[5] = 0.5;

    // des[0] = 0;
    // des[1] = 0;
    // des[2] = 1;
    // des[3] = 1;
    // des[4] = 0;
    // des[5] = 0;

    // weight[0] = 1;
    // weight[1] = 1;
    // weight[2] = 1;

    // remez(taps_temp, &numOfTaps, &numband, (const double*)bands, (const double*) des,(const double*) weight, &type, &griddensity);

    // printf("\na = [");
    // for(int i = 0; i < numOfTaps; i++) {
    //     filterTaps[i] = taps_temp[i];
    //     printf("%f ", filterTaps[i]);
    // }
    // printf("];\n\n");

    // this->filterTaps.LateInit(numOfTaps);
    // this->filterTaps.SetBuffer(filterTaps, 0, numOfTaps);
    // this->numOfTaps = numOfTaps;
    // delete taps_temp;
    // delete filterTaps;

}

int SquarePreprocess::DoPreprocess(IConvolve *pConvolve, ComplexArray &input, ComplexArray &output) {
    ComplexArray temp = input * input;
    pConvolve->DoConvolve(temp, filterTaps, output);
    int offset = (filterSize - 1) / 2;
    for(int i = offset; i < output.GetElementSize() - offset; i++) {
        output[i] = output[i+offset];
    }
    output.SetSize(output.GetElementSize() - offset);
    return 0;
}

int SquarePreprocess::GetOutputSize(int InputSize) {
    return InputSize + numOfTaps - 1;
}

SquarePreprocess::~SquarePreprocess()
{

}
