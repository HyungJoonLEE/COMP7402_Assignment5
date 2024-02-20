//#include "CBC.h"
#include "ECB.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    User user;
    user.setEnvironment();



//    CBC *cbc = new CBC();

    if (user.isEncryption()) {
        if (user.getMode() == encryptMode::ECB_) {
            ECB *ecb = new ECB();
            ecb->processEncrypt(user);
            delete ecb;
        }
        if (user.getMode() == encryptMode::CBC_) {
            // TODO: CBC Encrypt
        }
    } else {  // Decrypt mode
        if (user.getMode() == encryptMode::ECB_) {
            ECB *ecb = new ECB();
            ecb->processDecrypt(user);
            delete ecb;
        }
        if (user.getMode() == encryptMode::CBC_) {
            // TODO: CBC Decrypt
        }
    }

    return 0;
}