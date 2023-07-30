#include "Math/RealArray/RealArray.h"

#include "Math/RealArray/RealArray.h"
#include "algorithm"
#include "cstring"
#include "cstdio"

void RealArray::CreateBuffers(int size) {
    buffer = new float[size];
    ptemp = new float[size];
    this->size = size;
    this->currentBuffersElementSize = size;
    isBufferOwnedByThis = true;
    isBufferInited = true;
}

RealArray::RealArray() {
    isBufferOwnedByThis = false;
    isBufferInited = false;
}

RealArray::RealArray(int size)
{
    CreateBuffers(size);
}

RealArray::RealArray(const RealArray & ca) {
    CreateBuffers(ca.size);
    std::copy(ca.buffer, ca.buffer + ca.size, buffer);
}

RealArray::RealArray(const RealArray & ca, int offset) {
    CreateBuffers(ca.size - offset);
    std::copy(ca.buffer, ca.buffer  + ca.size - offset, buffer);
}

int RealArray::LateInit(int size) {
    CreateBuffers(size);
    return 0;
}

RealArray& RealArray::operator*=(const RealArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] *= rhs.buffer[i];
    }
    return *this;
}

RealArray& RealArray::operator/=(const RealArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] /= rhs.buffer[i];
    }
    return *this;
}

RealArray& RealArray::operator+=(const RealArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] += rhs.buffer[i];
    }
    return *this;
}

RealArray& RealArray::operator-=(const RealArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] -= rhs.buffer[i];
    }
    return *this;
}

RealArray RealArray::operator*(const RealArray& rhs){
    RealArray result(*this);
    result *= rhs;
    return result;
}
RealArray RealArray::operator/(const RealArray& rhs){
    RealArray result(*this);
    result /= rhs;
    return result;
}
RealArray RealArray::operator+(const RealArray& rhs){
    RealArray result(*this);
    result += rhs;
    return result;
}
RealArray RealArray::operator-(const RealArray& rhs){
    RealArray result(*this);
    result -= rhs;
    return result;
}

RealArray& RealArray::operator=(RealArray& rhs) {
    if(this == &rhs) 
        return *this;
    if(!isBufferInited)
        LateInit(rhs.size);
    else 
        HandleBufferSize(rhs.size);

    std::copy(rhs.buffer, rhs.buffer + rhs.size, buffer);
    return *this;
}

void RealArray::operator=(const RealArray& rhs) {
    if(this == &rhs) 
        return;

    if(!isBufferInited)
        LateInit(rhs.size);
    else 
        HandleBufferSize(rhs.size);

    std::copy(rhs.buffer, rhs.buffer + rhs.size, buffer);
}

float & RealArray::operator[](int index) {
    return buffer[index];
}


RealArray::~RealArray()
{
    if (isBufferOwnedByThis && isBufferInited){
        delete buffer;
        delete ptemp;    
    }
}

void RealArray::SetBuffer(int16_t *start, int inputSize, int step) {
    HandleBufferSize(inputSize);
    for(int i = 0; i < inputSize; i++) {
        buffer[i] = start[step*i];
    }
}

void RealArray::SetBuffer(float *start, int inputSize, int step) {
    HandleBufferSize(inputSize);
    for(int i = 0; i < inputSize; i++) {
        buffer[i] = start[step*i];
    }
}


void RealArray::SetBuffer(std::vector<float> start, int offset, size_t copysize) {
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}

void RealArray::SetBuffer(std::vector<double> start, int offset, size_t copysize){
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}

int RealArray::GetBuffer(float *RequestedBuffer, int bufferSize) {
    if(bufferSize < size * 2) {
        return -1;
    }

    // for(int i = 0; i < size; i++) {
    //     RequestedBuffer[2*i] = buffer[i].real();
    // }

    memcpy(RequestedBuffer, buffer, size);
    return 0;
}


int RealArray::GetElementSize() {
    return size;
}
int RealArray::GetBufferLenghtFloats() {
    return size * 2;
}

int RealArray::GetBufferSizeInBytes() {
    return size * 2 * sizeof(float);
}

int RealArray::GetMaxPos() {
    auto maxelem = std::max_element(buffer, buffer + size, RealArray::ComplexCompare);
    int result = std::distance(buffer, maxelem);
    return result;
}

int RealArray::GetMinPos() {
    auto minelem = std::min_element(buffer, buffer + size, RealArray::ComplexCompare);
    int result = std::distance(buffer, minelem);
    return result;
}

float RealArray::GetMaxValAbs() {
    return std::abs(buffer[GetMaxPos()]);
}

float RealArray::GetMinValAbs() {
    return std::abs(buffer[GetMinPos()]);
}

void RealArray::SwapHalfs() {
    std::copy(buffer + size / 2, buffer + size, ptemp);
    std::copy(buffer, buffer + size / 2, buffer + size / 2);
    std::copy(ptemp, ptemp + size/2, buffer);
}

RealArray RealArray::GetSorted() {
    RealArray result(*this);
    result.Sort();
    return result;
}

void RealArray::Sort() {
    std::sort(buffer, buffer + size, RealArray::ComplexCompare);
}

std::vector<std::pair<float, int>> RealArray::SortWithIndexes() {
    std::vector<std::pair<float, int>> result;
    for(int i = 0; i < size; i++) {
        result.push_back(std::pair<float, int>(buffer[i], i));
    }
    std::sort(result.begin(), result.end(), RealArray::ComplexComprarePairs);
    return result;
}

void RealArray::PrintContentsForPython() {
    printf("\na = [");
    for(int i = 0; i < size; i++) {
        printf("%f,", buffer[i]);
    }
    printf("]\n");
}


void RealArray::Normalize() {
    float max, min;
    Normalize(min, max);
}

void RealArray::Normalize(float &minAbs, float &maxAbs) {
    maxValBeforeNormalize = buffer[GetMaxPos()];
    minValBeforeNormalize = buffer[GetMinPos()];
    avarageBeforNormalize = 0;
    maxAbs = std::abs(maxValBeforeNormalize);
    minAbs = std::abs(minValBeforeNormalize);
    for(int i = 0; i < size; i++) {
        avarageBeforNormalize += buffer[i] / ((float)size);
        buffer[i] = buffer[i] / std::abs(maxValBeforeNormalize - minValBeforeNormalize) * 2.0f;
    }
}


void RealArray::ScaleWith(float scalar) {
    for(int i = 0; i < size; i++) {
        buffer[i] = buffer[i] * scalar;
    }
}

void RealArray::GetNonZeroIndexes(std::vector<std::pair<int,int>> &indices, int slack) {
    float absval;
    bool isNonZeroStart = false;
    int indexStart = 0, zeroCounter = 0, oneCounter = 0;


    float treshold = GetMaxValAbs() * 0.5;
    // float treshold = 0.65;

    for(int i = 0; i < size; i++) {
        absval = std::abs(buffer[i]);
        if(absval < treshold) {
            zeroCounter++;
            oneCounter = 0;
            if(isNonZeroStart && (zeroCounter > 50)) {
                indices.push_back(std::pair<int, int>(indexStart, i));
                isNonZeroStart = false;
            }
        } else {
            zeroCounter = 0;
            oneCounter++;
            if((oneCounter > 0) && (!isNonZeroStart)) {
                indexStart = i;
                if(indexStart < 0) 
                    indexStart = 0;
                isNonZeroStart = true;
            }
        }
    }
    if(isNonZeroStart) {
        indices.push_back(std::pair<int, int>(indexStart, size-1));
    }

    if(slack > 0) {
        for(std::pair<int, int> &index : indices) {
            if((index.first - slack) > 0) {
                index.first -= slack;
            }
            if((index.second + slack) < GetElementSize()) {
                index.second += slack;
            }
        } 
    }

    // for(auto &index : indices) {
    //     index.first = index.first-50;
    //     if(index.first < 0) {
    //         index.first = 0;
    //     }
    // }
}


void RealArray::HandleBufferSize(int newSize) {
    size = newSize;
    if(newSize != this->size) {
        size = newSize;
    }
    if(newSize <= currentBuffersElementSize) 
        return;
    int newBufferElementSize = (int) newSize * 1.5;
    delete ptemp;
    delete buffer;
    buffer = new float[newBufferElementSize];
    ptemp = new float[newBufferElementSize];
    currentBuffersElementSize = newBufferElementSize;

}

int RealArray::SetSize(int newSize) {
    if(newSize < currentBuffersElementSize) {

    } else {
        HandleBufferSize(newSize);
    }
    if(newSize != this->size) {
        size = newSize;
    }
    return 0;
}

// int RealArray::SetSize(int newSize) {
//     if(newSize < currentBuffersElementSize) {
//         if(newSize != this->size) {
//             size = newSize;
//         }
//     } else {
//         HandleBufferSize(newSize);
//     }
//     return 0;
// }

float RealArray::GetMeanAbs() {
    float result = 0;
    for(int i = 0; i < size; i++) {
        result += std::abs(buffer[i]) / (float) size;
    }

    return result;
}

float RealArray::GetMean() {
    float result = 0;
    for(int i = 0; i < size; i++) {
        result += (buffer[i]) / (float) size;
    }

    return result;
}

void RealArray::PartialCopyTwoArrays(RealArray& in1, int offset1, int size1, RealArray& in2, int offset2, int size2) {
    int resultingSize = size1 + size2;
    SetSize(resultingSize);
    std::copy(in1.buffer+offset1, in1.buffer + offset1 + size1, buffer);
    std::copy(in2.buffer+offset2, in2.buffer + offset2 + size2, buffer+size1);
}

void RealArray::CopyFromAnotherArray(RealArray& input, int offset, int copySize) {
    int resultingSize = copySize;
    SetSize(resultingSize);
    std::copy(input.buffer+offset, input.buffer + offset + copySize, buffer);
}