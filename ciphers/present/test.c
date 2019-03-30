#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "present.h"

/* #define ENC_SET 12500000  *8 bytes = 100 Megabytes */ 
#define ENC_SET 125000 /* *8 bytes = 100 Megabytes */


int selftest();


int main(int argc, char* argv[]) {

    if(strcmp(argv[1], "test") == 0) {
        selftest();
    } else if (strcmp(argv[1], "bench-cache") == 0) {

        int i;
        uint64_t plain = 0xDEADBEAFFAEBDAED;
        uint64_t cipher;
        uint64_t key[2];
        uint64_t subkeys[32];

        KeySchedule(key, subkeys);

        for(i = 0; i < ENC_SET; ++i) {
            cipher = encipher(plain, subkeys);
        }
    } else if (strcmp(argv[1], "bench-nocache") ==0) {

        int i;
        uint64_t key[2];
        uint64_t subkeys[32];
        uint64_t* plains = malloc(sizeof(uint64_t) * ENC_SET);
        uint64_t* ciphers = malloc(sizeof(uint64_t) * ENC_SET);

        KeySchedule(key, subkeys);
        for(i = 0; i < ENC_SET; ++i) {
            ciphers[i] = encipher(plains[i], subkeys);
        }

        free(plains);
        free(ciphers);
    }
    return 0;
}


int selftest() {
    uint64_t plains[4] = {
        0x0000000000000000, 
        0x0000000000000000, 
        0xFFFFFFFFFFFFFFFF, 
        0xFFFFFFFFFFFFFFFF
    };
    uint64_t ciphers[4] = {
        0x5579C1387B228445, 
        0xE72C46C0F5945049, 
        0xA112FFC72F68417B, 
        0x3333DCD3213210D2
    };
    uint64_t keys[4][2] = {
        {0x0000000000000000, 0x0000}, 
        {0xFFFFFFFFFFFFFFFF, 0xFFFF}, 
        {0x0000000000000000, 0x0000}, 
        {0xFFFFFFFFFFFFFFFF, 0xFFFF} 
    };
    uint64_t enc = {0};
    uint64_t subkeys[32];
    int i;
    for (i = 0; i < 4; ++i) {
        KeySchedule(&keys[i][0], subkeys);
        enc = encipher(plains[i], subkeys);
        if(enc != ciphers[i]) {
            printf("Enciphering ERROR!\n\n");

            printf("enc:\t%8.8lx\n", enc);
            printf("ciph:\t%8.8lx\n", ciphers[i]);
            printf("pln:\t%8.8lx\n", plains[i]);
            printf("key:\t%16.16lx%16.16lx\n\n", keys[i][1], keys[i][0]);
            return -1;
        }
    }
    printf("TEST PASSED\n");
    return 0;
}
