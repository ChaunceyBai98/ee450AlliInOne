//
// Created by student on 3/31/22.
//

#include "tcpHelper.h"
int tcpHelper::tcpSocket(){
    int sockfd;
    if ( (sockfd = socket(AF_INET,SOCK_STREAM,0))==-1) { perror("socket"); return -1; }
    return sockfd;
};
int tcpHelper::tcpConnect(int sockfd, char * ip,char * port){
    printf("%d\n",sockfd);
    struct hostent* h;
    if ( (h = gethostbyname(ip)) == 0 )   // 指定服务端的ip地址。
    { printf("gethostbyname failed.\n"); close(sockfd); return -1; }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(port)); // 指定服务端的通信端口。
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);
    if (connect(sockfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)  // 向服务端发起连接清求。
    { perror("connect"); close(sockfd); return -1; }
    return 0;
}
int tcpHelper::tcpBind(int socketfd, int port){
    struct sockaddr_in servaddr;    // 服务端地址信息的数据结构。
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;  // 协议族，在socket编程中只能是AF_INET。
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);          // 任意ip地址。
    //servaddr.sin_addr.s_addr = inet_addr("192.168.190.134"); // 指定ip地址。
    servaddr.sin_port = htons(port);  // 指定通信端口。
    if (bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0 )
    { perror("bind"); close(socketfd); return -1; }
}
int tcpHelper::tcpListen(int socketfd){
    if (listen(socketfd,5) != 0 ) { perror("listen"); close(socketfd); return -1; }
}

