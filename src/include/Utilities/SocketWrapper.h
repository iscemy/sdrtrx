//
// Created by cem on 31.12.2022.
//

#ifndef UNTITLED_SOCKETWRAPPER_H
#define UNTITLED_SOCKETWRAPPER_H
#include <cstdint>
#ifndef __WIN32__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#else
#include<winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif

#include <string.h>

class SocketWrapper {
    int socketfd;
    bool SocketReady = false;
    struct sockaddr_in socketops_in, socketops_out;
    bool CurrentlyBlocking = false;
    uint32_t socketTimeOutValMs = 0;

public:
    SocketWrapper(const char *send_ip, uint16_t send_port, const char *receive_ip, uint16_t receive_port) {
        #ifdef __WIN32__
        WSADATA wsaData;
        int iResult;
        u_long iMode = 0;

        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

        #endif

        socketfd = socket(AF_INET, SOCK_DGRAM, 0);

        if(socketfd < 0)
            return;

        memset(&socketops_in, 0, sizeof(socketops_in));

        socketops_in.sin_addr.s_addr = inet_addr(receive_ip);
        socketops_in.sin_family = AF_INET;
        socketops_in.sin_port = htons(receive_port);

        if(bind(socketfd, (sockaddr*) &socketops_in, sizeof(socketops_in)) != 0)
            return;

        memset(&socketops_out, 0, sizeof(socketops_out));
        socketops_out.sin_addr.s_addr = inet_addr(send_ip);
        socketops_out.sin_family = AF_INET;
        socketops_out.sin_port = htons(send_port);


        SocketReady = true;

    }

    inline int Send(void *data, uint32_t length) {
        return sendto(socketfd, (const char *) data, length, 0, (sockaddr *) &socketops_out, sizeof(socketops_out));
    }

    inline int Receive(void *buffer, uint32_t buffer_size, uint32_t timeoutMs) {

        if(timeoutMs != socketTimeOutValMs) {
            struct timeval tov;
            tov.tv_sec = timeoutMs / 1000;
            tov.tv_usec = (timeoutMs % 1000) * 1000;


            #ifdef _WIN32
            DWORD dw = (timeoutMs);
            setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &dw, sizeof(dw));
            #else
            setsockopt(socketfd, SOL_SOCKET, SO_RCVTIMEO, (char *) &tov, sizeof(struct timeval));
            #endif
        }


        return recvfrom(socketfd, (char *) buffer, buffer_size, 0, 0, 0);

    }
};
#endif //UNTITLED_SOCKETWRAPPER_H
