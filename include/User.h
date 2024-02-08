#ifndef COMP7402_ASSIGNMENT5_USER_H
#define COMP7402_ASSIGNMENT5_USER_H

#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <sstream>

enum keyFlag {
    PRE_DEFINED,
    USER_DEFINED,
    DEFAULT
};

#define PREDEFINED_ROUND_KEYS {0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, \
                               0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff}

using namespace std;

class User {
private:
    string _type;
    string _mode;
    string _inFile;
    string _outFile;
    bool _keyboard;
    int _keyFlag;
    int _roundNum;
    string _password;
    vector<unsigned int> _roundKeys;

    void generateRandomPassword(int length);
    void askEncryptType();
    void askEncryptMode();
    void askInOutFileType();
    void askRoundKeyType();
    void setRoundKeys(bool d);
    void setPredefinedRoundKeys();
    void askRoundNum();
    void askRoundKeys();
    void printRoundKeys();
    int askPasswordLength();
    void askPersonalPassword(int pl);
    void askPasswordOption(int pl);
public:
    void inputProcess();
};

#endif //COMP7402_ASSIGNMENT5_USER_H
