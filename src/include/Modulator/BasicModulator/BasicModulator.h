#include "Math/RealArray/RealArray.h"
#include "Filter/IFilter.h"

class BasicModulator {
public:
    BasicModulator(float fsHz, float filterFreqHz, float filterBWHz);
    void RunAlgorithm(RealArray &interfreq, RealArray &carrier, RealArray &baseband);
    void ChangeParameters(float fsHz, float filterFreqHz, float filterBWHz);
    int getOuputSize(int inputSize);
private:
    IFilter *pFilter;
    
    float fsHz, filterFreqHz, filterBWHz;
    void CreateFilter();
    
};