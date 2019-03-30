/*
 * Copyright 2011 Zoresvit (c) <zoresvit@gmail.com>
 *
 * MDS Permutation working example.
 */

#include <stdio.h>
#include "mds.h"

#define INT_TO_BYTES(num, arr) \
    arr[0] = (num) & 0xFF; \
arr[1] = ((num) & (0xFF << 8)) >> 8; \
arr[2] = ((num) & (0xFF << 16)) >> 16; \
arr[3] = ((num) & (0xFF << 24)) >> 24; \

#define BYTES_TO_INT(num, arr) \
    num = 0; \
num |= arr[0]; \
num |= arr[1] << 8; \
num |= arr[2] << 16; \
num |= arr[3] << 24;

int main(int argc, char** argv) {

    int i, res;
    long c;
    unsigned char state[4] = {0};

    for (c = 0; c <= 0xFFFF; ++c) {
        INT_TO_BYTES(c << 16, state);

        for (i = 0; i < 4; ++i)
            state[i] = aes_sbox[state[i]];

        MdsPermute(state);

        BYTES_TO_INT(res, state);
        printf("%x\t%x\n", c << 16, (res & 0xFFFF0000) >> 16);
    }

    return 0;
}
