#include "Math/ComplexArray/ComplexArray.h"
#include "Filter/IFilter.h"
class BasicModulator {
public:
    BasicModulator(float fsHz, float filterFreqHz, float filterBWHz);
    void RunAlgorithm(ComplexArray &interfreq, ComplexArray &carrier, ComplexArray &baseband);
    void ChangeParameters(float fsHz, float filterFreqHz, float filterBWHz);
    int getOuputSize(int inputSize);
private:
    IFilter *pFilter;
    
    float fsHz, filterFreqHz, filterBWHz;
    void CreateFilter();
    
};