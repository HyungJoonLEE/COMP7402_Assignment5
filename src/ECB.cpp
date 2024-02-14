#include "ECB.h"


void ECB::processEncrypt(User& u) {
    hexData_.reserve(10000);
    if (u.isPlainTextMode()) {
        setPlainText("Enter plain text: ");
        for (const auto& hexStr : hexData_) {
            cout << hexStr << " ";
        }
//        encryptHexData();
    }
    else {

    }
}



void ECB::setPlainText(const string& prompt) {
    string input;
    stringstream hexStream;

    cout << prompt;
    getline(cin, input);

    for (unsigned char c : input) {
        // Clear the stringstream
        hexStream.str("");
        hexStream.clear();

        // Convert character to hexadecimal
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);

        // Store the hexadecimal string in the vector
        hexData_.push_back(hexStream.str());
    }
}


//void ECB::encryptHexData() {
//    if ()
//}
