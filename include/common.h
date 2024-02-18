#ifndef COMP7402_ASSIGNMENT5_COMMON_H
#define COMP7402_ASSIGNMENT5_COMMON_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <limits>
#include <utility>
#include <vector>
#include <string>
#include <iomanip>
#include <bitset>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdint>


using namespace std;

extern vector<string> PREDEFINED_ROUND_KEYS;

string hexCharToBin(char hexChar);
string hexToBin(const string& hex);
string binToHex(const string& binary);
string XOR_binary(const string& a, const string& b);
string strToHex(const string& input);
string keyHexToBinary(const string& hex, bool padTo64);
string decToBin(int decimal);
int binToDec(string binary);
string strToBin(const string& input);
void appendToFile(const string& filename, const string& hex);
string readFile(const string& filename);
string readPlainText(const string& prompt);
bool isTxt(const string& filename);
string hexToASCII(const string& hexStr);
string removeTrailingZeros(string binStr);
void addPadding(string& hex);
void runDD(const string& originFileName, const string& encryptedFileName);
void printRoundKeys(vector<string> rks);
int hexCharToValue(char hexChar);

struct BMPHeader {
    uint16_t file_type{0x4D42};          // File type always BM which is 0x4D42
    uint32_t file_size{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};               // Reserved, always 0
    uint16_t reserved2{0};               // Reserved, always 0
    uint32_t offset_data{0};             // Start position of pixel data (bytes from the beginning of the file)
};

struct DIBHeader {
    uint32_t size{0};                    // Size of this header (in bytes)
    int32_t width{0};                    // width of bitmap in pixels
    int32_t height{0};                   // width of bitmap in pixels
    uint16_t planes{1};                  // No. of planes for the target device, this is always 1
    uint16_t bit_count{0};               // No. of bits per pixel
    //... other fields not used here
};

void readBMPHeader(ifstream& file, BMPHeader& bmpHeader, DIBHeader& dibHeader);
int calculatePadding(int width, int bitsPerPixel);
void changeBMPRowPadding(const string& inputFileName, const string& outputFileName);

#endif //COMP7402_ASSIGNMENT5_COMMON_H