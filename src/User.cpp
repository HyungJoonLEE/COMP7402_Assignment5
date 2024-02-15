#include "User.h"


void User::setEnvironment() {
    setEncryptDecrypt();
    if (encryptFlag_) {
        setMode();
        if (mode_ == ECB_) {
            setMainKeyOption();
            setRoundNum();
            setRoundKeyOption();
        }
        if (mode_ == CBC_) {

        }
        setDataType();
    }
    else { // decrypt mode
        setMode();
        setMainKey();
        setRoundNum();
        setInFile();
        setOutFile();
    }
}


void User::setEncryptDecrypt() {
    int choice;

    while (true) {
        cout << "< Encrypt | Decrypt >\n"
                "Please choose a mode:\n"
                "1. Encryption\n"
                "2. Decryption\n"
                ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input, please enter a number." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                encryptFlag_ = true;
                cout << "========== Encryption selected\n" << endl;
                return;
            case 2:
                encryptFlag_ = false;
                cout << "========== Decryption selected\n" << endl;
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::setMode() {
    int choice;

    while (true) {
        cout << "< Encryption mode >\n"
                "Please choose a mode:\n"
                "1. ECB (Electronic Code Block)\n"
                "2. CBC (Cipher Block Chaining)\n"
                ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input, please enter a number." << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                mode_ = ECB_;
                cout << "========== ECB mode selected\n" << endl;
                return;
            case 2:
                mode_ = CBC_;
                cout << "========== CBC mode selected\n" << endl;
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::setMainKeyOption() {
    int choice;

    while (true) {
        cout << "< Main key generator >\n"
                "Generate main key option:\n"
                "1: Random Main key\n"
                "2: User Main key\n"
                ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                generateRandomKey();
                cout << "========== Random selected\n";
                cout << "========== Generated main key: " << mainKey_
                    << " = 0x" << binToHex(mainKey_) <<"\n"<< endl;
                return;
            case 2:
                cout << "========== User selected\n" << endl;
                setMainKey();
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    int rn;

    while(true) {
        cout << "< Number of rounds >\n"
                "Set number of Round(s)\n"
                ">> ";
        cin >> rn;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (rn <= 0 || rn > 16) {
            cout << "Invalid input\n" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            roundNum_ = rn;
            cout << "========== Round Number : " << roundNum_ << "\n" << endl;
            break;
        }
    }
}


void User::setRoundKeyOption() {
    int choice;
    while(true) {
        cout << "< Setting Round key option >\n"
                "Round key option\n"
                "1. Default (program will generate for you based on your main Key)\n"
                "2. Enter own round keys\n"
                "3. Use predefined round keys\n"
                "   ( 0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, \n"
                "       0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff )\n";
        cout << ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                keyFlag_ = keyFlag::DEFAULT;
                cout << "========== Default selected\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return;
            case 2:
                keyFlag_ = keyFlag::USER_DEFINED;
                cout << "========== Own selected\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setUserRoundKeys();
                return;
            case 3:
                keyFlag_ = keyFlag::PRE_DEFINED;
                roundNum_ = 8;
                cout << "========== Predefined selected \n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                setPredefinedRoundKeys();
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
    printRoundKeys();
}


void User::setUserRoundKeys() {
    string rk;

    for (int i = 0; i < roundNum_; i++) {
        cout << "Round key [" << i << "] :";
        cin >> rk; // Read the key as hexadecimal

        // Input validation
        if(cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore incorrect input
            cout << "Invalid input. Please enter a hexadecimal number" << endl;
            i--;
            continue;
        }
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
    printRoundKeys();
}


void User::printRoundKeys() {
    cout << "< Round Keys >" << endl;
    for (int i = 0; i < roundKeys_.size(); i++) {
        cout << hex << "Round Key " << i + 1 << " : " << roundKeys_[i]
            << " = 0x" << binToHex(roundKeys_[i]) << endl;
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
    int choice;

    while (true) {
        cout << "< Data type setting >\n"
                "Choose input data type:\n"
                "1. Plaintext\n"
                "2. File\n"
                ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            // Correctly ignore characters until newline after failed input
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }

        // Ignore newline left after reading choice
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1:
                ptMode_ = true;
                cout << "========== Plaintext mode selected\n" << endl;
                cout << "Enter output file name:\n";
                cout << ">> ";
                getline(cin, outFile_); // Correctly wait for user input
                cout << "========== Write to: " << outFile_ << "\n" << endl;
                return;
            case 2:
                ptMode_ = false;
                cout << "========== File mode selected\n" << endl;
                // Assuming setInFile and setOutFile are correctly implemented
                setInFile();
                setOutFile();
                return;
            default:
                cout << "Invalid input\n" << endl;
                // No need for cin.ignore here; already done after cin >> choice
        }
    }
}

void User::setInFile() {
    cout << "Enter input file name: " << endl;
    cout << ">> ";
    getline(cin, inFile_);
    cout << "==========  Read from: " << inFile_ << "\n" << endl;

}


void User::setOutFile() {
    cout << "Enter output file name: " << endl;
    cout << ">> ";
    getline(cin, outFile_);
    cout << "========== Write to: " << outFile_ << "\n" << endl;
}


bool User::isEncryption() { return encryptFlag_; }
int User::getMode() const { return mode_; }
string User::getInFile() const { return inFile_; }
string User::getOutFile() const { return outFile_; }
bool User::isPlainTextMode() const { return ptMode_; }
string User::getMainKey() const { return mainKey_; }

int User::getRoundNum() const { return roundNum_; }
int User::getKeyFlag() const { return keyFlag_; }
vector<string> User::getRoundKeys() { return roundKeys_; }



