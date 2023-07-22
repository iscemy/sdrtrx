#define BANDPASS       1
#define DIFFERENTIATOR 2
#define HILBERT        3


void remez(double h[], int *numtaps,
      int *numband, const double bands[], 
      const double des[], const double weight[],
      int *type, int *griddensity);