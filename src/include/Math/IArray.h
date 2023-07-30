#include <cstdint>
#include <vector>
#include <complex>
#pragma once
class IArray
{
public:
    // IArray() {};
    ~IArray() {};
    virtual void *GetBufferPtr() = 0;
    virtual int LateInit(int newSize) = 0;
    virtual int GetElementSize() = 0;
    virtual int GetBufferLenghtFloats() = 0;
    virtual int GetBufferSizeInBytes() = 0;
    virtual int SetSize(int size) = 0;
    virtual void SetBuffer(int16_t *start, int inputSize, int step) = 0;
    virtual void SetBuffer(float *start, int inputSize, int step) = 0;
    virtual void SetBufferReal(float *start, int inputSize) = 0;
    virtual void SetBuffer(std::vector<std::complex<double>> start, int offset, size_t copysize) = 0;
    virtual void SetBuffer(std::vector<std::complex<float>> start, int offset, size_t copysize) = 0;
    virtual void SetBuffer(std::vector<double> start, int offset, size_t copysize) = 0;
    virtual void SetBuffer(std::vector<float> start, int offset, size_t copysize) = 0;
    virtual int GetBuffer(float *RequestedBuffer, int bufferSize) = 0;
    virtual int GetBufferOnlyReal(float *pBuffer, int bufferSize) = 0;
    virtual void SwapHalfs() = 0;
    virtual void PrintContentsForPython() = 0;
    virtual void SetAllImaginaryTo(float imag) = 0;
    virtual void ConjugateThis() = 0;
    virtual IArray *GetConjugate() = 0;
    virtual int GetMaxPos() = 0;
    virtual int GetMinPos() = 0;
    virtual float GetMaxValAbs() = 0;
    virtual float GetMinValAbs() = 0;
    virtual IArray *GetSorted() = 0;
    virtual void Sort() = 0;
    virtual void Normalize() = 0;
    virtual void Normalize(float &minVal, float &maxVal) = 0;
    virtual void GetNonZeroIndexes(std::vector<std::pair<int,int>> &indices, int slack = 0) = 0;
    virtual void ScaleWith(float scalar) = 0;
    virtual std::vector<std::pair<std::complex<float>, int>> SortWithIndexes() = 0;
    virtual float GetMeanAbs() = 0;
    virtual std::complex<float> GetMean() = 0;
    virtual void PartialCopyTwoArrays(IArray* in1, int offset1, int size1, IArray* in2, int offset2, int size2) = 0;
    virtual void CopyFromAnotherArray(IArray* input, int offset, int copySize) = 0;
};
