#include "RadioInterfaces/IRadio.h"
#include "vector"
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
    int Receive(ComplexArray &data);
    int Transmit(ComplexArray &data);

};
