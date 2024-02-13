#include "DES.h"
#include "AES.h"

int main() {
    User user;
//    user.inputProcess();
    user.setType(1);
    user.setMode("ECB");
    user.setOutFile("a.txt");
    user.setPtMode(true);
    user.setKeyFlag(1);
    user.setRoundNum(8);
    user.setPKey("03ksb~l!");

    DES des;
    AES aes;
    switch (user.getType()) {
        case DES_:
            if (user.getMode() == "ECB") des.handleDES_ECB(user);
            if (user.getMode() == "CBC") des.handleDES_CBC(user);
            break;
        case AES_:
            if (user.getMode() == "ECB")
            if (user.getMode() == "CBC")
            break;
        default:
            cout << "Shouldn't get here" << endl;
            break;
    }

    return 0;
}