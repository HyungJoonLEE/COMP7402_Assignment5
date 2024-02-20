#include "Key.h"


void Key::generateRoundKeys(string key, int round, bool flag) {
    int count = 0;
    if (flag) {
        roundKeys_.emplace_back("000000000000000011011101110111011101110111011101");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000010101010101010101010101010101010");
        roundKeys_.emplace_back("000000000000000011011101110111011101110111011101");
        roundKeys_.emplace_back("000000000000000010111011101110111011101110111011");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000011101110111011101110111011101110");
        roundKeys_.emplace_back("000000000000000011111111111111111111111111111111");
    }
    else {
        // Compressing the key using the PC1 table
        string perm_key;
        for (int i : pc1) {
            perm_key += key[i - 1];
        }

        // Dividing the key into two equal halves
        string left  = perm_key.substr(0, 28);
        string right = perm_key.substr(28, 28);

        for(int i = 0; i < 16; i++){
            // Rounds 1, 2, 9, 16: key_chunks are shifted by one.
            if(i == 0 || i == 1 || i == 8 || i == 15 ){
                left = shift_left_once(left);
                right = shift_left_once(right);
            }

            //  Other rounds: key_chunks are shifted by two
            else {
                left = shift_left_twice(left);
                right = shift_left_twice(right);
            }

            // Combining the two chunks
            string combined_key = left + right;
            string round_key;

            // Using the PC2 table to transpose the key bits
            for (int j : pc2) {
                round_key += combined_key[j - 1];
            }

            roundKeys_.push_back(round_key);
            count++;
            if (count == round) break;
        }
    }
}


void Key::generateReverseRoundKeys(const vector<string> &rkv, int round) {
    int i = round - 1;
    while(i >= 0) {
        reverseRoundKeys_.push_back(rkv[i]);
        i--;
    }
}


string Key::shift_left_once(string key_chunk){
    string shifted;

    for(int i = 1; i < 28; i++){
        shifted += key_chunk[i];
    }
    shifted += key_chunk[0];

    return shifted;
}


string Key::shift_left_twice(string key_chunk) {
    string shifted;

    for(int i = 0; i < 2; i++){
        for(int j = 1; j < 28; j++){
            shifted += key_chunk[j];
        }
        shifted += key_chunk[0];
        key_chunk = shifted;
        shifted = "";
    }

    return key_chunk;
}

vector<string> Key::getRK() const {
    return roundKeys_;
}

vector<string> Key::getRRK() const {
    return reverseRoundKeys_;
}
