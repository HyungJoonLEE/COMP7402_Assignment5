#include "DES.h"

string round_keys[16];
string pt;

void DES::handleDES(User &u) {
//    pt.reserve(10000);
//    bpt.reserve(40000);
    if (u.isPlainTextMode()) { // TODO: process plain text
        pt = getPTInput("Type plain text to encrypt: \n>> ");
        pt = hexToBin(pt);
        generate_keys(hexToBin(u.getPKey()));
        cout << encryptECB(u) << endl;
    }
    else {

    } // TODO: process plain text
}


void DES::handleDES_CBC(User &u) {
    if (!u.isPlainTextMode()) { } // TODO: process plain text
    if (!u.isPlainTextMode()) { } // TODO: process plain text
}


bool DES::isValidHex(const string& str) {
    for (char c : str) {
        if (!isxdigit(c)) {
            return false;
        }
    }
    return true;
}


string DES::getPTInput(const string& prompt) {
    string input;
    while (input.empty()) {
        cout << prompt;
        cin >> input;
        if (input.empty()) {
            cout << "Empty input. Please try again." << endl;
        }
    }
    return input;
}


string DES::hexCharToBin(char hexChar) {
    switch(hexChar) {
        case '0': return "0000";
        case '1': return "0001";
        case '2': return "0010";
        case '3': return "0011";
        case '4': return "0100";
        case '5': return "0101";
        case '6': return "0110";
        case '7': return "0111";
        case '8': return "1000";
        case '9': return "1001";
        case 'A': case 'a': return "1010";
        case 'B': case 'b': return "1011";
        case 'C': case 'c': return "1100";
        case 'D': case 'd': return "1101";
        case 'E': case 'e': return "1110";
        case 'F': case 'f': return "1111";
        default: return ""; // invalid input
    }
}


string DES::hexToBin(const string& hexStr) {
    string binStr = "";
    for (char hexChar : hexStr) {
        binStr += hexCharToBin(hexChar);
    }
    return binStr;
}


string DES::binToHex(const string& binary) {
    string hex;
    int len = binary.length();
    int padding = (4 - len % 4) % 4; // Padding to make length a multiple of 4

    string paddedBinary = string(padding, '0') + binary;

    for (int i = 0; i < paddedBinary.length(); i += 4) {
        string fourBits = paddedBinary.substr(i, 4);
        int value = 0;

        if (fourBits[0] == '1') value += 8;
        if (fourBits[1] == '1') value += 4;
        if (fourBits[2] == '1') value += 2;
        if (fourBits[3] == '1') value += 1;

        if (value < 10) {
            hex += (char)('0' + value);
        } else {
            hex += (char)('A' + (value - 10));
        }
    }

    return hex;
}


string DES::decToBin(int decimal) {
    string binary;

    while(decimal != 0) {
        binary = (decimal % 2 == 0 ? "0" : "1") + binary;
        decimal = decimal/2;
    }

    while(binary.length() < 4){
        binary = "0" + binary;
    }

    return binary;
}


int DES::binToDec(string binary) {
    int decimal = 0;
    int counter = 0;
    int size = binary.length();

    for(int i = size-1; i >= 0; i--) {
        if(binary[i] == '1'){
            decimal += pow(2, counter);
        }
        counter++;
    }

    return decimal;
}


string DES::shift_left_once(string key_chunk){
    string shifted = "";

    for(int i = 1; i < 28; i++){
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];

    return shifted;
}


string DES::shift_left_twice(string key_chunk) {
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


string DES::Xor(string a, string b) {
    string result = "";
    int size = b.size();

    for(int i = 0; i < size; i++) {
        if(a[i] != b[i])
            result += "1";
        else
            result += "0";
    }

    return result;
}


void DES::generate_keys(string key) {
    // 1. Compressing the key using the PC1 table
    string perm_key, round_key;
    for (int i : pc1) {
        perm_key += key[i - 1];
    }

    // 2. Dividing the key into two equal halves
    string left = perm_key.substr(0, 28);
    string right = perm_key.substr(28, 28);

    for (int i = 0; i < 16; i++) {
        // Shift once for rounds 1, 2, 9, 16 the key_chunks
        if (i == 0 || i == 1 || i == 8 || i == 15 ) {
            left = shift_left_once(left);
            right = shift_left_once(right);
        }
        // Shift twice for other rounds
        else {
            left = shift_left_twice(left);
            right = shift_left_twice(right);
        }

        // Combining the left and right
        string combined_key = left + right;
        round_key = "";

        // Use PC2 to transpose the key bits
        for (int j : pc2){
            round_key += combined_key[j - 1];
        }

        round_keys[i] = round_key;
    }
}


string DES::encryptECB(User &user){
    // Initial permutation
    string perm, txt;
    for (int i : initial_permutation){
        perm += pt[i - 1];
    }

    // Dividing the result into two equal halves
    string left = perm.substr(0, 32);
    string right = perm.substr(32, 32);

    // The plain text is encrypted 16 times
    for (int i = 0; i < user.getRoundNum(); i++) {
        string right_expanded;

        // The right half of the plain text is expanded
        for (int j : expansion_table) {
            right_expanded += right[j - 1];
        }

        // result XOR key
        string xored = Xor(round_keys[i], right_expanded);
        string res;
        // The result is divided into 8 equal parts and passed
        // through 8 substitution boxes. After passing through a
        // substituion box, each box is reduces from 6 to 4 bits.
        for (int j = 0; j < 8; j++) {
            // Finding row and column indices in the substitution box
            string row1 = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
            int row = binToDec(row1);
            string col1 = xored.substr(j * 6 + 1,1) + xored.substr(j * 6 + 2,1)
                          + xored.substr(j * 6 + 3,1) + xored.substr(j * 6 + 4,1);;
            int col = binToDec(col1);
            int val = substition_boxes[j][row][col];
            res += decToBin(val);
        }

        // Another permutation is applied
        string perm2;
        for (int i : permutation_tab) {
            perm2 += res[i - 1];
        }
        // result XOR left half
        xored = Xor(perm2, left);
        // The left and the right parts of the plain text are swapped
        left = xored;
        if (i < 15) {
            string temp = right;
            right = xored;
            left = temp;
        }
        cout << "Round " << i + 1 << ": "
             << "L" << i + 1 << ": " << binToHex(left) << "  "
             << "R" << i + 1 << ": " << binToHex(right) << "  "
             << "Round Key: " << binToHex(round_keys[i]) << endl;

    }
    // The halves of the plain text are applied
    string combined_text = left + right;
    string ciphertext;
    // The inverse of the initial permutation is applied
    for (int i : inverse_permutation) {
        ciphertext += combined_text[i - 1];
    }
//    appendToFile(user.getOutFile(), ciphertext);
    return ciphertext;
}


void DES::appendToFile(const string& filename, const string& text) {
    // Open the file in append mode. It will be created if it doesn't exist.
    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        cerr << "Failed to open or create the file: " << filename << endl;
        return;
    }

    // Write the text to the file
    file << text << endl;

    // Close the file
    file.close();
}