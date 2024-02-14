#include "ECB.h"


void ECB::processEncrypt(User& u) {
    hexData_.reserve(10000);
    encData_.reserve(10000);

    if (u.isPlainTextMode())
        setPlainText("Enter plain text: ");

    else
        setInFileData(u.getInFile());

    addPadding();
    encryptHexData(u.getHexMainKey());
    setOutFileData(u.getOutFile());
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

        hexData_.push_back(hexStream.str());
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
        hexData_.push_back(hexStream.str());
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
            hexData_.emplace_back("00");
        }
    }
}


void ECB::encryptHexData(const vector<string>& KeyVec) {
    string key = hexVectorToString(KeyVec);
    string kl = key.substr(0, key.size() / 2);
    string kr = key.substr(key.size() / 2);

    // Convert keys to binary for XOR operation
    string klb = hexToBinary(kl);
    string krb = hexToBinary(kr);

    // Process each 8-byte (16 hex characters) block from hexData_
    for (size_t i = 0; i < hexData_.size(); i += 8) {
        string block;
        for (size_t j = i; j < i + 8 && j < hexData_.size(); ++j) {
            block += hexData_[j];
        }

        // Divide it into half (ld, rd)
        string lt = block.substr(0, block.size() / 2);
        string rt = block.substr(block.size() / 2);

        // Convert ld and rd to binary
        string ltb = hexToBinary(lt);
        string rtb = hexToBinary(rt);

        // XOR operations
        string left = xorBinaryStrings(ltb, krb);
        string right = xorBinaryStrings(rtb, klb);
        binaryToHex(left + right);
    }
}


string ECB::hexToBinary(const string& hex) {
    string binary;

    for (size_t i = 0; i < hex.length(); i += 2) {
        string byteString = hex.substr(i, 2);
        char byte = static_cast<char>(strtol(byteString.c_str(), nullptr, 16));
        for (int j = 7; j >= 0; --j) {
            binary.push_back((byte & (1 << j)) ? '1' : '0');
        }
    }
    return binary;
}


string ECB::xorBinaryStrings(const string& a, const string& b) {
    string result;
    for (size_t i = 0; i < a.size(); ++i) {
        result.push_back((a[i] == b[i]) ? '0' : '1');
    }
    return result;
}


void ECB::binaryToHex(const string& binary) {
    for (size_t i = 0; i < binary.length(); i += 8) {
        bitset<8> eightBits(binary.substr(i, 8));
        stringstream ss;
        ss << hex << setw(2) << setfill('0') << eightBits.to_ulong();
        encData_.push_back(ss.str());
    }
}


string ECB::hexVectorToString(const vector<string> &vec) {
    string result;
    for (const auto& str : vec) {
        result += str;
    }
    return result;
}
