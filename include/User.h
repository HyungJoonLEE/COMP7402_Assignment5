#ifndef COMP7402_ASSIGNMENT5_USER_H
#define COMP7402_ASSIGNMENT5_USER_H

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <sstream> // For std::istringstream

enum keyFlag {
    PRE_DEFINED,
    USER_DEFINED,
    DEFAULT
};

using namespace std;

class User {
private:
    string _eMode;
    string _inFile;
    string _outFile;
    int _keyFlag;
    int _roundNum;
    vector<unsigned int> _roundKeys;
public:
    void askEncryptType();
    void askInOutFileType();
    void askRoundKeyType();
    void setRoundKeys(bool d);
    void setPredefinedRoundKeys();
    void askUserRoundNum();
};

#endif //COMP7402_ASSIGNMENT5_USER_H
