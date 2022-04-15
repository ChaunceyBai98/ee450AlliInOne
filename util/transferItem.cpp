//
// Created by student on 3/28/22.
//

#include "transferItem.h"
#include <string>
#include <sstream>
#include "iostream"

using namespace std;
transferItem::transferItem(int transferNo, string sender, string receiver, int amount) : transferNo(transferNo),
                                                                                       sender(sender),
                                                                                       receiver(receiver),
                                                                                       amount(amount) {}
transferItem::transferItem(string des){
    istringstream in(des);
    vector<string> params;
    string t;
    while (in >> t) {
        params.push_back(t);
    }
    transferNo = atoi( params[0].c_str());
    sender = params[1];
    receiver = params[2];
    amount = atoi( params[3].c_str());
}
int transferItem::getTransferNo() const {
    return transferNo;
}

string transferItem::getSender() const {
    return sender;
}

string transferItem::getReceiver() const {
    return receiver;
}

int transferItem::getAmount() const {
    return amount;
}
string transferItem::toString() {
    return to_string(transferNo)+" "+ sender+" " + receiver+" "+ to_string(amount);
}