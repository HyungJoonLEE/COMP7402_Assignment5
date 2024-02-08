#include "ECB.h"

int main() {
    User user;
    srand(static_cast<unsigned int>(time(0)));

    user.askEncryptMode();
    user.askInOutFileType();
    user.askRoundKeyType();




    return 0;
}