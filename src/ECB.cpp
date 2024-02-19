#include "ECB.h"


void ECB::processEncrypt(User& u) {
    string data;
    string hexdata;
    string bindata;

    data.reserve(100000);
    hexdata.reserve(200000);
    bindata.reserve(8000000);

    if (u.isPlainTextMode()) {
        data = readPlainText("Enter plain text: ");
        hexdata = strToHex(data);
    }
    else {
        if (isTxt(u.getInFile())) {
            data = readFile(u.getInFile());
            hexdata = strToHex(data);
        }
        else {
            hexdata = readFile(u.getInFile());
        }
    }

    if (u.getKeyFlag() == PRE_DEFINED)
        generateRoundKeys(u.getMainKey(), u.getRoundNum(), true);
    else
        generateRoundKeys(u.getMainKey(), u.getRoundNum(), false);

    int padding = addPadding(hexdata);
//    cout << "hex: " <<  hexdata << endl;
    bindata = hexToBin(hexdata);

    for (int i = 0; i < bindata.length(); i += 64) {
        string bin = bindata.substr(i, 64);
        string cipherBin = Feistel(u.getRoundNum(), bin, roundKeys_);
        if (i + 64 >= bindata.length() && !isTxt(u.getOutFile())) {
            cutLastPadding(cipherBin, padding * 4);
        }
        appendToFile(u.getOutFile(), binToHex(cipherBin));
        bin.clear();
        cipherBin.clear();
    }

    if (!u.getInFile().empty() && !isTxt(u.getInFile())) {
        runDD(u.getInFile(), u.getOutFile());
    }
}


void ECB::processDecrypt(User &u) {
    string bindata;
    string hexdata;
    int padding;

    hexdata.reserve(200000);
    bindata.reserve(800000);

    if (u.getKeyFlag() == PRE_DEFINED)
        generateRoundKeys(u.getMainKey(), u.getRoundNum(), true);
    else
        generateRoundKeys(u.getMainKey(), u.getRoundNum(), false);

    generateReverseRoundKeys(roundKeys_, u.getRoundNum());

    hexdata = readFile(u.getInFile());

    if (!(isTxt(u.getInFile()))) {
        padding = addPadding(hexdata);
    }

    bindata = hexToBin(hexdata);

    for (int i = 0; i < bindata.length(); i += 64) {
        string bin = bindata.substr(i, 64);
        string decryptBin = Feistel(u.getRoundNum(), bin, reverseRoundKeys_);

        if (bin.size() <= 64 && isTxt(u.getInFile())) {
            decryptBin =  removeTrailingZeros(decryptBin);
        }
        if (i + 64 >= bindata.length() && !isTxt(u.getInFile())) {
            cutLastPadding(decryptBin, padding * 4);
        }
        if (isTxt(u.getInFile())) {
            appendToFile(u.getOutFile(), hexToASCII(binToHex(decryptBin)));
        }
        else {
            appendToFile(u.getOutFile(), binToHex(decryptBin));
        }
    }

    if (!isTxt(u.getInFile())) {
        runDD(u.getInFile(), u.getOutFile());
    }
}


void ECB::generateRoundKeys(string key, int round, bool flag) {
    int count = 0;
    if (flag) {
        roundKeys_.emplace_back("000000000000000011011101110111011101110111011101");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000010101010101010101010101010101010");
        roundKeys_.emplace_back("000000000000000011011101110111011101110111011101");
        roundKeys_.emplace_back("000000000000000010111011101110111011101110111011");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000011111111111111111111111111111111");
    }
    else {
        // 1. Compressing the key using the PC1 table
        string perm_key;
        for (int i : pc1) {
            perm_key += key[i - 1];
        }
        // 2. Dividing the key into two equal halves
        string left  = perm_key.substr(0, 28);
        string right = perm_key.substr(28, 28);
        for(int i = 0; i < 16; i++){
            // 3.1. For rounds 1, 2, 9, 16 the key_chunks are shifted by one.
            if(i == 0 || i == 1 || i == 8 || i == 15 ){
                left = shift_left_once(left);
                right = shift_left_once(right);
            }
                // 3.2. For other rounds, the key_chunks are shifted by two
            else {
                left = shift_left_twice(left);
                right = shift_left_twice(right);
            }

            // Combining the two chunks
            string combined_key = left + right;
            string round_key;
            // Using the PC2 table to transpose the key bits
            for (int j : pc2) {
                round_key += combined_key[j - 1];
            }
            roundKeys_.push_back(round_key);
            count++;
            if (count == round) break;
        }
    }
}


string ECB::shift_left_once(string key_chunk){
    string shifted;

    for(int i = 1; i < 28; i++){
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];

    return shifted;
}


string ECB::shift_left_twice(string key_chunk) {
    string shifted;

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
    // Initial permutation
    string perm;
    string txt;
    for (int i : initial_permutation){
        perm += bin[i - 1];
    }

    // Dividing the result into two equal halves
    string binL = perm.substr(0, 32);
    string binR = perm.substr(32, 32);

    // The plain text is encrypted # of round times
    for (int i = 0; i < round; i++) {
        string right_expand;
        // 3.1. The right half of the plain text is expanded
        for (int j : expansion_table) {
            right_expand += binR[j - 1];
        }
        // 3.3. The result is xored with a key
        string xored = XOR_binary(rk[i], right_expand);
        string res;
        // 3.4. The result is divided into 8 equal parts and passed
        // through 8 substitution boxes. After passing through a
        // substitution box, each box is reduces from 6 to 4 bits.
        for (int j = 0; j < 8; j++) {
            // Finding row and column indices to look up the substitution box
            string row1= xored.substr(j * 6,1) + xored.substr(j * 6 + 5,1);
            int row = binToDec(row1);
            string col1 = xored.substr(j * 6 + 1,1) + xored.substr(j * 6 + 2,1)
                          + xored.substr(j * 6 + 3,1) + xored.substr(j * 6 + 4,1);
            int col = binToDec(col1);
            int val = substition_boxes[j][row][col];
            res += decToBin(val);
        }
        // Another permutation2 is applied
        string perm2;
        for (int j : permutation_tab) {
            perm2 += res[j - 1];
        }
        // XOR with the left half
        xored = XOR_binary(perm2, binL);

        // The left and the right swapped
        binL = xored;
        if (i < round - 1) {
            string temp = binR;
            binR = xored;
            binL = temp;
        }
//        cout << "Round " << i + 1 << ": "
//             << "L" << i + 1 << ": " << binToHex(binL) << "  "
//             << "R" << i + 1 << ": " << binToHex(binR) << "  "
//             << "Round Key: " << binToHex(rk[i]) << endl;
    }

    // The halves of the plain text are applied
    string combined = binL + binR;
    string ciphertext;

    // Inverse of the initial permutation is applied
    for (int i : inverse_permutation){
        ciphertext += combined[i - 1];
    }

    return ciphertext;
}


// TODO: DECRYPTION FIELD
void ECB::generateReverseRoundKeys(const vector<string> &rkv, int round) {
    int i = round - 1;
    while(i >= 0) {
        reverseRoundKeys_.push_back(rkv[i]);
        i--;
    }
}


