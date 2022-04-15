//
// Created by student on 4/6/22.
//

#include "stasInfo.h"

int stasInfo::getRank() const {
    return rank;
}

void stasInfo::setRank(int rank) {
    stasInfo::rank = rank;
}

const string &stasInfo::getUsername() const {
    return username;
}

void stasInfo::setUsername(const string &username) {
    stasInfo::username = username;
}

int stasInfo::getTransAmount() const {
    return transAmount;
}

void stasInfo::setTransAmount(int transAmount) {
    stasInfo::transAmount = transAmount;
}

stasInfo::stasInfo(const string &username, const int numOfTrans, int transAmount) : username(username),
                                                                                        numOfTrans(numOfTrans),
                                                                                        transAmount(transAmount) {}

int stasInfo::getNumOfTrans() const {
    return numOfTrans;
}

void stasInfo::setNumOfTrans(int numOfTrans) {
    stasInfo::numOfTrans = numOfTrans;
}

string stasInfo::toString(){
    return to_string(rank)+" "+ username+" " + to_string(numOfTrans)+" "+ to_string(transAmount);
}
