#include "User.h"

void User::setEnvironment() {
    setEncryptDecrypt();
    if (encryptFlag_) {
        setEncryptMode();
        setMainKeyOption();
        setHexMainKey();
        if (mode_ == ECB_) {
            setRoundNum();
            setRoundKeyOption();
        }
        setInOutFile();
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


void User::setEncryptMode() {
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

        switch (choice) {
            case 1:
                generateRandomKey();
                cout << "========== Random selected\n";
                cout << "========== Generated main key: " << mainKey_ << "\n"<< endl;
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

        cin.ignore();
        getline(cin, mainKey_);
        if (mainKey_.length() != static_cast<size_t>(8)) {
            cin.clear();
            cout << "Invalid Main key. Please enter a key of length : 8\n"<< endl;
        }
        else {
            cout << "========== Main key: " << mainKey_ << "\n" << endl;
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

        if (rn <= 0 || rn > 16) {
            cout << "Invalid input\n" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        else {
            cout << "========== Round Number : " << roundNum_ << "selected\n" << endl;
            roundNum_ = rn;
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
                return;
            case 2:
                keyFlag_ = keyFlag::USER_DEFINED;
                cout << "========== Own selected\n" << endl;
                setUserRoundKeys();
                return;
            case 3:
                keyFlag_ = keyFlag::PRE_DEFINED;
                roundNum_ = 8;
                cout << "========== Predefined selected \n" << endl;
                setPredefinedRoundKeys();
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::setPredefinedRoundKeys() {
    unsigned int tempKeys[] = PREDEFINED_ROUND_KEYS;
    roundKeys_.clear();
    roundKeys_.insert(roundKeys_.end(), begin(tempKeys), end(tempKeys));
    printRoundKeys();
}


void User::setUserRoundKeys() {
    unsigned int rk;

    for (int i = 0; i < roundNum_; i++) {
        cout << "Round key [" << i << "] :";
        cin >> hex >> rk; // Read the key as hexadecimal

        // Input validation
        if(cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore incorrect input
            cout << "Invalid input. Please enter a hexadecimal number" << endl;
            i--;
            continue;
        }
        roundKeys_.push_back(rk); // Add the key to the vector
    }
    printRoundKeys();
}


void User::printRoundKeys() {
    for (int i = 0; i < roundKeys_.size(); i++) {
        cout << hex << "Round Key " << i << " : 0x" << roundKeys_[i] << endl;
    }
}





void User::generateRandomKey() {
    string characters = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < 8; ++i) {
        mainKey_ += characters[rand() % characters.length()];
    }
}


void User::setInOutFile() {
    int choice;

    while(true) {
        cout << "< Input / Output file setting >\n"
                "Choose input data type:\n"
                "1. Plaintext\n"
                "2. File\n"
                ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }

        switch (choice) {
            case 1:
                ptMode_ = true;
                cout << "========== Plaintext mode selected\n" << endl;
                cout << "Enter output file name:\n";
                cout << ">> ";
                cin.ignore();
                getline(cin, outFile_);
                cout << "========== Write to: " << outFile_ << "\n" << endl;
                return;
            case 2:
                ptMode_ = false;
                cout << "========== File mode selected\n" << endl;
                cout << "Enter input file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, inFile_);
                cout << "==========  Read from: " << inFile_ << "\n" << endl;

                cout << "Enter output file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, outFile_);
                cout << "========== Write to: " << outFile_ << "\n" << endl;
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::setHexMainKey() {
    stringstream hexStream;
    for (unsigned char c : mainKey_) {
        hexStream.str("");
        hexStream.clear();

        // Convert character to hexadecimal
        hexStream << hex << setw(2) << setfill('0') << static_cast<int>(c);

        hexMainKey_.push_back(hexStream.str());
    }
}


bool User::isEncryption() { return encryptFlag_; }


//int User::getType() const { return type_; }
int User::getMode() const { return mode_; }
string User::getInFile() const { return inFile_; }
string User::getOutFile() const { return outFile_; }
bool User::isPlainTextMode() const { return ptMode_; }
string User::getMainKey() const { return mainKey_; }

vector<string> User::getHexMainKey() const { return hexMainKey_; }

//int User::getRoundNum() const { return roundNum_; }
int User::getKeyFlag() const { return keyFlag_; }
//vector<unsigned int> User::getRoundKeys() const { return roundKeys_; }


