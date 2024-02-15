#include "ECB.h"


void ECB::processEncrypt(User& u) {
    hexData_.reserve(10000);
    encData_.reserve(10000);

    string hexMainKey = stringToHex(u.getMainKey());
    string binMainKey = hexToBin(hexMainKey);
    if (u.isPlainTextMode())
        setPlainText("Enter plain text: ");
    else    // In/out FILE mode
        setInFileData(u.getInFile());
    addPadding();
    if (u.getKeyFlag() == DEFAULT)  generate_keys(binMainKey, u.getRoundNum());
    else if (u.getKeyFlag() == PRE_DEFINED) roundKeys_ = u.getRoundKeys();
    else {

    }
//    encryptHexData(u.getHexMainKey());
//    setOutFileData(u.getOutFile());
}


void ECB::setPlainText(const string& prompt) {
    string input;
    stringstream hexStream;

    cout << prompt;
    getline(cin, input);

    for (unsigned char c : input) {
        hexStream.str("");
        hexStream.clear();
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);

        hexData_.push_back(hexStream.str());
    }
}


void ECB::setInFileData(const string &inFile) {
    char byte;

    ifstream file(inFile, ios::binary);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << inFile << endl;
        return;
    }

    while (file.read(&byte, sizeof(byte))) {
        stringstream hexStream;
        hexStream << hex << setw(2) << setfill('0') << (0xFF & static_cast<int>(byte));
        hexData_.push_back(hexStream.str());
    }

    file.close();
}


void ECB::setOutFileData(const string &outFile) {
    ofstream file(outFile);

    if (!file.is_open()) {
        cerr << "Failed to open output file: " << outFile << endl;
        return;
    }

    for (const auto& hexStr : encData_) {
        file << hexStr << " ";
    }

    file.close();
    cout << "Hexadecimal data successfully saved to: " << outFile << endl;
}


void ECB::addPadding() {
    int padding = 8 - (int)hexData_.size() % 8;
    if (padding != 8) {
        for (int i = 0; i < padding; ++i) {
            hexData_.emplace_back("00");
        }
    }
}


void ECB::generate_keys(string key, int round) {
    // 1. Compressing the key using the PC1 table
    string perm_key = "";
    for (int i = 0; i < 56; i++){
        perm_key += key[pc1[i]-1];
    }
    // 2. Dividing the key into two equal halves
    string left = perm_key.substr(0, 28);
    string right = perm_key.substr(28, 28);
    for(int i = 0; i < 16; i++){
        // 3.1. For rounds 1, 2, 9, 16 the key_chunks
        // are shifted by one.
        if(i == 0 || i == 1 || i == 8 || i == 15 ){
            left = shift_left_once(left);
            right = shift_left_once(right);
        }
            // 3.2. For other rounds, the key_chunks
            // are shifted by two
        else{
            left = shift_left_twice(left);
            right = shift_left_twice(right);
        }

        // Combining the two chunks
        string combined_key = left + right;
        string round_key = "";
        // Finally, using the PC2 table to transpose the key bits
        for (int i = 0; i < 48; i++){
            round_key += combined_key[pc2[i]-1];
        }
        roundKeys_.push_back(round_key);
    }

    for (int i = 0; i < round; i++) {
        cout << "Round key " << i << " : " <<  roundKeys_[i]
            << " = 0x" << binToHex(roundKeys_[i]) << endl;
    }
}


string ECB::shift_left_once(string key_chunk){
    string shifted = "";

    for(int i = 1; i < 28; i++){
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];

    return shifted;
}


string ECB::shift_left_twice(string key_chunk) {
    string shifted = "";

    for(int i = 0; i < 2; i++){
        for(int j = 1; j < 28; j++){
            shifted += key_chunk[j];
        }
        shifted += key_chunk[0];
        key_chunk = shifted;
        shifted = "";
    }

    return key_chunk;
}
