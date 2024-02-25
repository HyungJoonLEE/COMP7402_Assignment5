//#include "CBC.h"
#include "Feistel.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    User u;
    u.setEnvironment();
    auto *f = new Feistel();

    if (u.isEncryption()) {
        if (u.getMode() == encryptMode::ECB_) {
            f->ECBencrypt(u);
        }
        if (u.getMode() == encryptMode::CBC_) {
            f->CBCencrypt(u);
        }
        cout << "\nSuccessfully written to " << u.getOutFile() << endl;
    }
    else {  // Decrypt mode
        if (u.getMode() == encryptMode::ECB_) {
            f->ECBdecrypt(u);
        }
        if (u.getMode() == encryptMode::CBC_) {
            f->CBCdecrypt(u);
        }
        printDifferenceRate(u.getInFile(), u.getOutFile());
    }

    delete f;

    return 0;
}