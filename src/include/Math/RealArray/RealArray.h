
#include "complex"
#include "vector"

#pragma once

class RealArray
{
private:
    /* data */
    int size = 0, currentBuffersElementSize;
    
    bool isBufferOwnedByThis = false;
    bool isBufferInited = false;

    float maxValBeforeNormalize = 0;
    float minValBeforeNormalize = 0;
    float avarageBeforNormalize = 0;

    // static bool ComplexCompare(const float &a, const float &b) {
    //     if(abs(a) < abs(b))
    //         return true;
    //     else 
    //         return false;
    // }

    // static bool ComplexComprarePairs(const std::pair<float, int> &a,
    //                                  const std::pair<float, int> &b) {
    //     if(abs(a.first) < abs(b.first))
    //         return true;
    //     else 
    //         return false;
    // }

    static bool ComplexCompare(const float &a, const float &b) {
        if(a < b)
            return true;
        else
            return false;
    }

    static bool ComplexComprarePairs(const std::pair<float, int> &a,
                                     const std::pair<float, int> &b) {
        if(a.first < b.first)
            return true;
        else
            return false;
    }

    void CreateBuffers(int size);
    void HandleBufferSize(int newSize);
public:
float *buffer, *ptemp;
    RealArray();
    RealArray(int size);
    RealArray(const RealArray & ca);
    RealArray(const RealArray & ca, int offset);
    
    ~RealArray();
    
    int LateInit(int newSize);

    int GetElementSize();
    int GetBufferLenghtFloats();
    int GetBufferSizeInBytes();
    
    int SetSize(int size);

    void SetBuffer(int16_t *start, int inputSize, int step);
    void SetBuffer(float *start, int inputSize, int step);
    void SetBuffer(std::vector<float> start, int offset, size_t copysize);
    void SetBuffer(std::vector<double> start, int offset, size_t copysize);

    int GetBuffer(float *RequestedBuffer, int bufferSize);
    void SwapHalfs();

    void PrintContentsForPython();

    RealArray& operator*=(const RealArray& rhs);
    RealArray& operator/=(const RealArray& rhs);
    RealArray& operator+=(const RealArray& rhs);
    RealArray& operator-=(const RealArray& rhs);
    RealArray& operator=(RealArray& rhs);

    RealArray operator*(const RealArray& rhs);
    RealArray operator/(const RealArray& rhs);
    RealArray operator+(const RealArray& rhs);
    RealArray operator-(const RealArray& rhs);
    
    void operator=(const RealArray& rhs);

    float & operator[](int index);

    int GetMaxPos();
    int GetMinPos();

    float GetMaxValAbs();
    float GetMinValAbs();
    RealArray GetSorted();
    void Sort();
    void Normalize();
    void Normalize(float &minVal, float &maxVal);
    void GetNonZeroIndexes(std::vector<std::pair<int,int>> &indices, int slack = 0);
    void ScaleWith(float scalar);
    std::vector<std::pair<float, int>> SortWithIndexes();

    float GetMeanAbs();
    float GetMean();

    void PartialCopyTwoArrays(RealArray& in1, int offset1, int size1, RealArray& in2, int offset2, int size2);
    void CopyFromAnotherArray(RealArray& input, int offset, int copySize);
};