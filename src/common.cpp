#include "Common.h"

vector<string> PREDEFINED_ROUND_KEYS {"0xdddddddd", "0xeeeeeeee",
                                      "0xaaaaaaaa", "0xdddddddd",
                                      "0xbbbbbbbb", "0xeeeeeeee",
                                      "0xeeeeeeee", "0xffffffff"};

string hexToBin(const string& hex) {
    string binStr;

    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        for (int j = 7; j >= 0; --j) {
            binStr.push_back((byte & (1 << j)) ? '1' : '0');
        }
    }
    return binStr;
}


string binToHex(const string& binary) {
    string hexStr;
    for (size_t i = 0; i < binary.length(); i += 8) {
        bitset<8> eightBits(binary.substr(i, 8));
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << eightBits.to_ulong();
        hexStr += ss.str();
    }
    return hexStr;
}


string XOR_binary(const string& a, const string& b) {
    string result;
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back((a[i] == b[i]) ? '0' : '1');
    }
    return result;
}


string stringToHex(const string& input) {
    stringstream hexStream;
    // Iterate over each character in the string
    for (unsigned char c : input) {
        // Convert the character's ASCII value to hexadecimal
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);
    }
    // Return the hexadecimal string
    return hexStream.str();
}


string hexVectorToString(const vector<string> &vec) {
    string result;
    for (const auto& str : vec) {
        result += str;
    }
    return result;
}


string keyHexToBinary(const string& hex, bool padTo64) {
    string binaryString;
    for (size_t i = 0; i < hex.length(); ++i) {
        char c = hex[i];
        // Correctly calculate the value for both uppercase and lowercase hex digits
        int value = (c >= '0' && c <= '9') ? (c - '0') : (tolower(c) - 'a' + 10);
        for (int j = 3; j >= 0; j--) {
            binaryString.push_back(((value >> j) & 1) ? '1' : '0');
        }
    }
    cout << binaryString << endl;
    // Correct the padding logic
    if (padTo64 && binaryString.length() < 64) {
        string padded(64 - binaryString.length(), '0'); // Calculate correct number of zeros needed
        binaryString = padded + binaryString; // Prepend zeros to make the binary string 64 bits long
    }
    return binaryString;
}