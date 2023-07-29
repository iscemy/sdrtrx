#include "Math/ComplexArray/ComplexArray.h"
#include "algorithm"

void ComplexArray::CreateBuffers(int size) {
    buffer = new std::complex<float>[size];
    ptemp = new std::complex<float>[size];
    this->size = size;
    this->currentBuffersElementSize = size;
    isBufferOwnedByThis = true;
    isBufferInited = true;
}

ComplexArray::ComplexArray() {
    isBufferOwnedByThis = false;
    isBufferInited = false;
}

ComplexArray::ComplexArray(int size)
{
    CreateBuffers(size);
}

ComplexArray::ComplexArray(const ComplexArray & ca) {
    CreateBuffers(ca.size);
    std::copy(ca.buffer, ca.buffer + ca.size, buffer);
}

ComplexArray::ComplexArray(const ComplexArray & ca, int offset) {
    CreateBuffers(ca.size - offset);
    std::copy(ca.buffer, ca.buffer  + ca.size - offset, buffer);
}

int ComplexArray::LateInit(int size) {
    CreateBuffers(size);
    return 0;
}

ComplexArray& ComplexArray::operator*=(const ComplexArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] *= rhs.buffer[i];
    }
    return *this;
}

ComplexArray& ComplexArray::operator/=(const ComplexArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] /= rhs.buffer[i];
    }
    return *this;
}

ComplexArray& ComplexArray::operator+=(const ComplexArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] += rhs.buffer[i];
    }
    return *this;
}

ComplexArray& ComplexArray::operator-=(const ComplexArray& rhs) {
    for(int i = 0; i < size; i++) {
        this->buffer[i] -= rhs.buffer[i];
    }
    return *this;
}

ComplexArray ComplexArray::operator*(const ComplexArray& rhs){
    ComplexArray result(*this);
    result *= rhs;
    return result;
}
ComplexArray ComplexArray::operator/(const ComplexArray& rhs){
    ComplexArray result(*this);
    result /= rhs;
    return result;
}
ComplexArray ComplexArray::operator+(const ComplexArray& rhs){
    ComplexArray result(*this);
    result += rhs;
    return result;
}
ComplexArray ComplexArray::operator-(const ComplexArray& rhs){
    ComplexArray result(*this);
    result -= rhs;
    return result;
}

ComplexArray& ComplexArray::operator=(ComplexArray& rhs) {
    if(this == &rhs) 
        return *this;
    if(!isBufferInited)
        LateInit(rhs.size);
    else 
        HandleBufferSize(rhs.size);

    std::copy(rhs.buffer, rhs.buffer + rhs.size, buffer);
    return *this;
}

void ComplexArray::operator=(const ComplexArray& rhs) {
    if(this == &rhs) 
        return;

    if(!isBufferInited)
        LateInit(rhs.size);
    else 
        HandleBufferSize(rhs.size);

    std::copy(rhs.buffer, rhs.buffer + rhs.size, buffer);
}

std::complex<float> & ComplexArray::operator[](int index) {
    return buffer[index];
}

void ComplexArray::ConjugateThis() {
    for(int i = 0; i < size; i++) {
        buffer[i] = conj(buffer[i]);
    }
}

ComplexArray ComplexArray::GetConjugate() {
    ComplexArray result(size);
    for(int i = 0; i < size; i++) {
        result.buffer[i] = conj(this->buffer[i]);
    }
    return result;
}

ComplexArray::~ComplexArray()
{
    if (isBufferOwnedByThis && isBufferInited){
        delete buffer;
        delete ptemp;    
    }
}

void ComplexArray::SetBuffer(int16_t *start, int inputSize, int step) {
    HandleBufferSize(inputSize);
    for(int i = 0; i < inputSize; i++) {
        buffer[i] = std::complex<float>(start[step*i], start[step*i+1]);
    }
}

void ComplexArray::SetBuffer(float *start, int inputSize, int step) {
    HandleBufferSize(inputSize);
    for(int i = 0; i < inputSize; i++) {
        buffer[i] = std::complex<float>(start[step*i], start[step*i+1]);
    }
}

void ComplexArray::SetBufferReal(float *start, int inputSize) {
    HandleBufferSize(inputSize);
    for(int i = 0; i < inputSize; i++) {
        buffer[i] = std::complex<float>(start[i], 0);
    }
}

void ComplexArray::SetBuffer(std::vector<std::complex<double>> start, int offset, size_t copysize) {
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}

void ComplexArray::SetBuffer(std::vector<std::complex<float>> start, int offset, size_t copysize) {
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}

void ComplexArray::SetBuffer(std::vector<double> start, int offset, size_t copysize){
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}

void ComplexArray::SetBuffer(std::vector<float> start, int offset, size_t copysize){
    HandleBufferSize(copysize);
    for(size_t i = 0; i < copysize; i++) {
        buffer[i] = start[i + offset];
    }
}


int ComplexArray::GetBuffer(float *RequestedBuffer, int bufferSize) {
    if(bufferSize < size * 2) {
        return -1;
    }

    for(int i = 0; i < size; i++) {
        RequestedBuffer[2*i] = buffer[i].real();
        RequestedBuffer[2*i+1] = buffer[i].imag();
    }
    return 0;
}

int ComplexArray::GetBufferOnlyReal(float *pBuffer, int bufferSize) {
    if(bufferSize < size) {
        return -1;
    }

    for(int i = 0; i < size; i++) {
        pBuffer[i] = buffer[i].real();
    } 
    return size * sizeof(float);
}


int ComplexArray::GetElementSize() {
    return size;
}
int ComplexArray::GetBufferLenghtFloats() {
    return size * 2;
}

int ComplexArray::GetBufferSizeInBytes() {
    return size * 2 * sizeof(float);
}

int ComplexArray::GetMaxPos() {
    auto maxelem = std::max_element(buffer, buffer + size, ComplexArray::ComplexCompare);
    int result = std::distance(buffer, maxelem);
    return result;
}

int ComplexArray::GetMinPos() {
    auto minelem = std::min_element(buffer, buffer + size, ComplexArray::ComplexCompare);
    int result = std::distance(buffer, minelem);
    return result;
}

float ComplexArray::GetMaxValAbs() {
    return std::abs(buffer[GetMaxPos()]);
}

float ComplexArray::GetMinValAbs() {
    return std::abs(buffer[GetMinPos()]);
}

void ComplexArray::SwapHalfs() {
    std::copy(buffer + size / 2, buffer + size, ptemp);
    std::copy(buffer, buffer + size / 2, buffer + size / 2);
    std::copy(ptemp, ptemp + size/2, buffer);
}

ComplexArray ComplexArray::GetSorted() {
    ComplexArray result(*this);
    result.Sort();
    return result;
}

void ComplexArray::Sort() {
    std::sort(buffer, buffer + size, ComplexArray::ComplexCompare);
}

std::vector<std::pair<std::complex<float>, int>> ComplexArray::SortWithIndexes() {
    std::vector<std::pair<std::complex<float>, int>> result;
    for(int i = 0; i < size; i++) {
        result.push_back(std::pair<std::complex<float>, int>(buffer[i], i));
    }
    std::sort(result.begin(), result.end(), ComplexArray::ComplexComprarePairs);
    return result;
}

void ComplexArray::PrintContentsForPython() {
    printf("\na = [");
    for(int i = 0; i < size; i++) {
        printf("%f+%fj,", buffer[i].real(), buffer[i].imag());
    }
    printf("]\n");
}

void ComplexArray::SetAllImaginaryTo(float imag) {
    for(int i = 0; i < size; i++) {
        buffer[i] = std::complex<float>(buffer[i].real(), imag);
    }
}

void ComplexArray::Normalize() {
    float max, min;
    Normalize(min, max);
}

void ComplexArray::Normalize(float &minAbs, float &maxAbs) {
    maxValBeforeNormalize = buffer[GetMaxPos()];
    minValBeforeNormalize = buffer[GetMinPos()];
    avarageBeforNormalize = std::complex<float>(0,0);
    maxAbs = std::abs(maxValBeforeNormalize);
    minAbs = std::abs(minValBeforeNormalize);
    for(int i = 0; i < size; i++) {
        avarageBeforNormalize += buffer[i] / ((float)size);
        buffer[i] = buffer[i] / std::abs(maxValBeforeNormalize - minValBeforeNormalize) * 2.0f;
    }
}


void ComplexArray::ScaleWith(float scalar) {
    for(int i = 0; i < size; i++) {
        buffer[i] = buffer[i] * scalar;
    }
}

void ComplexArray::GetNonZeroIndexes(std::vector<std::pair<int,int>> &indices, int slack) {
    float absval;
    bool isNonZeroStart = false;
    int indexStart = 0, zeroCounter = 0, oneCounter = 0;


    float treshold = GetMaxValAbs() * 0.5;
    // float treshold = 0.65;

    for(int i = 0; i < size; i++) {
        absval = abs(buffer[i]);
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


void ComplexArray::HandleBufferSize(int newSize) {
    size = newSize;
    if(newSize != this->size) {
        size = newSize;
    }
    if(newSize <= currentBuffersElementSize) 
        return;
    int newBufferElementSize = (int) newSize * 1.5;
    delete ptemp;
    delete buffer;
    buffer = new std::complex<float>[newBufferElementSize];
    ptemp = new std::complex<float>[newBufferElementSize];
    currentBuffersElementSize = newBufferElementSize;

}

int ComplexArray::SetSize(int newSize) {
    if(newSize < currentBuffersElementSize) {

    } else {
        HandleBufferSize(newSize);
    }
    if(newSize != this->size) {
        size = newSize;
    }
    return 0;
}

// int ComplexArray::SetSize(int newSize) {
//     if(newSize < currentBuffersElementSize) {
//         if(newSize != this->size) {
//             size = newSize;
//         }
//     } else {
//         HandleBufferSize(newSize);
//     }
//     return 0;
// }

float ComplexArray::GetMeanAbs() {
    float result = 0;
    for(int i = 0; i < size; i++) {
        result += abs(buffer[i]) / (float) size;
    }

    return result;
}

std::complex<float> ComplexArray::GetMean() {
    std::complex<float> result = 0;
    for(int i = 0; i < size; i++) {
        result += (buffer[i]) / (float) size;
    }

    return result;
}

void ComplexArray::PartialCopyTwoArrays(ComplexArray& in1, int offset1, int size1, ComplexArray& in2, int offset2, int size2) {
    int resultingSize = size1 + size2;
    SetSize(resultingSize);
    std::copy(in1.buffer+offset1, in1.buffer + offset1 + size1, buffer);
    std::copy(in2.buffer+offset2, in2.buffer + offset2 + size2, buffer+size1);
}

void ComplexArray::CopyFromAnotherArray(ComplexArray& input, int offset, int copySize) {
    int resultingSize = copySize;
    SetSize(resultingSize);
    std::copy(input.buffer+offset, input.buffer + offset + copySize, buffer);
}