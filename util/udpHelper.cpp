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
    struct sockaddr_in servaddr;    // 服务端地址信息的数据结构。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
    servaddr.sin_port = htons(port);  // 指定通信端口。
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
    servaddr.sin_port = htons(port); // 指定服务端的通信端口。
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    return servaddr;
}
