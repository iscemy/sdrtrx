#include "CarrierRecovery/PLLPhaseFreq/DualPll/DuallPll.h"
#include "cmath"
using namespace std;

DualPll::DualPll(float fs, float f0, int BufferSize) { 
    this->f0 = f0;
    this->fs = fs;
    t1.LateInit(BufferSize);
    t2.LateInit(BufferSize);
}

void DualPll::RunAlgorithm(RealArray &input, RealArray &output, float coarsef0) {
    if (input.GetElementSize() > t1.GetElementSize()) {
        //TODO resize
    }
    #if 0
    float mu1 = 0.05;
    float mu2 = 0.005;
    t1[0] = 0;
    t2[0] = 0;
    
    for(int i = 0; i < input.GetElementSize(); i++) {
        t1[i+1] = t1[i] - mu1 * input[i] * sin(4.0f * ((float)M_PI) * coarsef0 * ((float) i) * 1.0f/fs + 2.0f*t1[i]);
        t2[i+1] = t2[i] - mu2 * input[i] * sin(4.0f * ((float)M_PI)  * coarsef0 * ((float) i) * 1.0f/fs + 2.0f*t1[i] + 2.0f * t2[i]);
        output[i] = cos(2.0f*((float)M_PI)*coarsef0/fs*((float) i)+t1[i]+t2[i]);
    }
    #else
    output.SetSize(input.GetElementSize());
    ResizeBuffersIfNeeded(input.GetElementSize());
    double mu1 = 0.03;
    double mu2 = 0.003;

    // if(isFirstRun){
    //     t1d[0] = 0;
    //     t2d[0] = 0;
    // }
    t1d[0] = td1init;
    t2d[0] = td2init;
    double coarsef0d = coarsef0, fsd = fs;
    for(int i = 0; i < input.GetElementSize(); i++) {
        t1d[i+1] = t1d[i] - mu1 * ((double)input[i]) * sin(4.0 * (M_PI) *  coarsef0 * ((double) i) * 1.0f/fsd + 2.0*t1d[i]);
        t2d[i+1] = t2d[i] - mu2 * ((double)input[i]) * sin(4.0 * (M_PI)  * coarsef0 * ((double) i) * 1.0f/fsd + 2.0*t1d[i] + 2.0 * t2d[i]);
        output[i] = cos(2.0*(M_PI)*coarsef0d/fsd*((double) i)+t1d[i]+t2d[i]);
    }

    td1init =  t1d[input.GetElementSize()-1];
    td2init =  t2d[input.GetElementSize()-1];

    

    #endif

}

void DualPll::ResizeBuffersIfNeeded(int size) {
    if(this->bufferSize < size) {
        if(t1d != nullptr) {
            delete t1d;
        }
        if(t2d != nullptr) {
            delete t2d;
        }

        t1d = new double[(int)(size * 1.5)];
        t2d = new double[(int)(size * 1.5)];

        this->bufferSize = size * 1.5;
    }
}

bool DualPll::getIsLocked() {
    return isLocked;
}