//
// Created by student on 3/29/22.
//

#ifndef A_FILEOPERATION_H
#define A_FILEOPERATION_H


#include "transferItem.h"
#include <vector>
#include <fstream>
#include <stdlib.h>
class fileOperation {
private: string path;
public:
    fileOperation(const string &path);

    vector<transferItem*> readFile();
    vector<string> readConfig();
    void writeFileAppend(transferItem * item);
    void writeFileCreate(vector<transferItem*> allItems);
};


#endif //A_FILEOPERATION_H
