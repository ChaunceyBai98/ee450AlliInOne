//
// Created by student on 4/6/22.
//

#ifndef M_STASINFO_H
#define M_STASINFO_H
#include <string>
#include <vector>
#include "string.h"
using namespace std;

class stasInfo {
    int rank;
    string username;
    int numOfTrans;
    int transAmount;
public:
    int getRank() const;

    void setRank(int rank);

    stasInfo(const string &username, int numOfTrans, int transAmount);

    const string &getUsername() const;

    void setUsername(const string &username);


    int getTransAmount() const;

    int getNumOfTrans() const;

    void setNumOfTrans(int numOfTrans);

    void setTransAmount(int transAmount);

    string toString();
};


#endif //M_STASINFO_H
