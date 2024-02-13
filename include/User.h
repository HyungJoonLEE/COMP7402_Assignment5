#ifndef COMP7402_ASSIGNMENT5_USER_H
#define COMP7402_ASSIGNMENT5_USER_H

#include <iostream>
#include <limits>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#define DES_ 1
#define AES_ 2

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
    string pKey_;
    vector<unsigned int> roundKeys_;

    void generateRandomKey(int length);
    void askEncryptType();
    void askEncryptMode();
    void askInOutFileType();
    void askRoundKeyType();
    void setRoundKeys(bool d);
    void setPredefinedRoundKeys();
    void askRoundNum();
    void askRoundKeys();
    void printRoundKeys();
    int askPKeyLength();
    void askPKey(int pl);
    void askPKeyOption(int pl);
public:
    void inputProcess();
    int getType() const;
    string getMode() const;
    string getInFile() const;
    string getOutFile() const;
    bool getPtMode() const;
    int getRoundNum() const;
    int getKeyFlag() const;
    string getPKey() const;
    vector<unsigned int> getRoundKeys() const;

    void setType(int t) { type_ = t; }
    void setMode(string m) { mode_ = m; }
    void setInFile(string s) { inFile_ = s; }
    void setOutFile(string s) { inFile_ = s; }
    void setPtMode(bool b) { ptMode_ = b; }
    void setKeyFlag(int k) { keyFlag_ = k; }
    void setRoundNum(int r) { roundNum_ = r; }
    void setPKey(string k) { pKey_ = k; }
};


enum keyFlag {
    PRE_DEFINED,
    USER_DEFINED,
    DEFAULT
};



#endif //COMP7402_ASSIGNMENT5_USER_H
