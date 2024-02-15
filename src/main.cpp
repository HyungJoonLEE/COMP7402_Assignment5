//#include "CBC.h"
#include "ECB.h"

int main() {
    User user;
    user.setEnvironment();
//    user.setType(1);
//    user.setMode("CBC");
//    user.setOutFile("a.txt");
//    user.setPtMode(true);
//    user.setKeyFlag(1);
//    user.setRoundNum(8);
//    user.setPKey("03ksb~l!");
    ECB *ecb = new ECB();
//    CBC *cbc = new CBC();


    if (user.isEncryption()) {    // Encrypt mode
        switch (user.getMode()) {
            case encryptMode::ECB_:
                ecb->processEncrypt(user);
                break;
            case encryptMode::CBC_:

                break;
            default:
                cout << "Shouldn't get here" << endl;
                break;
        }
    }
    else {  // Decrypt mode

    }

    delete ecb;
//    delete cbc;

    return 0;
}