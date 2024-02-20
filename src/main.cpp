//#include "CBC.h"
#include "Feistel.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    User user;
    user.setEnvironment();
    auto *f = new Feistel();

    if (user.isEncryption()) {
        if (user.getMode() == encryptMode::ECB_) {
            f->ECBencrypt(user);
        }
        if (user.getMode() == encryptMode::CBC_) {
            // TODO: CBC Encrypt
        }
    } else {  // Decrypt mode
        if (user.getMode() == encryptMode::ECB_) {
            f->ECBdecrypt(user);
        }
        if (user.getMode() == encryptMode::CBC_) {
            // TODO: CBC Decrypt
        }
    }

    delete f;

    return 0;
}