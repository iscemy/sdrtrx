#include "RadioInterfaces/TestRadioUDP/TestRadioUDP.h"
#include "Utilities/log.h"

TestRadioUDP::TestRadioUDP(const char *remote_ip, uint16_t remote_port, const char *local_ip, uint16_t localport) {
    this->pSock = new SocketWrapper(remote_ip, remote_port, local_ip, localport);
}

int TestRadioUDP::Receive(ComplexArray &data) {
    
    int ret = pSock->Receive(receiveBuffer, sizeof(receiveBuffer), 0);
    
    if(ret < 0) {
        LOG2CONSOLE("err ");
        return -1;
    } 

    if((ret % 4) != 0) {
        LOG2CONSOLE("receive size err ");
    }
    //to not change the buffers size.
    if((ret/((int)sizeof(float))) > data.GetElementSize()) {
        ret = data.GetElementSize() * sizeof(float);
    }
    data.SetBufferReal((float*)receiveBuffer, ret/sizeof(float));
    return ret;
}

int TestRadioUDP::Transmit(ComplexArray &data) {

    int numOfBytes = data.GetBufferOnlyReal((float*)txBuffer, sizeof(txBuffer)/sizeof(float)); 
    return pSock->Send(txBuffer, numOfBytes);;
}