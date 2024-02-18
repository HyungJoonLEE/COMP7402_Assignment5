#include "common.h"


using namespace std;

vector<string> PREDEFINED_ROUND_KEYS {"0xdddddddd", "0xeeeeeeee",
                                      "0xaaaaaaaa", "0xdddddddd",
                                      "0xbbbbbbbb", "0xeeeeeeee",
                                      "0xeeeeeeee", "0xffffffff"};



string hexCharToBin(char hexChar) {
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
        default: return "";
    }
}


string hexToBin(const string& hexStr) {
    string binStr;
    for (char hexChar : hexStr) {
        binStr += hexCharToBin(hexChar);
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
    if (isTxt(filename)) {
        ofstream file(filename, ios::out | ios::app);
        if (!file) {
            cerr << "Failed to open file for writing." << endl;
            return;
        }
        file << hex;

        file.close();
    }
    else {
        ofstream file(filename, ios::app | ios::binary);
        if (!file) {
            cerr << "Failed to open file for writing." << endl;
            return;
        }

        for (size_t i = 0; i < hex.length(); i += 2) {
            // Convert each pair of hex digits to a byte
            int value = hexCharToValue(hex[i]) * 16 + hexCharToValue(hex[i + 1]);
            char byte = static_cast<char>(value);
            file.write(&byte, 1);
        }
        file.close();
    }
}


string readPlainText(const string& prompt) {
    string input;

    cout << prompt;
    getline(cin, input);
    return input;
}


string readFile(const string& filename) {
    if (isTxt(filename)) {
        ifstream file(filename);
        string line;
        string data;

        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return "";
        }

        // Read file line by line
        while (getline(file, line)) {
            data += line;
        }

        file.close();
        return data;
    }

    else {
        ifstream file(filename, ios::binary);
        if (!file) {
            cerr << "Cannot open file." << endl;
            return "";
        }

        stringstream hexStream;
        char byte[2];
        while (file.read(byte, 2)) {
//            swap(byte[0], byte[1]);
            for (char i : byte) {
                hexStream << hex << setw(2) << setfill('0') << (0xFF & static_cast<int>(i));
            }
        }

        file.close();
        return hexStream.str();
    }
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


string removeTrailingZeros(string binStr) {
    string result;
    string pattern = "00000000"; // Pattern to remove
    for (size_t i = 0; i < binStr.length(); ) {
        // If we find a sequence of eight '0's, we skip it
        if (i + 8 <= binStr.length() && binStr.substr(i, 8) == pattern) {
            i += 8; // Move past this block of '0's
        } else {
            // Otherwise, copy the current character to the result
            result += binStr[i];
            i++;
        }
    }
    return result;
}


void addPadding(string& hex) {
    int padding = 16 - (int) hex.size() % 16;
    for (int i = 0; i < padding; ++i) {
        hex += "0";
    }
}


void printRoundKeys(vector<string> rks) {
    cout << "< Round Keys >" << endl;
    for (int i = 0; i < rks.size(); i++) {
        cout << hex << "Round Key " << i + 1 << " : " << rks[i]  << endl;
    }
}


void runDD(const string& originFileName, const string& encryptedFileName) {
    string command = "dd if=" + originFileName + " of=" + encryptedFileName + " bs=1 count=54 conv=notrunc";
    int status = system(command.c_str());
    if (status != 0) {
        cerr << "Command failed with status: " << status << endl;
    }
}


void readBMPHeader(ifstream& file, BMPHeader& bmpHeader, DIBHeader& dibHeader) {
    file.read(reinterpret_cast<char*>(&bmpHeader.file_type), sizeof(bmpHeader.file_type));
    file.read(reinterpret_cast<char*>(&bmpHeader.file_size), sizeof(bmpHeader.file_size));
    file.read(reinterpret_cast<char*>(&bmpHeader.reserved1), sizeof(bmpHeader.reserved1));
    file.read(reinterpret_cast<char*>(&bmpHeader.reserved2), sizeof(bmpHeader.reserved2));
    file.read(reinterpret_cast<char*>(&bmpHeader.offset_data), sizeof(bmpHeader.offset_data));

    file.read(reinterpret_cast<char*>(&dibHeader), sizeof(dibHeader));
}

int calculatePadding(int width, int bitsPerPixel) {
    int lineBytes = (width * bitsPerPixel + 7) / 8;
    int padding = (4 - (lineBytes % 4)) % 4;
    return padding;
}


void changeBMPRowPadding(const string& inputFileName, const string& outputFileName) {
    ifstream inputFile(inputFileName, ios::binary);
    ofstream outputFile(outputFileName, ios::binary);

    if (!inputFile.is_open() || !outputFile.is_open()) {
        cerr << "Could not open file(s)." << endl;
        return;
    }

    BMPHeader bmpHeader;
    DIBHeader dibHeader;
    readBMPHeader(inputFile, bmpHeader, dibHeader);

    // Write the read headers to the output file
    outputFile.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(bmpHeader));
    outputFile.write(reinterpret_cast<const char*>(&dibHeader), sizeof(dibHeader));

    // Calculate padding
    int padding = calculatePadding(dibHeader.width, dibHeader.bit_count);
    int newPadding = (4 - ((dibHeader.width * dibHeader.bit_count / 8) % 4)) % 4;
    vector<char> paddingBytes(newPadding, 0x00); // New padding bytes

    // Skip to the pixel data
    inputFile.seekg(bmpHeader.offset_data, ios::beg);
    outputFile.seekp(bmpHeader.offset_data, ios::beg);

    // For each row, read it, skip the original padding, write it, and add new padding
    int rowSize = (dibHeader.width * dibHeader.bit_count + 7) / 8;
    vector<char> row(rowSize);

    for (int y = 0; y < dibHeader.height; ++y) {
        inputFile.read(row.data(), row.size());
        outputFile.write(row.data(), row.size());
        if (newPadding > 0) {
            outputFile.write(paddingBytes.data(), paddingBytes.size());
        }
        inputFile.seekg(padding, ios::cur); // Skip original padding
    }

    inputFile.close();
    outputFile.close();
}


int hexCharToValue(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    if (hexChar >= 'a' && hexChar <= 'f') return 10 + (hexChar - 'a');
    if (hexChar >= 'A' && hexChar <= 'F') return 10 + (hexChar - 'A');
    return 0;
}