#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "skipjack.h"


#define ENC_SET 12500000 /* *8 bytes input = 100 Megabytes */


int selftest() {
    SJ_context c;
    byte inp[8] = {0x33, 0x22, 0x11, 0x00, 0xdd, 0xcc, 0xbb, 0xaa};
    byte key[10] = {0x00, 0x99, 0x88, 0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
    byte enc[8] = {0};
    byte dec[8] = {0};
    byte chk[8] = {0x25, 0x87, 0xca, 0xe2, 0x7a, 0x12, 0xd3, 0x00};

    makeKey(&c, key, 10);
    encrypt_block(&c, inp, enc);
    if(memcmp(enc, chk, 8)) {
        printf("Skipjack test encryption FAILED\n");
        return -1;
    }
    decrypt_block(&c, enc, dec);
    if(memcmp(dec, inp, 8)) {
        printf("Skipjack test decryption FAILED\n");
        return -1;
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if(strcmp(argv[1], "test") == 0){

        if (selftest() != 0) {
            printf("FAIL\n");
            return -1;
        } else {
            printf("TEST PASSED\n");
        }
    } else if (strcmp(argv[1], "bench-cache") == 0) {

        int i;
        SJ_context c;
        byte key[10] = {0};
        byte pln[8] = {0};
        byte enc[8] = {0};

        makeKey(&c, key, 10);
        for (i = 0; i < ENC_SET; ++i) {  
            encrypt_block(&c, pln, enc);
        }
    } else if (strcmp(argv[1], "bench-nocache") ==0) {

        int i;
        SJ_context c;
        byte key[10] = {0};

        byte* pln = malloc(8*ENC_SET);
        byte* enc = malloc(8*ENC_SET);

        makeKey(&c, key, 10);
        for (i = 0; i < ENC_SET; ++i) {  
            encrypt_block(&c, &pln[8 * i], &enc[8 * i]);
        }

        free(pln);
        free(enc);
    }
    return 0;
}
