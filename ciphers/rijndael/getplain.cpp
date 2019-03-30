/* 
 * CREATED : 2011 Feb 18
 * AUTHOR  : Zoresvit (zoresvit@gmail.com)
 *           Kharkiv National University of Radioelectronics
 * DESCR   : 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <ctime>
#include <map>
#include <limits.h>

#include "rijndael.h"

using namespace std;

typedef unsigned char ubyte;

inline uint xorMask(uint value, uint mask)
{
    value = value & mask;
    bool xorbit = 0;
    while(value != 0)
    {
        xorbit ^= value & 0x1;
        value >>= 1;
    }
    return xorbit;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));

    if(argc != 3)
    {
        cerr << "Wrong arguments!" << endl;
        return 1;
    }

    int i;
    size_t count = 0;
    size_t klen;
    FILE* fp;
    aes_ctx_t* ctx;
    aes_init();

    /*
       if(strcmp(argv[1], "--key") == 0)
       {
       klen = atoi(argv[2])/8;
       }
       */

    klen = 128/8;

    unsigned char* key = (unsigned char*)malloc(klen);
    memset(key, 0, klen);
    for(i = 0; i < klen; ++i)
        key[i] = rand();

    unsigned char plain[16] = {0};
    unsigned char cipher[16] = {0};
    unsigned char iv[16] = {0};

    /*
       for(int i = 0; i < klen; ++i)
       {
       key[i] = rand();
       }
       for(int i = 0; i < 16; ++i)
       {
       iv[i] = rand();
       }
       */

    ctx = aes_alloc_ctx(key, klen);
    // ctx = iv_alloc_ctx(key, klen, iv);

    unsigned long int pl = 0;
    unsigned long int ciph = 0;
    unsigned long int imask = atoi(argv[1]);
    unsigned long int omask = atoi(argv[2]);
    long int matches = -2147483648;

    cout << "Key:\t\t";
    for(i = klen-1; i >= 0; --i)
        cout << hex << static_cast<int>(key[i]);


    for(pl = 0; pl <= 0xFFFFFFFF; ++pl)
    {
        if(pl % 1000000 == 0)
        {
            cerr << "Computing:\t" << pl << "\tCurrent matches:\t" << matches << endl;
        }

        plain[0] = pl & 0x000000FF;
        plain[1] = (pl & 0x0000FF00) >> 8;
        plain[2] = (pl & 0x00FF0000) >> 16;
        plain[3] = (pl & 0xFF00000000) >> 24;

        aes_encrypt(ctx, plain, cipher);

        ciph = cipher[0];
        ciph |= cipher[1] << 8;
        ciph |= cipher[2] << 16;
        ciph |= cipher[3] << 24;

        if(xorMask(pl, imask) == xorMask(ciph, omask))
            ++matches;
    }

    cout << dec << endl << "Input Mask:\t" << imask << endl;
    cout << "Output Mask:\t" << omask << endl;
    cout << "Final Matches:\t" << matches << endl;

    aes_free_ctx(ctx);

    free(key);
    return 0;
}

