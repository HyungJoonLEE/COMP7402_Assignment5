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
string readFile(const string& filename, bool enc);
string readPlainText(const string& prompt);
bool isTxt(const string& filename);
string hexToASCII(const string& hexStr);
void processHexFile(const string& filePath);
string removeTrailingZeros(string binStr);
void addPadding(bool txt, string& hex);
void printRoundKeys(vector<string> rks);



unsigned char hexCharToValue(char hexChar);
vector<unsigned char> hexStringToBinary(const string& hexString);
string binaryDataToHexString(const vector<unsigned char>& binaryData);

#endif //COMP7402_ASSIGNMENT5_COMMON_H