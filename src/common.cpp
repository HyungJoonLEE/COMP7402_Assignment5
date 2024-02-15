#include "Common.h"


using namespace std;

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
//    string hexStr;
//    for (size_t i = 0; i < binary.length(); i += 8) {
//        bitset<8> eightBits(binary.substr(i, 8));
//        stringstream ss;
//        ss << hex << setw(2) << setfill('0') << eightBits.to_ulong();
//        hexStr += ss.str();
//    }
//    return hexStr;
    stringstream ss;
    for (size_t i = 0; i < binary.length(); i += 4) {
        bitset<4> bs(binary.substr(i, 4));
        ss << hex << bs.to_ulong();
    }
    return ss.str();
}


string XOR_binary(const string& a, const string& b) {
    string result;
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back((a[i] == b[i]) ? '0' : '1');
    }
    return result;
}


string strToHex(const string& input) {
    stringstream hexStream;
    // Iterate over each character in the string
    for (unsigned char c : input) {
        // Convert the character's ASCII value to hexadecimal
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);
    }
    // Return the hexadecimal string
    return hexStream.str();
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

    // Correct the padding logic
    if (padTo64 && binaryString.length() < 64) {
        string padded(64 - binaryString.length(), '0'); // Calculate correct number of zeros needed
        binaryString = padded + binaryString; // Prepend zeros to make the binary string 64 bits long
    }
    return binaryString;
}


string decToBin(int decimal) {
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


int binToDec(string binary)
{
    int decimal = 0;
    int counter = 0;
    int size = binary.length();
    for(int i = size-1; i >= 0; i--)
    {
        if(binary[i] == '1'){
            decimal += pow(2, counter);
        }
        counter++;
    }
    return decimal;
}


string strToBin(const string& input) {
    return hexToBin(strToHex(input));
}


void appendToFile(const string& filename, const string& hex) {
    ofstream file(filename, ios::out | ios::app);
    if (!file) {
        cerr << "Failed to open file for writing." << endl;
        return;
    }
    file << hex;

    file.close();
}


string readHexDataFromFile(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Failed to open file: " << filename << endl;
        return "";
    }

    stringstream buffer;
    string hexData;

    // Read the file line by line
    while (getline(file, hexData)) {
        // Append each line of hex data to the stringstream
        buffer << hexData;
    }

    // Convert the stringstream to a string and return it
    return buffer.str();
}


bool isTxt(const string& filename) {
    size_t dotPosition = filename.find_last_of('.');

    if (dotPosition != string::npos) {
        string extension = filename.substr(dotPosition + 1);
        return extension == "txt";
    }
    return false;
}


string hexToASCII(const string& hexStr) {
    string asciiStr;
    for (size_t i = 0; i < hexStr.length(); i += 2) {
        string part = hexStr.substr(i, 2); // Get two hexadecimal digits
        char ch = static_cast<char>(stoi(part, nullptr, 16)); // Convert to char
        asciiStr += ch;
    }
    return asciiStr;
}