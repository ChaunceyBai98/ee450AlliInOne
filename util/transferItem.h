//
// Created by student on 3/28/22.
//

#ifndef A_TRANSFERITEM_H
#define A_TRANSFERITEM_H
#include <string>
#include <vector>
#include "string.h"

using namespace std;

class transferItem {
    int transferNo;
    string sender;
    string receiver;
    int amount;
public:
    transferItem(int transferNo, string sender, string receiver, int amount);
    transferItem(string des);

    int getTransferNo() const;

    string getSender() const;

    string getReceiver() const;

    int getAmount() const;

    string toString() ;

};



#endif //A_TRANSFERITEM_H
