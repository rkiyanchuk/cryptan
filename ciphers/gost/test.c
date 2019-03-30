#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "gost.h"


#define ENC_SET 12500000 /* *8 bytes = 100 Megabytes */

/* Designed to cope with 15-bit rand() implementations */
#define RAND32 ((word32)rand() << 17 ^ (word32)rand() << 9 ^ rand())

int main(int argc, char* argv[]) {
    word32 key[4][8] = {
        {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 
        {0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF}, 
        {0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF, 0x01234567, 0x89ABCDEF}, 
        {0x6ebabf8d, 0x1a8cad60, 0x124744f9, 0xd400b5d8, 0xa721e3fd, 0x11d0702d, 0x06fd4827, 0x476df4bf}
    };

    word32 plain[5][2] = {
        {0x0, 0x0}, 
        {0xACACACAC, 0xBDBDBDBD}, 
        {0xFFFFFFFF, 0xFFFFFFFF}, 
        {0x01234567, 0x89ABCDEF}, 
        {0xC8A99A39, 0xC0AE942B}
    };
    word32 cipher[2] = {0};

    kboxinit();

    int k, p;
    for (k = 0; k < 4; ++k) {
        printf("\nkey\t%8.8X %8.8X %8.8X %8.8X %8.8X %8.8X %8.8X %8.8X\n", 
                key[k][0], key[k][1], key[k][2], key[k][3], key[k][4], key[k][5], key[k][6], key[k][7]);
        for (p = 0; p < 5; ++p) {
            gostcrypt(plain[p], cipher, key[k]);
            printf("\nplain\t%8.8X %8.8X\n", plain[p][1], plain[p][0]);
            printf("cipher\t%8.8X %8.8X\n", cipher[1], cipher[0]);
        }
    }

    /*
       if(strcmp(argv[1], "test") == 0) {
       int i, j;
       word32 key[8];
       word32 plain[2];
       word32 cipher[2];

       kboxinit();

       printf("GOST 21847-89 selftest.\n");

       for (i = 0; i < 1000; i++) {
       for (j = 0; j < 8; j++)
       key[j] = RAND32;
       plain[0] = RAND32;
       plain[1] = RAND32;

       printf("%3d\r", i);
       fflush(stdout);

       gostcrypt(plain, cipher, key);
       gostdecrypt(cipher, cipher, key);

       if (plain[0] != cipher[0] || plain[1] != cipher[1]) {
       fprintf(stderr, "\nError! i = %d\n", i);
       return 1;
       }
       }
       printf("TEST PASSED\n");
       } else if (strcmp(argv[1], "bench-cache") == 0) {

       int i;
       word32 key[8];
       word32 plain[2];
       word32 cipher[2];

       kboxinit();

       for (i = 0; i < ENC_SET; ++i) {
       gostcrypt(plain, cipher, key);
       }
       } else if (strcmp(argv[1], "bench-nocache") ==0) {

       int i;
       word32 key[8];

       word32* plain = (word32*)malloc(ENC_SET * sizeof(word32) * 2);
       word32* cipher = (word32*)malloc(ENC_SET * sizeof(word32) * 2);

       kboxinit();

       for (i = 0; i < ENC_SET; ++i) {
       gostcrypt(&plain[2 * i], &cipher[2 * i], key);
       }

       free(plain);
       free(cipher);
       }
       */
    return 0;
}
