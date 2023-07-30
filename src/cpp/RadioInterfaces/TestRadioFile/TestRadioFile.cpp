#include "RadioInterfaces/TestRadioFile/TestRadioFile.h"
#include "Utilities/libnpy.hpp"


TestRadioFile::TestRadioFile(std::string path) {
    this->path = path;
    npy::LoadArrayFromNumpy(path.c_str(), shape, fortran_order, DataReadFromFile);
}
int TestRadioFile::Receive(RealArray &data) {
    int readSize;
    int dataLeft = DataReadFromFile.size() - index;

    if(dataLeft <= 0) {
        return 0;
    }

    if(dataLeft > data.GetElementSize()){
        data.SetBuffer(DataReadFromFile, index, data.GetElementSize());
        readSize = data.GetElementSize();
    }
    else{
        data.SetBuffer(DataReadFromFile, index, dataLeft);
        readSize = dataLeft;
    }

    index += readSize;

    return readSize;

}
int TestRadioFile::Transmit(RealArray &data) {
    data = data;
    return 0;
}

TestRadioFile::~TestRadioFile() {

}
