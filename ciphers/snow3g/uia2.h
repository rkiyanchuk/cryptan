/*---------------------------------------------------------
* uia2.h
*---------------------------------------------------------*/
#ifndef uia2_H_
#define uia2_H_
#include "snow_3g.h"
/* uia2.
* Input key: 128 bit Integrity Key.
* Input count:32-bit Count, Frame dependent input.
* Input fresh: 32-bit Random number.
* Input dir:1 bit, direction of transmission (in the LSB).
* Input data: length number of bits, input bit stream.
* Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
* Output : 32 bit block used as MAC
* Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
*/
u8* uia2( u8* key, int count, int fresh, int dir, u8 *data, u64 length);
#endif
