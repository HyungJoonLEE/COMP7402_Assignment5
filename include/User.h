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

enum EncryptionType {
    DES_,
    AES_
};

#define PREDEFINED_ROUND_KEYS {0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, \
                               0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff}

using namespace std;

class User {

protected:
    int type_;
    string mode_;
    string inFile_;
    string outFile_;
    bool ptMode_;
    int keyFlag_;
    int roundNum_;
    string password_;
    vector<unsigned int> roundKeys_;

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
    int getType() const;
    string getMode() const;
    string getInFile() const;
    string getOutFile() const;
    bool getPtMode() const;
    int getRoundNum() const;
    int getKeyFlag() const;
    string getPassword() const;
    vector<unsigned int> getRoundKeys() const;

};

#endif //COMP7402_ASSIGNMENT5_USER_H
