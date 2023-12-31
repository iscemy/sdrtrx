#include "Math/RealArray/RealArray.h"

class DualPll {
public:
    DualPll(float fs, float f0, int BufferSize = 262144); // TODO: remove f0 if not needed
    void RunAlgorithm(RealArray &input, RealArray &output, float coarsef0);
    bool getIsLocked();

    float GetLastPhase(); // needed ??
    float GetLastFreq(); // needed ??


private:
    bool isFirstRun = true;
    double td1init, td2init;
    int bufferSize = 0;
    bool isLocked;
    float f0, fs;
    RealArray t1, t2;
    double *t1d = nullptr, *t2d = nullptr;
    void ResizeBuffersIfNeeded(int size);
    void ValidateLockStatus();
};