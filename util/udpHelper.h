//
// Created by student on 3/31/22.
//
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

#ifndef A_UDPHELPER_H
#define A_UDPHELPER_H

using namespace std;
class udpHelper {
public:
    int udpSocket();
    int udpBind(int socketFd, int port);
    sockaddr_in ipPortTransfer(hostent* h, int port);
};


#endif //A_UDPHELPER_H
