#ifndef COMP7402_ASSIGNMENT5_ECB_H
#define COMP7402_ASSIGNMENT5_ECB_H


#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cctype>
#include "User.h"

using namespace std;

class ECB {
private:
    vector<string> hexData_;
    vector<string> encData_;
    string encHexString_;

public:
    void processEncrypt(User& u);
    void setPlainText(const string& prompt);
    void setInFileData(const string& inFile);
    void setOutFileData(const string& outFile);
    void addPadding();
    string hexToBinary(const string& hex);
    string xorBinaryStrings(const string& a, const string& b);
    void binaryToHex(const string& binary);
    string hexVectorToString(const std::vector<string>& vec);
    void encryptHexData(const vector<string>& vec);

    void processDecrypt();
};


#endif //COMP7402_ASSIGNMENT5_ECB_H
