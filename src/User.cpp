#include "User.h"

void User::processInput() {
    int choice;

    while (true) {
        cout << "< Encrypt type >\n"
                "Please choose a encrypt type:\n"
                "1. DES (Data Encryption Standard)\n"
                "2. AES (Advanced Encryption Standard)\n"
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
                cout << "[ Chose DES mode ]\n" << endl;
                type_ = DES_;
                askEncryptMode();
                askKeyGenOption();
                askRoundNum();
                askInOutFile();
                askRoundKeyType();

                return;
            case 2:
                cout << "[ Chose AES mode ]\n" << endl;
                type_ = AES_;
                askEncryptMode();
                askKeyGenOption();
                askRoundNum();
                askInOutFile();
                askRoundKeyType();
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

    cout << "< Encrypt Mode Selection >" << endl;
    if (this->getType() == DES_) {
        cout << "[ DES use 8-round Feistel Cipher ]\n" << endl;
    } else {
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
}


void User::askKeyGenOption() {
    int choice;

    cout << "< Key generating option >" << endl;

    switch(type_) {
        case DES_:
            cout << "DES use 64-bit key (8 Bytes)\n" << endl;
            pKeyLen_ = 8;
            break;
        case AES_:
            askKeyLength();
            break;
        default:
            cout << "askKeyGenOption() : Couldn't get here" << endl;
    }

    cout << "Type key option:\n"
            "1: Random key to use for generatedKey()\n"
            "2: User key to use for generatedKey()\n"
            ">> ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "[ Chose random for generatedKey() ]\n" << endl;
            generateRandomKey(pKeyLen_);
            cout << "[ Random generated key: " << pKey_ << " ]\n" << endl;
            break;
        case 2:
            cout << "[ Chose personal for generatedKey() ]\n" << endl;
            askKey(pKeyLen_);
            cout << "[ Personal generated key: " << pKey_ << " ]\n" << endl;
            break;
        default:
            cout << "Invalid input\n" << endl;
    }
}


void User::askKeyLength() {
    int keyLength;
    cout << "Choose the length of the key for (key, iv, generateKey ...etc):\n"
            "(This must be a multiple of 8)\n"
            ">> ";
    cin >> keyLength;

    // Ensure the password length is a multiple of 8
    while (keyLength % 8 != 0) {
        cout << "\n[Invalid length. Please enter a length that is a multiple of 8: " << endl;
        cout << ">> ";
        cin >> keyLength;
    }
    pKeyLen_ = keyLength;
    cout << "[ Chose the length = " << pKeyLen_ << " ]\n" << endl;
}


void User::askRoundNum() {
    int rn;

    while (true) {
        cout << "< Asking Rounds >\n";
        if (type_ == DES_) {
            cout << "The default setting for this DES will use 8-round Feistel Cipher\n"
                    "(If you like to change rounds, type a number < 17)" << endl;
        }
        cout << "Setting number of Round(s)" << endl;
        cout << ">> ";
        cin >> rn;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input\n" << endl;
            continue;
        }
        else if (rn < 1 || rn > 16) {
            cout << "Invalid input  Please try again (1 <= round <= 16)\n" << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        } else {
            roundNum_ = rn;
            cout << "[ Round Number : " << roundNum_ << " ]\n" << endl;
            break;
        }
    }
}


void User::askInOutFile() {
    int choice;

    while (true) {
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
    while (true) {
        cout << "Setting Round keys" << endl;
        cout << "1. Default" << endl;
        cout << "2. Enter own round keys" << endl;
        cout << "3. Use predefined round keys" << endl;
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
        unsigned int tempKeys[] = PREDEFINED_ROUND_KEYS;
        roundKeys_.clear();
        roundKeys_.insert(roundKeys_.end(), begin(tempKeys), end(tempKeys));
    } else {
        askRoundKeys();
    }
    printRoundKeys();
}


void User::askRoundKeys() {
    unsigned int rk;

    for (int i = 0; i < roundNum_; i++) {
        cout << "Round key [" << i << "] :";
        cin >> hex >> rk; // Read the key as hexadecimal

        // Input validation
        if (cin.fail()) {
            cin.clear(); // Clear error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore incorrect input
            cout << "Invalid input. Please enter a hexadecimal number" << endl;
            i--;
            continue;
        }
        roundKeys_.push_back(rk); // Add the key to the vector
    }
}


void User::printRoundKeys() {
    for (int i = 0; i < roundNum_; i++) {
        cout << hex << "Round Key " << i << " : 0x" << roundKeys_[i] << endl;
    }
}


void User::askKey(int pl) {
    while (true) {
        cout << "Type your key for generateKey() or iv (length = " << pl << "): " << endl;
        cout << ">> ";

        cin.ignore();
        getline(cin, pKey_);
        if (pKey_.size() != pl) {
            cin.clear();
            cout << "Invalid password length. Please enter a password of length : " << pl << "\n" << endl;
        } else
            break;
    }
}


void User::generateRandomKey(int length) {
    string characters = "0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < length; ++i) {
        pKey_ += characters[rand() % characters.length()];
    }
}



int User::getType() const { return type_; }

string User::getMode() const { return mode_; }

string User::getInFile() const { return inFile_; }

string User::getOutFile() const { return outFile_; }

bool User::isPlainTextMode() const { return ptMode_; }

int User::getRoundNum() const { return roundNum_; }

int User::getKeyFlag() const { return keyFlag_; }

string User::getPKey() const { return pKey_; }

vector<unsigned int> User::getRoundKeys() const { return roundKeys_; }







