#ifndef COMP7402_ASSIGNMENT5_USER_H
#define COMP7402_ASSIGNMENT5_USER_H

#include <iostream>
#include <limits>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <sstream>

#define PREDEFINED_ROUND_KEYS {0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, \
                               0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff}

using namespace std;

enum keyFlag {
    PRE_DEFINED,
    USER_DEFINED,
    DEFAULT
};


enum encryptMode {
    ECB_,
    CBC_
};


class User {

protected:
    bool encryptFlag_;
    int mode_;
    string inFile_;
    string outFile_;
    bool ptMode_;
    int keyFlag_;
    int roundNum_;
    string mainKey_;
    vector<string> hexMainKey_;
    vector<unsigned int> roundKeys_;

    void generateRandomKey();

    void setEncryptDecrypt();
    void setEncryptMode();
    void setMainKeyOption();
    void setMainKey();
    void setHexMainKey();
    void setRoundKeyOption();
    void setRoundNum();
    void setUserRoundKeys();
    void setInOutFile();
    void setPredefinedRoundKeys();
    void printRoundKeys();
public:
    void setEnvironment();

    int getMode() const;
    bool isEncryption();
    string getInFile() const;
    string getOutFile() const;
    bool isPlainTextMode() const;
    int getRoundNum() const;
    vector<string> getHexMainKey() const;
    int getKeyFlag() const;
    string getMainKey() const;
    vector<unsigned int> getRoundKeys() const;

//    void setMode(string m) { mode_ = m; }
//    void setInFile(string s) { inFile_ = s; }
//    void setOutFile(string s) { inFile_ = s; }
//    void setPtMode(bool b) { ptMode_ = b; }
//    void setKeyFlag(int k) { keyFlag_ = k; }
//    void setRoundNum(int r) { roundNum_ = r; }
//    void setPKey(string k) { pKey_ = k; }
};





#endif //COMP7402_ASSIGNMENT5_USER_H
