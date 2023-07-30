#include "RadioInterfaces/IRadio.h"
#include "vector"
#include "string"

class TestRadioFile : public IRadio {
    private:
    std::string path;
    FILE* fp;
    std::vector<unsigned long> shape {};
    bool fortran_order;
    std::vector<float> DataReadFromFile;
    int index = 0;
    public:
    TestRadioFile(std::string path);
    ~TestRadioFile();
    int Receive(RealArray &data);
    int Transmit(RealArray &data);

};
