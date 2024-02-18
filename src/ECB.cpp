#include "ECB.h"


void ECB::processEncrypt(User& u) {
    hexData_.reserve(4000);
    binData_.reserve(16000);

    string data;
    string bindata;

    if (u.isPlainTextMode()) {
        data = readPlainText("Enter plain text: ");
        hexData_ = strToHex(data);
        addPadding(false);
    }
    else {
        data = readFile(u.getInFile());
        hexData_ = strToHex(data);
        addPadding(isTxt(u.getInFile()));
    }
    cout << hexData_ << endl;

    if (u.getKeyFlag() == DEFAULT)
        generateRoundKeys(u.getMainKey(), u.getRoundNum());
    else {
        for (const auto &rk: u.getRoundKeys()) {
            roundKeys_.push_back(rk);
        }
    }

    binData_ = hexToBin(hexData_);
    cout << binData_ << endl;
    for (int i = 0; i < binData_.length(); i += 64) {
        string bin = binData_.substr(i, 64);
        string cipherBin = Feistel(u.getRoundNum(), bin, roundKeys_);
        cout << "encrypt: " << binToHex(cipherBin) << endl;
        appendToFile(u.getOutFile(), binToHex(cipherBin));
        bin.clear();
        cipherBin.clear();
    }
}


void ECB::processDecrypt(User &u) {
    hexData_.reserve(10000);
    encData_.reserve(10000);
    binData_.reserve(10000);
    string data;

    if (u.getKeyFlag() == PRE_DEFINED) {
        for (const auto& rk : u.getRoundKeys()) {
            roundKeys_.push_back(rk);
        }
    }
    else {
        generateRoundKeys(u.getMainKey(), u.getRoundNum());
    }
    generateReverseRoundKeys(roundKeys_);
    hexData_ = readFile(u.getInFile());
    cout << "size: " <<  hexData_.size() << endl;
//    encData_ = readHexDataFromFile(u.getInFile());
//
//    binData_ = hexToBin(hexData_);
//    for (int i = 0; i < binData_.length(); i += 64) {
//        string binData = binData_.substr(i, 64);
//        string decryptBin = Feistel(u.getRoundNum(), binData, reverseRoundKeys_);
//        cout << decryptBin << endl;
//        if (i >= binData_.length() - 64) {
//            decryptBin =  removeTrailingZeros(decryptBin, "00000000");
//        }
//        appendToFile(u.getOutFile(), hexToASCII(binToHex(decryptBin)));
//        binData.clear();
//        decryptBin.clear();
//    }
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
        hexData_ += hexStream.str();
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


void ECB::addPadding(bool txt) {
    if (txt) {
        int padding = 8 - (int) hexData_.size() % 8;
        for (int i = 0; i < padding; ++i) {
            hexData_ += '\0';
        }
    }
    else {
        int padding = (16 - (int)hexData_.size() % 16) / 2 ;
        if (padding != 16) {
            for (int i = 0; i < padding; ++i) {
                hexData_ += "00";
            }
        }
    }
}


void ECB::generateRoundKeys(string key, int round) {
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
        cout << "Round key " << i + 1 << " : " <<  roundKeys_[i]
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


string ECB::Feistel(unsigned int round, const string& bin, const vector<string>& rk) {
    // 1. Applying the initial permutation
    string perm;
    string txt;
    for (int i = 0; i < 64; i++) {
        perm += bin[initial_permutation[i] - 1];
    }
    // 2. Dividing the result into two equal halves
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    // The plain text is encrypted 16 times
    for (int i = 0; i < round; i++) {
        string right_expanded;
        // 3.1. The right half of the plain text is expanded
        for (int j = 0; j < 48; j++) {
            right_expanded += right[expansion_table[j] - 1];
        };  // 3.3. The result is XOR with a key
        string xored = XOR_binary(rk[i], right_expanded);
        string res;
        // 3.4. The result is divided into 8 equal parts and passed through 8 substitution boxes.
        // After passing through a substitution box, each box is reduces from 6 to 4 bits.
        for (int j = 0; j < 8; j++) {
            // Finding row and column indices to look up the substitution box
            string row1 = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
            int row = binToDec(row1);
            string col1 = xored.substr(j * 6 + 1, 1) + xored.substr(j * 6 + 2, 1)
                          + xored.substr(j * 6 + 3, 1) + xored.substr(j * 6 + 4, 1);
            int col = binToDec(col1);
            int val = substition_boxes[j][row][col];
            res += decToBin(val);
        }
        // 3.5. Another permutation is applied
        string perm2;
        for (int j : permutation_tab) {
            perm2 += res[j - 1];
        }
        // 3.6. The result is xored with the left half
        xored = XOR_binary(perm2, left);
        // 3.7. The left and the right parts of the plain text are swapped
        left = xored;
        if (i < 15) {
            string temp = right;
            right = xored;
            left = temp;
        }

//        cout << "Round " << i + 1 << ": "
//             << "L" << i + 1 << ": " << binToHex(left) << "  "
//             << "R" << i + 1 << ": " << binToHex(right) << "  "
//             << "Round Key: " << binToHex(roundKeys_[i]) << endl;
//
//        txt += "Round " + to_string(i + 1) + ":  L" + to_string(i + 1) + ": "
//               + binToHex(left) + "  R" + to_string(i + 1) + ": "
//               + binToHex(right) + "  Round Key: " + binToHex(roundKeys_[i]);
//        cout << "left: " << left << "   " << "right: " << right << endl;
//        txt = "";
    }
    // 4. The halves of the plain text are applied
    string combined_text = left + right;
    string ciphertext;
    // The inverse of the initial permutation is applied
    for (int i : inverse_permutation) {
        ciphertext += combined_text[i - 1];
    }

    return ciphertext;
}


// TODO: DECRYPTION FIELD
void ECB::generateReverseRoundKeys(const vector<string> &rkv) {
    int i = rkv.size() - 1;
    while(i >= 0) {
        reverseRoundKeys_.push_back(rkv[i]);
        i--;
    }
}


