//
// Created by student on 3/31/22.
//

#include "udpHelper.h"
int udpHelper::udpSocket(){
    int sockfd;
    if ( (sockfd = socket(AF_INET,SOCK_DGRAM,0))==-1) { perror("socket"); return -1; }
    return sockfd;
};

int udpHelper::udpBind(int socketFd, int port) {
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    if (bind(socketFd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 )
    { perror("bind"); close(socketFd); return -1; }
}
int udpHelper::getRandomPort(){
    return 1;
}

sockaddr_in udpHelper::ipPortTransfer(hostent* h, int port) {
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    return servaddr;
}
