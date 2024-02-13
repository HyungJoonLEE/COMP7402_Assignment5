#include "User.h"

void User::askEncryptType() {
    int choice;

    while (true) {
        cout << "Please choose a encrypt type:" << endl;
        cout << "1. DES (Data Encryption Standard)" << endl;
        cout << "2. AES (Advanced Encryption Standard)" << endl;
        cout << ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input, please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "[ Chose DES mode ]\n" << endl;
                type_ = DES_;
                return;
            case 2:
                cout << "[ Chose AES mode ]\n" << endl;
                type_ = AES_;
                return;
            default:
                cout << "Invalid input\n" << endl;
                // Optionally clear the input buffer if expecting further inputs
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::askEncryptMode() {
    int choice;

    while (true) {
        cout << "Please choose a mode:" << endl;
        cout << "1. ECB (Electronic Code Block)" << endl;
        cout << "2. CBC (Cipher Block Chaining)" << endl;
        cout << ">> ";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input, please enter a number." << endl;
            continue;
        }

        switch (choice) {
            case 1:
                cout << "[ Chose ECB mode ]\n" << endl;
                mode_ = "ECB";
                askPKeyOption(askPKeyLength());
                return;
            case 2:
                cout << "[ Chose CBC mode ]\n" << endl;
                mode_ = "CBC";
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

void User::askInOutFileType() {
    int choice;

    while(true) {
        cout << "Please choose input data type: " << endl;
        cout << "1. Plaintext " << endl;
        cout << "2. File " << endl;
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
                ptMode_ = true;
                cout << "[ Chose Plaintext mode ]\n" << endl;
                cout << "Enter output file name: ";
                cin >> outFile_;
                cout << "[ Write to: " << outFile_ << " ]\n" << endl;
                return;
            case 2:
                ptMode_ = false;
                cout << "[ Chose File mode ]\n" << endl;
                cout << "Enter input file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, inFile_);
                cout << "[ Read from: " << inFile_ << " ]\n" << endl;

                cout << "Enter output file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, outFile_);
                cout << "[ Write to: " << outFile_ << " ]\n" << endl;
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::askRoundKeyType() {
    int choice;
    while(true) {
        cout << "Setting Round keys" << endl;
        cout << "1. Default" << endl;
        cout << "2. Enter own round keys" << endl;
        cout << "3. Use predefined round keys"<< endl;
        cout << "   ( 0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, "
                "0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff )" << endl;
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
                cout << "[ Chose Default ]\n" << endl;
                keyFlag_ = keyFlag::DEFAULT;
                return;
            case 2:
                cout << "[ Chose User defined Round keys\n" << endl;
                keyFlag_ = keyFlag::USER_DEFINED;
                setRoundKeys(false);
                return;
            case 3:
                cout << "[ Chose Predefined Round keys ]\n" << endl;
                keyFlag_ = keyFlag::PRE_DEFINED;
                roundNum_ = 8;
                setRoundKeys(true);
                return;
            default:
                cout << "Invalid input\n" << endl;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}


void User::setRoundKeys(bool r) {
    if (r) {
        setPredefinedRoundKeys();
    }
    else {
        askRoundNum();
        askRoundKeys();
    }
}


void User::setPredefinedRoundKeys() {
    unsigned int tempKeys[] = PREDEFINED_ROUND_KEYS;
    roundKeys_.clear();
    roundKeys_.insert(roundKeys_.end(), begin(tempKeys), end(tempKeys));
}


void User::askRoundNum() {
    int rn;

    while(true) {
        cout << "Setting number of Round(s)" << endl;
        cout << ">> ";
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
            cout << "[ Round Number : " << roundNum_ << " ]\n" << endl;
            roundNum_ = rn;
            break;
        }
    }
}


void User::askRoundKeys() {
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


int User::askPKeyLength() {
    int passwordLength;
    cout << "Choose the length of the key for (key, iv, generateKey ...etc): " << endl;
    cout << "(This must be a multiple of 8) " << endl;
    cout << ">> ";
    cin >> passwordLength;

    // Ensure the password length is a multiple of 8
    while (passwordLength % 8 != 0) {
        cout << "\n[Invalid length. Please enter a length that is a multiple of 8: " << endl;
        cout << ">> ";
        cin >> passwordLength;
    }
    cout << "[ Chose password length = "<< passwordLength <<" ]\n" << endl;
    return passwordLength;
}


void User::askPKeyOption(int pl) {
    int choice;
    cout << "Type password option: " << endl;
    cout << "1: Random generated password" << endl;
    cout << "2: Use personal password" << endl;
    cout << ">> ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "[ Chose random generated password ]\n" << endl;
            generateRandomKey(pl);
            cout << "[ Generated password: " << pKey_ << " ]\n"<< endl;
            break;
        case 2:
            cout << "[ Chose personal Password ]\n" << endl;
            askPKey(pl);
            cout << "[ Personal password: " << pKey_ << " ]\n"<< endl;
            break;
        default:
            cout << "Invalid input\n" << endl;
    }
}


void User::askPKey(int pl) {
    while(true) {
        cout << "Type your password (length = " << pl << "): " << endl;
        cout << ">> ";

        cin.ignore();
        getline(cin, pKey_);
        if (pKey_.length() != static_cast<size_t>(pl)) {
            cin.clear();
            cout << "Invalid password length. Please enter a password of length : " << pl << "\n"<< endl;
        }
        else
            break;
    }
}


void User::generateRandomKey(int length) {
    string characters = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < length; ++i) {
        pKey_ += characters[rand() % characters.length()];
    }
}


void User::inputProcess() {
    srand(static_cast<unsigned int>(time(0)));
    askEncryptType();
    askEncryptMode();
    askInOutFileType();
    askRoundKeyType();
}


int User::getType() const { return type_; }
string User::getMode() const { return mode_; }
string User::getInFile() const { return inFile_; }
string User::getOutFile() const { return outFile_; }
bool User::getPtMode() const { return ptMode_; }
int User::getRoundNum() const { return roundNum_; }
int User::getKeyFlag() const { return keyFlag_; }
string User::getPKey() const { return pKey_; }
vector<unsigned int> User::getRoundKeys() const { return roundKeys_; }


