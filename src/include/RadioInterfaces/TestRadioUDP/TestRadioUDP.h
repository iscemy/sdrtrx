#include "Utilities/SocketWrapper.h"
#include "RadioInterfaces/IRadio.h"
#include "mutex"

class TestRadioUDP : public IRadio{
public:
    TestRadioUDP(const char *remote_ip, uint16_t remote_port, const char *local_ip, uint16_t localport);
    virtual int Receive(ComplexArray &data);
    virtual int Transmit(ComplexArray &data);
private:
    SocketWrapper *pSock;
    uint8_t receiveBuffer[1024*1024];
    uint8_t txBuffer[1024*1024];
};