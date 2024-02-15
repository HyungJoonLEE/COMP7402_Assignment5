#include "ECB.h"


void ECB::processEncrypt(User& u) {
    hexData_.reserve(10000);
    encData_.reserve(10000);

    if (u.isPlainTextMode()) {
        setPlainText("Enter plain text: ");
    }
    else    // In/out FILE mode
    {
        setInFileData(u.getInFile());
    }
    addPadding();
    if (u.getKeyFlag() == DEFAULT) generate_keys(u.getMainKey(), u.getRoundNum());
    else roundKeys_ = u.getRoundKeys();

    feistel(u.getRoundNum());

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

        hexData_ += hexStream.str();
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


void ECB::addPadding() {
    int padding = 8 - (int)hexData_.size() % 8;
    if (padding != 8) {
        for (int i = 0; i < padding; ++i) {
            hexData_ += "00";
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

string ECB::feistel(unsigned int round) {
    //1. Applying the initial permutation
    string perm = "";
    string txt = "";
    for (int i = 0; i < 64; i++){
        perm += hexData_[initial_permutation[i] - 1];
    }
    // 2. Dividing the result into two equal halves
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    // The plain text is encrypted 16 times
    for (int i = 0; i < round; i++) {
        string right_expanded = "";
        // 3.1. The right half of the plain text is expanded
        for (int j = 0; j < 48; j++) {
            right_expanded += right[expansion_table[j]-1];
        };  // 3.3. The result is xored with a key
        string xored = XOR_binary(roundKeys_[i], right_expanded);
        string res = "";
        // 3.4. The result is divided into 8 equal parts and passed
        // through 8 substitution boxes. After passing through a
        // substituion box, each box is reduces from 6 to 4 bits.
        for (int j = 0; j < 8; j++) {
            // Finding row and column indices to lookup the
            // substituition box
            string row1 = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
            int row = binToDec(row1);
            string col1 = xored.substr(j * 6 + 1,1) + xored.substr(j * 6 + 2,1)
                          + xored.substr(j * 6 + 3,1) + xored.substr(j * 6 + 4,1);;
            int col = binToDec(col1);
            int val = substition_boxes[j][row][col];
            res += decToBin(val);
        }
        // 3.5. Another permutation is applied
        string perm2 ="";
        for (int i = 0; i < 32; i++) {
            perm2 += res[permutation_tab[i]-1];
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
//        txt = "";
    }
    // 4. The halves of the plain text are applied
    string combined_text = left + right;
    string ciphertext = "";
    // The inverse of the initial permuttaion is applied
    for (int i = 0; i < 64; i++){
        ciphertext += combined_text[inverse_permutation[i] - 1];
    }

    //And we finally get the cipher text
    return ciphertext;
}
