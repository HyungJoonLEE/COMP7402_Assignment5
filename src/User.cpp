#include "User.h"

void User::askEncryptType() {
    int choice;

    while (true) {
        cout << "Please choose an option:" << endl;
        cout << "1. DES" << endl;
        cout << "2. AES" << endl;
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
                cout << "[ Encrypt in DES mode ]\n" << endl;
                _eMode = "DES";
                return;
            case 2:
                cout << "[ Encrypt in AES mode ]\n" << endl;
                _eMode = "AES";
                return;
            default:
                cout << "Invalid input\n" << endl;
                // Optionally clear the input buffer if expecting further inputs
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
                cout << "[ File mode ]\n" << endl;
                cout << "Enter input file name: ";
                cin >> _inFile;
                cout << "[ Read from: " << _inFile << " ]\n" << endl;
                cout << "Enter output file name: ";
                cin >> _outFile;
                cout << "[ Write to: " << _outFile << " ]\n" << endl;
                return;
            case 2:
                cout << "[ Plaintext mode ]\n" << endl;
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
                cout << "[ Predefined Round keys ]\n" << endl;
                _keyFlag = keyFlag::PRE_DEFINED;
                setRoundKeys(true);
                return;
            case 2:
                cout << "[ User defined Round keys\n" << endl;
                _keyFlag = keyFlag::USER_DEFINED;
                setRoundKeys(false);
                return;
            case 3:
                cout << "[ Default Program settings ]\n" << endl;
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
        askUserRoundNum();
//        askUserRoundKeys();
    }
}


void User::setPredefinedRoundKeys() {
//    _roundKeys = {0xdddddddd, 0xeeeeeeee, 0xaaaaaaaa, 0xdddddddd,
//                  0xbbbbbbbb, 0xeeeeeeee, 0xeeeeeeee, 0xffffffff};
    cout << "Predefined round keys have been selected.\n" << endl;
}


void User::askUserRoundNum() {
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
            _roundNum = rn;
        }
    }
}


