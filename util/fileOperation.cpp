//
// Created by student on 3/29/22.
//

#include <iostream>
#include "fileOperation.h"
vector<transferItem*> fileOperation:: readFile(){
    vector<transferItem*> items;
    ifstream  fin;
    fin.open(path.data(),ios::in);
    char buf[2048] = {0};
    int i = -1;
    int tNO;
    int amount;
    string sender ;
    string receiver;
    while (fin>>buf){
        i++;
        switch (i) {
            case 0:
                tNO = atoi(buf);
                break;
            case 1:
                sender.assign(buf);
                break;
            case 2:
                receiver.assign(buf);
                break;
            case 3:
                amount = atoi(buf);
                items.push_back( new transferItem(tNO, sender, receiver, amount));
                i = -1;
                break;
        }
    }
    fin.close();
    return items;
}
vector<string> fileOperation::readConfig(){
    vector<string> configs;
    ifstream  fin;
    fin.open(path.data(),ios::in);
    char buf[1024] = {0};
    while (fin>>buf){
       configs.emplace_back(buf);
    }
    fin.close();
    return configs;
}
void fileOperation::writeFileAppend(transferItem * item){
    ofstream fout;
    fout.open(path.data(),ios::app);
    fout<<endl<<item->toString();
    fout.close();
}
void fileOperation:: writeFileCreate(vector<transferItem*> allItems){
    ofstream fout;
    fout.open(path.data());
    for (int i = 0; i < allItems.size(); ++i) {
        if(i!=allItems.size()-1){
            fout<<allItems[i]->toString()<<endl;
        } else{
            fout<<allItems[i]->toString();
            fout.flush();
        }
    }
    fout.close();
}

fileOperation::fileOperation(const string &path) : path(path) {}
