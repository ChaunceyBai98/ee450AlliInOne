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

#ifndef A_TCPHELPER_H
#define A_TCPHELPER_H


class tcpHelper {


public:
    int tcpSocket();
    int tcpConnect(int sockfd, char * ip,char * port);
    int tcpBind(int socketfd, int port);
    int tcpListen(int socketfd);
};


#endif //A_TCPHELPER_H
