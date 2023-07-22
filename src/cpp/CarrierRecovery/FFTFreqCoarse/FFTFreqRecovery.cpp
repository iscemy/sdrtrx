#include "CarrierRecovery/FFTFreqCoarse/FFTFreqRecovery.h"
#include "Math/FFTW/FFTW.h"
FFTFreqRecovery::FFTFreqRecovery(IFFT *fft) {
    this->pFFT = fft;
}

float FFTFreqRecovery::processBuffer(ComplexArray &Array, float f0, float df0, float &freq, float fs) {
    ComplexArray inputInF(Array.GetElementSize());
    pFFT->DoForwardFFT(Array, inputInF);
    float N = inputInF.GetElementSize(), frequency, freq_amp = 0;
    // inputInF.PrintContentsForPython();
    std::vector<std::pair<std::complex<float>, int>> sorted = inputInF.SortWithIndexes();
    
    for(int i = sorted.size() - 1; i >= 0; i--) {
        auto & p = sorted[i];
        frequency = (p.second - N / 2) * fs / N / 2;
        if((frequency > (f0 - df0) && (frequency < (f0 + df0)))) {
            freq = frequency;
            freq_amp = abs(p.first);
            break;
        }
    }
    return freq_amp;
}