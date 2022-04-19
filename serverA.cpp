#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include "util/transferItem.h"
#include <vector>
#include "util/fileOperation.h"
#include <sstream>
#include "util/udpHelper.h"
#include <csignal>

#define BUFFER_SIZE 2048
using namespace std;
string selfIdentify(int port){
    string server;
    if(port==21844){
        server = "ServerA";
    } else if(port == 22844){
        server = "ServerB";
    } else if(port == 23844){
        server = "ServerC";
    }
    return server;
}
void signalHandler(int signum);

int socketFd;
int main()
{
    signal(SIGINT, signalHandler);
    int port = 21844;
    string dataPath = "./block1.txt";
    auto* fileOperator = new fileOperation(dataPath);

    auto* udphelper = new udpHelper();

    string serverName = selfIdentify(port);
    cout<<"The "<<serverName<<" is up and running using UDP on port "<< port<<"."<<endl;

    socketFd = udphelper->udpSocket();

    udphelper->udpBind(socketFd, port);

    int  socklen=sizeof(struct sockaddr_in);
    struct sockaddr_in clientaddr{};

    char buffer[BUFFER_SIZE];
    while (true)
    {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(socketFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, (socklen_t *)&socklen);
        cout<<"The "<<serverName<<" received a request from the Main Server."<<endl;
        //4 kinds of operation + ack
        string ml(buffer);
        istringstream in(ml);
        vector<string> params;
        string t;
        while (in >> t) {
            params.push_back(t);
        }
        if(params[0] == "check"){
            //1st check wallet: check someone
            vector<transferItem*> items = fileOperator->readFile();
            vector<transferItem*> relatedItems;
            for (auto & item : items) {
                if(item->getSender() == params[1]||item->getReceiver() == params[1]){
                    relatedItems.push_back(item);
                }
            }
            for (const auto &item : relatedItems){
                strcpy(buffer, item->toString().c_str());
                sendto(socketFd, buffer, strlen(buffer), 0, (struct sockaddr *)&clientaddr, socklen);
            }
        } else if(params[0] == "transfer"){
            //2nd transfer: transfer sender receiver amount transferNo
            //explanation: serverM has to check if the transition can be executed
            fileOperator->writeFileAppend(new transferItem(atoi(params[4].c_str()), params[1], params[2], atoi(params[3].c_str())));
        }else if(params[0] == "txlist"){
            //3rd txlist: txlist
            vector<transferItem*> items = fileOperator->readFile();
            for (auto & item : items) {
                strcpy(buffer,item->toString().c_str());
                sendto(socketFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, socklen);
            }
        }else if(params[0] == "stats"){
            //4th stats: stats username
            vector<transferItem*> items = fileOperator->readFile();
            for (auto & item : items) {
                if(item->getSender() == params[1] || item->getReceiver() == params[1]){
                    strcpy(buffer,item->toString().c_str());
                    sendto(socketFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, socklen);
                }
            }
        }else if(params[0] == "getSerialNo"){
            vector<transferItem*> items = fileOperator->readFile();
            int max = 0;
            for (const auto &item : items){
                if(item->getTransferNo()>max){
                    max = item->getTransferNo();
                }
            }
            strcpy(buffer,to_string(max).c_str());
            sendto(socketFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, socklen);
        }
        strcpy(buffer,"done");
        sendto(socketFd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientaddr, socklen);
        cout<<"The "<<serverName<<" finished sending the response to the Main Server."<<endl;
    }
}
void signalHandler( int signum){
    close(socketFd);
    cout<<"Server stopped."<<endl;
    exit(signum);
}