//
// Created by student on 3/31/22.
//
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "util/transferItem.h"
#include <vector>
#include "util/fileOperation.h"
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netdb.h>
#include "util/tcpHelper.h"
#include "util/udpHelper.h"
#include <algorithm>
#include <set>
#include <csignal>
#include "util/stasInfo.h"

#define BUFFERSIZE 1024
#define BOTH_USERS_NOT_EX "-1"
#define SENDER_NOT_EX "-2"
#define RECEIVER_NOT_EX "-3"
#define INSUFFIECIENT_BALENCE "-4"
#define USER_NOT_EX "-5"

using namespace std;
vector<transferItem*> check(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen, string serverName);
//return balance; return -1 if userName not exists
int checkAll(int udpSocketfd, char *commandBuffer, char *infoBuffer,string userName, sockaddr_in servA,sockaddr_in servB,sockaddr_in servC, int socklen);
int getSerialNo(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen);
vector<transferItem*> getAllItems(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen, string serverName);
vector<stasInfo*> makeStas(vector<transferItem*> allItems,string whoToStas);
bool compTransferItem(transferItem* a, transferItem* b){
    return a->getTransferNo()<b->getTransferNo();
}
bool compStasInfo(stasInfo* a, stasInfo* b){
    return a->getNumOfTrans() > b->getNumOfTrans();
}
void signalHandler(int signum);
int listenfd;
int pid;
int main()
{
    signal(SIGINT, signalHandler);
    auto tcphelper = new tcpHelper();
    auto udphelper = new udpHelper();
    char commandBuffer[1024];
    char infoBuffer[1024];
    int socklen=sizeof(struct sockaddr_in);
    vector<transferItem*> itemsA;
    vector<transferItem*> itemsB;
    vector<transferItem*> itemsC;

    auto udpSocketfd = udphelper->udpSocket();
    udphelper->udpBind(udpSocketfd,24844);

    hostent* h ;
    if ( (h = gethostbyname(string("127.0.0.1").c_str())) == 0 )   // 指定服务端的ip地址。
    { printf("gethostbyname failed.\n"); close(udpSocketfd); return {};}
    sockaddr_in servA = udphelper->ipPortTransfer(h,21844);
    sockaddr_in servB = udphelper->ipPortTransfer(h,22844);
    sockaddr_in servC = udphelper->ipPortTransfer(h,23844);
    sockaddr_in servs[3] = {servA,servB,servC};

    int tcpPort;
    string clientName;
    cout<<"The main server is up and running."<<endl;
    if(pid = fork() > 0){
        //parent process listen to 25844
        tcpPort = 25844;
        clientName = "A";
    }else{
        //child process listen to 26844
        tcpPort = 26844;
        clientName = "B";
    }
    tcphelper->tcpBind(listenfd = tcphelper->tcpSocket(), tcpPort);

    tcphelper->tcpListen(listenfd);
    int  clientfd;
    struct sockaddr_in clientaddr;

    while (1)
    {
        clientfd=accept(listenfd, (struct sockaddr *)&clientaddr, (socklen_t*)&socklen);

        memset(commandBuffer, 0, BUFFERSIZE);
        recv(clientfd, commandBuffer, BUFFERSIZE, 0);

        //4 kinds of operation + ack
        string ml(commandBuffer);
        istringstream in(ml);
        vector<string> params;
        string t;
        while (in >> t) {
            params.push_back(t);
        }
        if(params[0] == "check"){
            //1st check wallet: check someone
            //                    0     1
            cout << "The main server received input=" << params[1] << " from the client using TCP over port " << tcpPort << "." << endl;
            int balance = checkAll(udpSocketfd, commandBuffer, infoBuffer,params[1], servA,servB,servC, socklen);
            if(balance == -1){
                strcpy(infoBuffer, USER_NOT_EX);
                send(clientfd,infoBuffer,BUFFERSIZE,0);
                cout<<"Unable to proceed with the request as "<<params[1]<<" is not part of the network. "<<endl;
            }else{
                strcpy(infoBuffer, to_string(balance).c_str());
                send(clientfd,infoBuffer,BUFFERSIZE,0);
                cout<<"The main server sent the current balance to client "<<clientName<<"."<<endl;
            }
        }else if(params[0] == "transfer"){
            //2nd transfer: transfer sender receiver amount
            //                 0        1       2       3
            //explanation: serverM has to check if the transition can be executed
            cout << "The main server received from " << params[1] << " to transfer " << params[3] << " coins to " << params[2] << " using TCP over port " << tcpPort << "." << endl;
            int tryAmount = atoi(params[3].c_str());
            char tempBuffer[1024];

            //get balance
            strcpy(tempBuffer,commandBuffer);
            string checkSender = "check " + params[1];
            strcpy(commandBuffer,checkSender.c_str());
            int senderBalence = checkAll(udpSocketfd, commandBuffer, infoBuffer, params[1], servA, servB, servC, socklen);
            string checkReceicer = "check " + params[2];
            strcpy(commandBuffer,checkReceicer.c_str());
            int receiverBalence = checkAll(udpSocketfd, commandBuffer, infoBuffer, params[2], servA, servB, servC, socklen);
            strcpy(commandBuffer,tempBuffer);

            if(senderBalence + receiverBalence == -2){
                //both users not exist
                strcpy(infoBuffer, BOTH_USERS_NOT_EX);
            } else if(senderBalence == -1){
                //sender not exists
                strcpy(infoBuffer, SENDER_NOT_EX);
            } else if(receiverBalence == -1){
                //receiver balance not exists
                strcpy(infoBuffer, RECEIVER_NOT_EX);
            }else if(senderBalence < tryAmount){
                strcpy(infoBuffer, INSUFFIECIENT_BALENCE);
                send(clientfd, infoBuffer, BUFFERSIZE, 0);
                strcpy(infoBuffer, to_string(senderBalence).c_str());
            } else{
                //need transfer number getTotalItemNum
                strcpy(tempBuffer,commandBuffer);

                strcpy(commandBuffer,"getSerialNo");

                //get senderBalence transferNo
                int serialNumA = getSerialNo(udpSocketfd, commandBuffer, infoBuffer, servA, socklen);
                int serialNumB = getSerialNo(udpSocketfd, commandBuffer, infoBuffer, servB, socklen);
                int serialNumC = getSerialNo(udpSocketfd, commandBuffer, infoBuffer, servC, socklen);
                int transferNo = max(serialNumA,max(serialNumB,serialNumC)) + 1;

                //modify command
                string transferNoStr(to_string(transferNo));
                string tempBufferStr(tempBuffer);
                tempBufferStr.append(" ");
                tempBufferStr.append(transferNoStr);
                strcpy(commandBuffer,tempBufferStr.c_str());

                //get a server randomly
                int randomNo = rand()%3;
                string writeToServer;
                if(randomNo == 0 ){writeToServer = "A";}
                else if(randomNo == 1){writeToServer = "B";}
                else if(randomNo == 2){writeToServer =  "C";}

                //make transfer
                sendto(udpSocketfd,commandBuffer,BUFFERSIZE,0,(struct sockaddr *)&servs[randomNo],socklen);
                cout<<"The main server sent a request to server "<<writeToServer<<"."<<endl;
                //receive "done" info from serve#
                recvfrom(udpSocketfd, infoBuffer, BUFFERSIZE, 0, (struct sockaddr *)&servs[randomNo],(socklen_t *)&socklen);
                cout<<"The main server received the feedback from server " <<writeToServer<< " using UDP over port "<<htons(servs[randomNo].sin_port)<<"."<<endl;
                //send new balance to client
                int newBalence = senderBalence - tryAmount;
                strcpy(infoBuffer, to_string(newBalence).c_str());
            }
            send(clientfd, infoBuffer, BUFFERSIZE, 0);
            cout<<"The main server sent the result of the transaction to client "<<clientName<<"."<<endl;
        }else if(params[0] == "txlist"){
            //3rd exlist: exlist
            vector<transferItem*> itemsA = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servA, socklen,"A");
            vector<transferItem*> itemsB = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servB, socklen,"B");
            vector<transferItem*> itemsC = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servC, socklen,"C");
            vector<transferItem*> allItems;
            allItems.insert(allItems.end(),itemsA.begin(),itemsA.end());
            allItems.insert(allItems.end(),itemsB.begin(),itemsB.end());
            allItems.insert(allItems.end(),itemsC.begin(),itemsC.end());
            //produce a file
            sort(allItems.begin(), allItems.end(), compTransferItem);
//            fileOperation *fileOperator = new fileOperation("../alichain.txt");
            auto *fileOperator = new fileOperation("alichain.txt");
            fileOperator->writeFileCreate(allItems);
        }
        else if(params[0] == "stats"){
            //4th stats: stats username
            //              0   1
            //only related item will be received
            vector<transferItem*> itemsA = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servA, socklen,"A");
            vector<transferItem*> itemsB = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servB, socklen,"B");
            vector<transferItem*> itemsC = getAllItems(udpSocketfd, commandBuffer, infoBuffer, servC, socklen,"C");
            vector<transferItem*> allItems;
            allItems.insert(allItems.end(),itemsA.begin(),itemsA.end());
            allItems.insert(allItems.end(),itemsB.begin(),itemsB.end());
            allItems.insert(allItems.end(),itemsC.begin(),itemsC.end());
            if(allItems.empty()){
                strcpy(infoBuffer,USER_NOT_EX);
                send(clientfd, infoBuffer, BUFFERSIZE, 0);
            } else{
                string InfoStr;
                vector<stasInfo*> StatsInfo = makeStas(allItems,params[1]);
                for (const auto &item : StatsInfo){
                    InfoStr = item->toString();
                    replace(InfoStr.begin(),InfoStr.end(),' ','\t');
                    strcpy(infoBuffer, InfoStr.c_str());
                    send(clientfd, infoBuffer, BUFFERSIZE, 0);
                }
            }
        }
        //Operation finished
        strcpy(infoBuffer, "done");
        send(clientfd, infoBuffer, BUFFERSIZE, 0);
        close(clientfd);
    }

    // should close listenfd;
}

vector<stasInfo*> makeStas(vector<transferItem*> allItems,string whoToStas){
    vector<stasInfo*> stasInfos;
    for (const auto &item : allItems){
        bool ifRecorded = false;
        if(item->getReceiver() == whoToStas){
            // the user receive money in this transaction
            for (const auto &info : stasInfos){
                if(info->getUsername() == item->getSender()){
                    // this user is already recorded
                    info->setNumOfTrans(info->getNumOfTrans() + 1);
                    info->setTransAmount(info->getTransAmount() + item->getAmount());
                    ifRecorded = true;
                    break;
                }
            }
            if(!ifRecorded){
                stasInfo* a = new stasInfo(item->getSender(),1,item->getAmount());
                stasInfos.push_back(a);
            }
        } else if(item->getSender() == whoToStas){
            // the user send money in this transaction
            for (const auto &info : stasInfos){
                if(info->getUsername() == item->getReceiver()){
                    // this user is already recorded
                    info->setNumOfTrans(info->getNumOfTrans() + 1);
                    info->setTransAmount(info->getTransAmount() - item->getAmount());
                    ifRecorded = true;
                    break;
                }
            }
            if(!ifRecorded){
                stasInfo* a = new stasInfo(item->getReceiver(),1,-item->getAmount());
                stasInfos.push_back(a);
            }
        }
    }
    sort(stasInfos.begin(),stasInfos.end(), compStasInfo);
    for (int i = 0; i < stasInfos.size(); ++i) {
        stasInfos[i]->setRank(i+1);
    }
    return stasInfos;
}
vector<transferItem*> getAllItems(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen, string serverName){
    vector<transferItem*> items;
    sendto(udpSocketfd,commandBuffer, BUFFERSIZE,0,(struct sockaddr *)&serv,socklen);
    cout<<"The main server sent a request to server "<<serverName<<"."<<endl;
    while (true){
        memset(infoBuffer,0,BUFFERSIZE);
        recvfrom(udpSocketfd, infoBuffer, BUFFERSIZE, 0, (struct sockaddr *)&serv,(socklen_t *)&socklen);
        string a(infoBuffer);
        if(a=="done"){
            cout<<"The main server received transactions from Server "<<serverName <<" using UDP over port "<<htons(serv.sin_port)<<"."<<endl;
            break;
        }
        transferItem* b = new transferItem(a);
        items.push_back(b);
    }
    return items;
}
vector<transferItem*> check(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen, string serverName){
    vector<transferItem*> items;
    sendto(udpSocketfd,commandBuffer, BUFFERSIZE,0,(struct sockaddr *)&serv,socklen);
    cout<<"The main server sent a request to server "<<serverName<<"."<<endl;
    while (true){
        memset(infoBuffer,0,BUFFERSIZE);
        recvfrom(udpSocketfd, infoBuffer, BUFFERSIZE, 0, (struct sockaddr *)&serv,(socklen_t *)&socklen);
        string a(infoBuffer);
        if(a=="done"){
            cout<<"The main server received transactions from Server "<<serverName<<" using UDP over port "<<htons(serv.sin_port)<<endl;
            break;
        }
        transferItem* b = new transferItem(a);
        items.push_back(b);
    }
    return items;
}
int checkAll(int udpSocketfd, char *commandBuffer, char *infoBuffer,string userName, sockaddr_in servA,sockaddr_in servB,sockaddr_in servC, int socklen){
    bool ifContainUser = false;
    int balance = 1000;
    vector<transferItem*> itemsA = check(udpSocketfd, commandBuffer, infoBuffer, servA, socklen,"A");
    vector<transferItem*> itemsB = check(udpSocketfd, commandBuffer, infoBuffer, servB, socklen,"B");
    vector<transferItem*> itemsC = check(udpSocketfd, commandBuffer, infoBuffer, servC, socklen,"C");
    for (const auto &item : itemsA){
        if(item->getSender()==userName){
            balance -= item->getAmount();
            ifContainUser = true;
        }else if(item->getReceiver()==userName){
            balance += item->getAmount();
            ifContainUser = true;
        }
    }
    for (const auto &item : itemsB){
        if(item->getSender()==userName){
            balance -= item->getAmount();
            ifContainUser = true;
        }else if(item->getReceiver()==userName){
            balance += item->getAmount();
            ifContainUser = true;
        }
    }
    for (const auto &item : itemsC){
        if(item->getSender()==userName){
            balance -= item->getAmount();
            ifContainUser = true;
        }else if(item->getReceiver()==userName){
            balance += item->getAmount();
            ifContainUser = true;
        }
    }
    if(!ifContainUser){
        return -1;
    }
    return balance;
}
int getSerialNo(int udpSocketfd, char *commandBuffer, char *infoBuffer, sockaddr_in serv, int socklen){
    sendto(udpSocketfd,commandBuffer,BUFFERSIZE,0,(struct sockaddr *)&serv,socklen);
    memset(infoBuffer,0,BUFFERSIZE);
    recvfrom(udpSocketfd, infoBuffer, BUFFERSIZE, 0, (struct sockaddr *)&serv,(socklen_t *)&socklen);
    int serialNum = atoi(infoBuffer);
    recvfrom(udpSocketfd, infoBuffer, BUFFERSIZE, 0, (struct sockaddr *)&serv,(socklen_t *)&socklen);
//    cout<<infoBuffer<<endl;
    return serialNum;
}
void signalHandler( int signum){
    close(listenfd);
    if(pid==0){
        cout<<"Server stopped."<<endl;
    }
    exit(signum);
}