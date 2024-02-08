#include "User.h"

void User::askEncryptType() {
    int choice;

    while (true) {
        cout << "Please choose a encrypt type:" << endl;
        cout << "1. ECB (Data Encryption Standard)" << endl;
        cout << "2. ECB (Advanced Encryption Standard)" << endl;
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
                _type = "ECB";
                return;
            case 2:
                cout << "[ Chose ECB mode ]\n" << endl;
                _type = "ECB";
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
                _mode = "ECB";
                askPasswordOption(askPasswordLength());
                return;
            case 2:
                cout << "[ Chose CBC mode ]\n" << endl;
                _mode = "CBC";
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
        cout << "1. File " << endl;
        cout << "2. Plaintext " << endl;
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
                _keyboard = false;
                cout << "[ Chose File mode ]\n" << endl;
                cout << "Enter input file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, _inFile);


                cout << "[ Read from: " << _inFile << " ]\n" << endl;
                cout << "Enter output file name: " << endl;
                cout << ">> ";
                cin.ignore();
                getline(cin, _outFile);
                cout << "[ Write to: " << _outFile << " ]\n" << endl;
                return;
            case 2:
                _keyboard = true;
                cout << "[ Chose Plaintext mode ]\n" << endl;
                cout << "Enter output file name: ";
                cin >> _outFile;
                cout << "[ Write to: " << _outFile << " ]\n" << endl;
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
        cout << "1. Use predefined round keys"<< endl;
        cout << "   ( 0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd, "
                "0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff )" << endl;
        cout << "2. Enter own round keys" << endl;
        cout << "3. Default" << endl;
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
                cout << "[ Chose Predefined Round keys ]\n" << endl;
                _keyFlag = keyFlag::PRE_DEFINED;
                _roundNum = 8;
                setRoundKeys(true);
                return;
            case 2:
                cout << "[ Chose User defined Round keys\n" << endl;
                _keyFlag = keyFlag::USER_DEFINED;
                setRoundKeys(false);
                return;
            case 3:
                cout << "[ Chose Default ]\n" << endl;
                _keyFlag = keyFlag::DEFAULT;
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
    _roundKeys.clear();
    _roundKeys.insert(_roundKeys.end(), begin(tempKeys), end(tempKeys));
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
            cout << "[ Round Number : " << _roundNum << " ]\n" << endl;
            _roundNum = rn;
            break;
        }
    }
}


void User::askRoundKeys() {
    unsigned int rk;

    for (int i = 0; i < _roundNum; i++) {
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
        _roundKeys.push_back(rk); // Add the key to the vector
    }
    printRoundKeys();
}


void User::printRoundKeys() {
    for (int i = 0; i < _roundKeys.size(); i++) {
        cout << hex << "Round Key " << i << " : 0x" << _roundKeys[i] << endl;
    }
}


int User::askPasswordLength() {
    int passwordLength;
    cout << "Choose the length of the password (must be a multiple of 8): " << endl;
    cout << ">> ";
    cin >> passwordLength;

    // Ensure the password length is a multiple of 8
    while (passwordLength % 8 != 0) {
        cout << "Invalid length. Please enter a length that is a multiple of 8: ";
        cin >> passwordLength;
    }
    cout << "[ Chose password length = "<< passwordLength <<" ]\n" << endl;
    return passwordLength;
}


void User::askPasswordOption(int pl) {
    int choice;
    cout << "Type password option: " << endl;
    cout << "1: Use personal password" << endl;
    cout << "2: Random generated password" << endl;
    cout << ">> ";
    cin >> choice;

    switch (choice) {
        case 1:
            cout << "[ Chose personal Password ]\n" << endl;
            askPersonalPassword(pl);
            cout << "[ Personal password: " << _password << " ]\n"<< endl;
            break;
        case 2:
            cout << "[ Chose random generated password ]" << endl;
            generateRandomPassword(pl);
            cout << "[ Generated password: " << _password << " ]\n"<< endl;
            break;
        default:
            cout << "Invalid input\n" << endl;
    }
}


void User::askPersonalPassword(int pl) {
    while(true) {
        cout << "Type your password (length = " << pl << "): " << endl;
        cout << ">> ";

        cin.ignore();
        getline(cin, _password);
        if (_password.length() != static_cast<size_t>(pl)) {
            cin.clear();
            cout << "Invalid password length. Please enter a password of length : " << pl << "\n"<< endl;
        }
        else
            break;
    }
}


void User::generateRandomPassword(int length) {
    string characters = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
    for (int i = 0; i < length; ++i) {
        _password += characters[rand() % characters.length()];
    }
}


void User::inputProcess() {
    srand(static_cast<unsigned int>(time(0)));
    askEncryptMode();
    askInOutFileType();
    askRoundKeyType();
}
