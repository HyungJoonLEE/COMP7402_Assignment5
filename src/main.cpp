#include "DES.h"
#include "AES.h"

int main() {
    User user;
    user.inputProcess();

    switch (user.getType()) {
        case EncryptionType::DES_:
            DES des;
            if (user.getMode() == "ECB") {

            }
            if (user.getMode() == "CBC") {

            }
            break;
        case EncryptionType::AES_:
            AES aes;
            if (user.getMode() == "ECB") {

            }
            if (user.getMode() == "CBC") {

            }
            break;

    }




    return 0;
}