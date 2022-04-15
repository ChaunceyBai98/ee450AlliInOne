//
// Created by student on 4/4/22.
//
//todo: 打包成文件提交的时候 注意修改serverA,B,C为了调试方面设置的config文件以及相关输入输出,注意修改clientB的cout
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "util/transferItem.h"
#include <vector>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include "util/tcpHelper.h"
#include "util/udpHelper.h"
#define BUFFER_SIZE 1024
#define BOTH_USERS_NOT_EXIST "-1"
#define SENDER_NOT_EXIST "-2"
#define RECEIVER_NOT_EXIST "-3"
#define INSUFFICIENT_BALANCE "-4"
#define USER_NOT_EXIST "-5"
using namespace std;
int main(int argc,char *argv[]) {
    char buffer[1024];
    tcpHelper* tcphelper = new tcpHelper();
    int socketfd = tcphelper->tcpSocket();
    struct hostent* h;
    if ( (h = gethostbyname("127.0.0.1")) == 0 )
    { printf("gethostbyname failed.\n"); close(socketfd); return -1; }
    struct sockaddr_in servaddr;
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(26844); // server listen port
    memcpy(&servaddr.sin_addr,h->h_addr,h->h_length);

    cout<<"The client B is up and running."<<endl;
    //no specific port
    if (connect(socketfd, (struct sockaddr *)&servaddr,sizeof(servaddr)) != 0)
    { perror("connect"); close(socketfd); return -1; }

    if(argc == 2){
        if(strcmp(argv[1],"TXLIST")==0){
            //send TXLIST
            string command("txlist");
            strcpy(buffer,command.c_str());
            send(socketfd,buffer,strlen(buffer),0);
            cout<<"ClientB sent a statistics enquiry request to the main server."<<endl;
            recv(socketfd, buffer, BUFFER_SIZE, 0);
            if(strcmp(buffer,"done")==0){
                cout<<"File produced."<<endl;
            }
        }else {
            //send check argv[1]
            string command("check ");
            command.append(argv[1]);
            strcpy(buffer,command.c_str());
            send(socketfd,buffer,strlen(buffer),0);

            cout<<argv[1]<<" sent a balance enquiry request to the main server."<<endl;
            recv(socketfd, buffer, BUFFER_SIZE, 0);
            cout<<"The current balance of "<<argv[1]<<" is : "<<buffer<<" alicoins."<<endl;

        }
    }
    if(argc == 4){
        //              argv[1] argv[2] argv[3]
        //send transfer sender receiver amount
        //      0       1       2       3
        string command("transfer ");
        command.append(argv[1]);
        command.append(" ");
        command.append(argv[2]);
        command.append(" ");
        command.append(argv[3]);
        strcpy(buffer,command.c_str());
        //1 send transfer sender receiver amount
        send(socketfd,buffer,strlen(buffer),0);
        cout<<argv[1]<<" has requested to transfer " <<argv[3]<<" coins to "<<argv[2]<<"."<<endl;
        recv(socketfd, buffer, BUFFER_SIZE, 0);
        if(strcmp(buffer, BOTH_USERS_NOT_EXIST) == 0){
            cout<<"Unable to proceed with the transaction as "<< argv[1]<<" and "<<argv[2]<<" are not part of the network"<<endl;
        }else if(strcmp(buffer, SENDER_NOT_EXIST) == 0){
            cout<<"Unable to proceed with the transaction as "<<argv[1]<<" is not part of the network."<<endl;
        }else if(strcmp(buffer, RECEIVER_NOT_EXIST) == 0){
            cout<<"Unable to proceed with the transaction as "<<argv[2]<<" is not part of the network."<<endl;
        } else if(strcmp(buffer, INSUFFICIENT_BALANCE) == 0){
            cout<<argv[1]<<" was unable to transfer "<<argv[3]<<" alicoins to "<<argv[2]<<" because of insufficient balance."<<endl;
            recv(socketfd, buffer, BUFFER_SIZE, 0);
            cout<<"The current balance of "<<argv[1]<<" is : "<<buffer<<" alicoins."<<endl;
        } else{
            //working well
            cout<<argv[1]<<" successfully transferred "<<argv[3]<<" alicoins to "<<argv[2]<<"."<<endl;
//            recv(socketfd, buffer, BUFFER_SIZE, 0);
//            cout<<buffer<<endl;
//            //server closed clientFd
//            string command("check ");
//            command.append(argv[1]);
//            strcpy(buffer,command.c_str());
//            send(socketfd,buffer,BUFFER_SIZE,0);
//            memset(buffer,0,BUFFER_SIZE);
//            recv(socketfd, buffer, BUFFER_SIZE, 0);
            cout<<"The current balance of "<<argv[1]<<" is : "<<buffer<<" alicoins."<<endl;
        }
    }
    if(argc == 3){
        //./ClientB <usernameA> stats
        //  0           1         2
        //send stats argv[1]
        string command(argv[2]);
        command.append(" ");
        command.append(argv[1]);
        strcpy(buffer,command.c_str());
        send(socketfd,buffer,strlen(buffer),0);
        cout<<argv[1]<<" sent a statistics enquiry request to the main server."<<endl;
        recv(socketfd, buffer, BUFFER_SIZE, 0);
        if(strcmp(buffer, USER_NOT_EXIST) == 0){
            cout<<"stats user not exist"<<endl;
        }else{
            cout<<argv[1]<<" statistics are the following.:"<<endl<<"Rank--Username--NumofTransacions--Total"<<endl;
            cout<<buffer<<endl;
            while (1){
                recv(socketfd, buffer, BUFFER_SIZE, 0);
                if(strcmp(buffer,"done")==0){
                    break;
                }
                cout<<buffer<<endl;
            }
        }
    }
    close(socketfd);
    return 0;
}

