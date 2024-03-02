#include "User.h"


void User::setEnvironment() {
    setEncryptDecrypt();
    setMode();
    setRoundNum();
    setRoundKeyOption();
    if (mode_ == CBC_) setIV();
    if (keyFlag_ == DEFAULT) setMainKeyOption();
    if (encryptFlag_) setDataType();
    else {
        setInFile();
        setOutFile();
    }
}


void User::setEncryptDecrypt() {
    string n;

    while (true) {
        cout << "< Encrypt | Decrypt >\n"
                "Please choose a mode:\n"
                "1. Encryption\n"
                "2. Decryption\n"
                ">> ";
        getline(cin, n);
        if (n == "1") {
            encryptFlag_ = true;
            cout << "========== Encryption selected\n" << endl;
            break;
        }
        if (n == "2") {
            encryptFlag_ = false;
            cout << "========== Decryption selected\n" << endl;
            break;
        }
        else {
            cout << "Invalid input\n" << endl;
        }
    }
}


void User::setMode() {
    string n;

    while (true) {
        cout << "< Encryption mode >\n"
                "Please choose a mode:\n"
                "1. ECB (Electronic Code Block)\n"
                "2. CBC (Cipher Block Chaining)\n"
                ">> ";
        getline(cin, n);

        if (n == "1") {
            mode_ = ECB_;
            cout << "========== ECB mode selected\n" << endl;
            break;
        }
        if (n == "2") {
            mode_ = CBC_;
            cout << "========== CBC mode selected\n" << endl;
            break;
        }
        else {
            cout << "Invalid input\n" << endl;
        }
    }
}


void User::setMainKeyOption() {
    string n;

    while (true) {
        cout << "< Main key generator >\n"
                "Generate main key option:\n"
                "1: Random Main key\n"
                "2: User Main key\n"
                ">> ";
        getline(cin, n);

        if (n == "1") {
            generateRandomKey();
            cout << "========== Random selected\n";
            cout << "========== Generated main key: " << mainKey_
                 << " = 0x" << binToHex(mainKey_) <<"\n"<< endl;
            break;
        }
        if (n == "2") {
            cout << "========== User selected\n" << endl;
            setMainKey();
            break;
        }
        else {
            cout << "Invalid input\n" << endl;
        }
    }
}


void User::setMainKey() {
    while(true) {
        cout << "< Main Key Setting >\n"
                "Type Main key (length must be 8):\n"
                ">> ";

        getline(cin, mainKey_);
        if (mainKey_.length() != static_cast<size_t>(8)) {
            cout << "Invalid Main key. Please enter a key of length : 8\n"<< endl;
            cin.clear();
        }
        else {
            cout << "========== Generated main key ASCII: " << mainKey_ << "\n" << endl;
            string binKey = strToBin(mainKey_);
            mainKey_ = binKey;
            break;
        }
    }
}


void User::setRoundNum() {
    string n;
    int rn;

    while(true) {
        cout << "< Number of rounds >\n"
                "Set number of Round(s)\n"
                ">> ";
        getline(cin, n);
        rn = stoi(n);
        if (rn <= 0 || rn > 16) {
            cout << "Invalid input\n" << endl;
        }
        else {
            roundNum_ = rn;
            cout << "========== Round Number : " << roundNum_ << "\n" << endl;
            break;
        }
    }
}


void User::setRoundKeyOption() {
    string n;

    while(true) {
        cout << "< Setting Round key option >\n"
                "Round key option\n"
                "1. Default (program will generate for you based on your main Key)\n"
                "2. Enter own round keys" << endl;
        if (roundNum_ == 8) {
            cout << "3. Use predefined round keys\n"
                    "   ( 0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, \n"
                    "       0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff )" << endl;
        }
        cout << ">> ";
        getline(cin, n);

        if (n == "1") {
            keyFlag_ = keyFlag::DEFAULT;
            cout << "========== Default selected\n" << endl;
            break;
        }
        if (n == "2") {
            keyFlag_ = keyFlag::USER_DEFINED;
            cout << "========== Own selected\n" << endl;
            setUserRoundKeys();
            break;
        }
        if (roundNum_ == 8 && n == "3") {
            keyFlag_ = keyFlag::PRE_DEFINED;
            cout << "========== Predefined selected \n" << endl;
            setPredefinedRoundKeys();
            break;
        }
        else {
            cout << "Invalid input\n" << endl;
        }
    }
}


void User::setPredefinedRoundKeys() {
    roundKeys_.clear();
    for (int i = 0; i < 8; i++) {
        string cut0x = PREDEFINED_ROUND_KEYS[i].substr(2);
        string binrk = keyHexToBinary(cut0x, true);
        roundKeys_.push_back(binrk);
    }
}


void User::setUserRoundKeys() {
    string rk;
    cout << "Round key size must be 6" << endl;
    for (int i = 0; i < roundNum_; i++) {
        cout << "Round key [" << i << "] :";
        getline(cin, rk);

        string binrk, hexrk;
        bool padTo64 = rk.find("0x") == 0;
        if (padTo64) {
            hexrk = rk.substr(2); // Remove the "0x" prefix if present.
            binrk = keyHexToBinary(hexrk, padTo64);
        }
        else {
            binrk = strToBin(rk);
        }
        roundKeys_.push_back(binrk); // Add the key to the vector
    }
    cout << endl;
}


void User::generateRandomKey() {
    string characters = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < 8; ++i) {
        mainKey_ += characters[rand() % characters.length()];
    }
    cout << "========== Generated main key ASCII: " << mainKey_ << endl;
    string binKey = strToBin(mainKey_);
    mainKey_ = binKey;
}


void User::setDataType() {
    string n;

    while (true) {
        cout << "< Data type setting >\n"
                "Choose input data type:\n"
                "1. Plaintext\n"
                "2. File\n"
                ">> ";
        getline(cin, n);

        if (n == "1") {
            ptMode_ = true;
            cout << "========== Plaintext mode selected\n" << endl;
            setOutFile();
            break;
        }
        if (n == "2") {
            ptMode_ = false;
            cout << "========== File mode selected\n" << endl;
            setInFile();
            setOutFile();
            break;
        }
        else {
            cout << "Invalid input\n" << endl;
        }
    }
}


void User::setInFile() {
    cout << "Enter input file name: " << endl;
    cout << ">> ";
    getline(cin, inFile_);
    cout << "========== Read from: " << inFile_ << "\n" << endl;
}


void User::setOutFile() {
    cout << "Enter output file name: " << endl;
    cout << ">> ";
    getline(cin, outFile_);
    cout << "========== Write to: " << outFile_ << "\n" << endl;
}


void User::setIV() {
    while(true) {
        cout << "< Initialize Vector Setting >\n"
                "Type Initialize Vector (length must be 8):\n"
                ">> ";

        getline(cin, iv_);
        if (iv_.length() != static_cast<size_t>(8)) {
            cout << "Invalid Main key. Please enter a key of length : 8\n"<< endl;
            cin.clear();
        }
        else {
            cout << "========== Generated initialize vector ASCII: " << iv_ << "\n" << endl;
            string binIv = strToBin(iv_);
            iv_ = binIv;
            break;
        }
    }
}


bool User::isEncryption() const { return encryptFlag_; }
int User::getMode() const { return mode_; }
string User::getInFile() const { return inFile_; }
string User::getOutFile() const { return outFile_; }
bool User::isPlainTextMode() const { return ptMode_; }
string User::getMainKey() const { return mainKey_; }
string User::getIV() const { return iv_; }
int User::getRoundNum() const { return roundNum_; }
int User::getKeyFlag() const { return keyFlag_; }
vector<string> User::getRoundKeys() const { return roundKeys_; }




