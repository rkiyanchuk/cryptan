/*---------------------------------------------------------
* uea2.h
*---------------------------------------------------------*/
#ifndef uea2_H_
#define uea2_H_
#include "snow_3g.h"
/* uea2.
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
void uea2( u8 *key, int count, int bearer, int dir, u8 *data, int length );
#endif
