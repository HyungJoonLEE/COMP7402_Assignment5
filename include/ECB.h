#ifndef COMP7402_ASSIGNMENT5_ECB_H
#define COMP7402_ASSIGNMENT5_ECB_H


#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>
#include "User.h"

using namespace std;

class ECB {
private:
    vector<std::string> hexData_;

public:
    void processEncrypt(User& u);
    static bool isPlainTextMode(size_t len);
    void setPlainText(const string& prompt);
    void encryptHexData();

    void processDecrypt();
};


#endif //COMP7402_ASSIGNMENT5_ECB_H
