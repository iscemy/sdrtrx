#include "complex"
#include "vector"

#pragma once
/*
TODO : create proper initialization scheme for late init.
currently no buffer is generated for default constructor.
for a instance with created with default constructor assignment is needed

todo: handle assignment for a non initialized array

TODO:make the size changeable,
    -asignments make the size change
    -throw exception operations with different size arrays
*/
class ComplexArray
{
private:
    /* data */
    int size = 0, currentBuffersElementSize;
    
    bool isBufferOwnedByThis = false;
    bool isBufferInited = false;

    // static bool ComplexCompare(const std::complex<float> &a, const std::complex<float> &b) {
    //     if(abs(a) < abs(b))
    //         return true;
    //     else 
    //         return false;
    // }

    // static bool ComplexComprarePairs(const std::pair<std::complex<float>, int> &a,
    //                                  const std::pair<std::complex<float>, int> &b) {
    //     if(abs(a.first) < abs(b.first))
    //         return true;
    //     else 
    //         return false;
    // }

        static bool ComplexCompare(const std::complex<float> &a, const std::complex<float> &b) {
        if(a.real() < b.real())
            return true;
        else if(a.real() == b.real())
            if(a.imag() < b.imag())
                return true;
            else
                return false;
        else
            return false;
    }

    static bool ComplexComprarePairs(const std::pair<std::complex<float>, int> &a,
                                     const std::pair<std::complex<float>, int> &b) {
        if(a.first.real() < b.first.real())
            return true;
        else if(a.first.real() == b.first.real())
            if(a.first.imag() < b.first.imag())
                return true;
            else
                return false;
        else
            return false;
    }

    void CreateBuffers(int size);
    void HandleBufferSize(int newSize);
public:
std::complex<float> *buffer, *ptemp;
    ComplexArray();
    ComplexArray(int size);
    ComplexArray(const ComplexArray & ca);
    ComplexArray(const ComplexArray & ca, int offset);
    
    ~ComplexArray();
    
    int LateInit(int newSize);

    int GetElementSize();
    int GetBufferLenghtFloats();
    int GetBufferSizeInBytes();
    
    int SetSize(int size);

    void SetBuffer(int16_t *start, int inputSize, int step);
    void SetBuffer(float *start, int inputSize, int step);
    void SetBufferReal(float *start, int inputSize);
    void SetBuffer(std::vector<std::complex<double>> start, int offset, size_t copysize);
    void SetBuffer(std::vector<std::complex<float>> start, int offset, size_t copysize);
    void SetBuffer(std::vector<double> start, int offset, size_t copysize);
    void SetBuffer(std::vector<float> start, int offset, size_t copysize);

    int GetBuffer(float *RequestedBuffer, int bufferSize);
    int GetBufferOnlyReal(float *pBuffer, int bufferSize);
    void SwapHalfs();

    void PrintContentsForPython();
    void SetAllImaginaryTo(float imag);

    ComplexArray& operator*=(const ComplexArray& rhs);
    ComplexArray& operator/=(const ComplexArray& rhs);
    ComplexArray& operator+=(const ComplexArray& rhs);
    ComplexArray& operator-=(const ComplexArray& rhs);
    ComplexArray& operator=(ComplexArray& rhs);

    ComplexArray operator*(const ComplexArray& rhs);
    ComplexArray operator/(const ComplexArray& rhs);
    ComplexArray operator+(const ComplexArray& rhs);
    ComplexArray operator-(const ComplexArray& rhs);
    
    void operator=(const ComplexArray& rhs);

    std::complex<float> & operator[](int index);

    void ConjugateThis();
    ComplexArray GetConjugate();

    int GetMaxPos();
    int GetMinPos();
    ComplexArray GetSorted();
    void Sort();
    void Normalize();
    void GetNonZeroIndexes(std::vector<std::pair<int,int>> &indices, int slack = 0);
    void ScaleWith(float scalar);
    std::vector<std::pair<std::complex<float>, int>> SortWithIndexes();

    float GetMeanAbs();
    std::complex<float> GetMean();

    void PartialCopyTwoArrays(ComplexArray& in1, int offset1, int size1, ComplexArray& in2, int offset2, int size2);
    void CopyFromAnotherArray(ComplexArray& input, int offset, int copySize);
};


