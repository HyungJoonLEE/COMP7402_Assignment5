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

using namespace std;

extern vector<string> PREDEFINED_ROUND_KEYS;

string hexToBin(const string& hex);
string binToHex(const string& binary);
string XOR_binary(const string& a, const string& b);
string stringToHex(const string& input);
string hexVectorToString(const vector<string> &vec);
string extendHex(const string& hex, size_t desiredLength);
string keyHexToBinary(const string& hex, bool padTo64);

#endif //COMP7402_ASSIGNMENT5_COMMON_H