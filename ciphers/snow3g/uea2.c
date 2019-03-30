/*---------------------------------------------------------
 * f8.c
 *---------------------------------------------------------*/
#include "uea2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* f8.
 * Input key: 128 bit Confidentiality Key.
 * Input count:32-bit Count, Frame dependent input.
 * Input bearer: 5-bit Bearer identity (in the LSB side).
 * Input dir:1 bit, direction of transmission.
 * Input data: length number of bits, input bit stream.
 * Input length: 16 bit Length, i.e., the number of bits to be encrypted or
 * decrypted.
 * Output data: Output bit stream. Assumes data is suitably memory
 * allocated.
 * Encrypts/decrypts blocks of data between 1 and 20000 bits in length as
 * defined in Section 3.
 */
void uea2( u8 *key, int count, int bearer, int dir, u8 *data, int length )
{
    u32 K[4],IV[4];
    int n = ( length + 31 ) / 32;
    int i=0;
    u32 *KS;
    /*Initialisation*/
    /* Load the confidentiality key for SNOW 3G initialization as in section
       3.4. */
    memcpy(K+3,key+0,4); /*K[3] = key[0]; we assume
                           K[3]=key[0]||key[1]||...||key[31] , with key[0] the
                          * most important bit of key*/
    memcpy(K+2,key+4,4); /*K[2] = key[1];*/
    memcpy(K+1,key+8,4); /*K[1] = key[2];*/
    memcpy(K+0,key+12,4); /*K[0] = key[3]; we assume
                            K[0]=key[96]||key[97]||...||key[127] , with key[127] the
                           * least important bit of key*/
    /* Prepare the initialization vector (IV) for SNOW 3G initialization as in
       section 3.4. */
    IV[3] = count;
    IV[2] = (bearer << 27) | ((dir & 0x1) << 26);
    IV[1] = IV[3];
    IV[0] = IV[2];
    /* Run SNOW 3G algorithm to generate sequence of key stream bits KS*/
    Initialize(K,IV);
    KS = (u32 *)malloc(4*n);
    GenerateKeystream(n,(u32*)KS);
    /* Exclusive-OR the input data with keystream to generate the output bit
       stream */
    for (i=0;i<n*4;i++)
        data[i] ^= *(((u8*)KS)+i);
    free(KS);
}
/* End of uea2.c */
