//#include "CBC.h"
#include "ECB.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    User user;
    user.setEnvironment();

    ECB *ecb = new ECB();
//    CBC *cbc = new CBC();

    if (user.isEncryption()) {
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
    } else {  // Decrypt mode
        switch (user.getMode()) {
            case encryptMode::ECB_:
                ecb->processDecrypt(user);
                break;
            default:
                cout << "Shouldn't get here" << endl;
                break;
        }
    }


    delete ecb;
//    delete cbc;

    return 0;
}