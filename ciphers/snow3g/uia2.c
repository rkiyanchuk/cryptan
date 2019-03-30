/*---------------------------------------------------------
* uia2.c
*---------------------------------------------------------*/
#include "uia2.h"
#include <stdio.h>
#include <math.h>
#include <string.h>
#define LOOP 1
/* MUL64x.
* Input V: a 64-bit input.
* Input c: a 64-bit input.
* Output : a 64-bit output.
* A 64-bit memory is allocated which is to be freed by the calling
* function.
* See section 4.3.2 for details.
*/
u64 MUL64x(u64 V, u64 c)
{
if ( V & 0x8000000000000000 )
return (V << 1) ^ c;
else
return V << 1;
}
#if (LOOP == 0)
/* MUL64xPOW.
* Input V: a 64-bit input.
* Input i: a positive integer.
* Input c: a 64-bit input.
* Output : a 64-bit output.
* A 64-bit memory is allocated which is to be freed by the calling
function.
* See section 4.3.3 for details.
*/
u64 MUL64xPOW(u64 V, u8 i, u64 c)
{
if ( i == 0)
return V;
else
return MUL64x( MUL64xPOW(V,i-1,c) , c);
}
#else
/* MUL64xPOW.
* Input V: a 64-bit input.
* Input i: a positive integer.
* Input c: a 64-bit input.
* Output : a 64-bit output.
* A 64-bit memory is allocated which is to be freed by the calling
function.
* See section 4.3.3 for details.
*/
u64 MUL64xPOW(u64 V, u8 i, u64 c)
{
u64 tmp;
u8 z;
tmp = V;
for(z = 1;z<= i;z++)
	{
		tmp = MUL64x(tmp,c);
	}
return tmp;
}
#endif

/* MUL64.
* Input V: a 64-bit input.
* Input P: a 64-bit input.
* Input c: a 64-bit input.
* Output : a 64-bit output.
* A 64-bit memory is allocated which is to be freed by the calling
* function.
* See section 4.3.4 for details.
*/
u64 MUL64(u64 V, u64 P, u64 c)
{
u64 result = 0;
int i = 0;
for ( i=0; i<64; i++)
{
if( ( P>>i ) & 0x1 )
result ^= MUL64xPOW(V,i,c);
}
return result;
}
/* mask32bit.
* Input n: an integer in 1-32.
* Output : a 32 bit mask.
* Prepares a 32 bit mask with required number of 1 bits on the MSB side.
*/
u32 mask32bit(int n)
{
u32 mask=0x0;
if ( n%32 == 0 )
return 0xffffffff;
while (n--)
mask = (mask>>1) ^ 0x80000000;
return mask;
}
/* f9.
* Input key: 128 bit Integrity Key.
* Input count:32-bit Count, Frame dependent input.
* Input fresh: 32-bit Random number.
* Input dir:1 bit, direction of transmission (in the LSB).
* Input data: length number of bits, input bit stream.
* Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
* Output : 32 bit block used as MAC
* Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
*/
u8* uia2( u8* key, int count, int fresh, int dir, u8 *data, u64 length)
{
u32 K[4],IV[4], z[5];
int i=0,D;
static u32 MAC_I = 0; /* static memory for the result */
u64 EVAL;
u64 V;
u64 P;
u64 Q;
u64 c;
u64 M_D_2;
int rem_bits = 0;
u32 mask = 0;
u32 *message;
message = (u32*)data; /* To operate 32 bit message internally. */
/* Load the Integrity Key for SNOW3G initialization as in section 4.4. */
memcpy(K+3,key+0,4); /*K[3] = key[0]; we assume
K[3]=key[0]||key[1]||...||key[31] , with key[0] the
* most important bit of key*/
memcpy(K+2,key+4,4); /*K[2] = key[1];*/
memcpy(K+1,key+8,4); /*K[1] = key[2];*/
memcpy(K+0,key+12,4); /*K[0] = key[3]; we assume
K[0]=key[96]||key[97]||...||key[127] , with key[127] the
* least important bit of key*/
/* Prepare the Initialization Vector (IV) for SNOW3G initialization as in
section 4.4. */
IV[3] = count;
IV[2] = fresh;
IV[1] = count ^ ( dir << 31 ) ;
IV[0] = fresh ^ (dir << 15);
z[0] = z[1] = z[2] = z[3] = z[4] = 0;
/* Run SNOW 3G to produce 5 keystream words z_1, z_2, z_3, z_4 and z_5. */
Initialize(K,IV);
GenerateKeystream(5,z);
P = (u64)z[0] << 32 | (u64)z[1];
Q = (u64)z[2] << 32 | (u64)z[3];
/* Calculation */
D = ceil( length / 64.0 ) + 1;
EVAL = 0;
c = 0x1b;
/* for 0 <= i <= D-3 */
for (i=0;i<D-2;i++)
{
V = EVAL ^ ( (u64)message[2*i] << 32 | (u64)message[2*i+1] );
EVAL = MUL64(V,P,c);
}
/* for D-2 */
rem_bits = length % 64;
if (rem_bits == 0)
rem_bits = 64;
mask = mask32bit(rem_bits%32);
if (rem_bits > 32)
{
M_D_2 = ( (u64) message[2*(D-2)] << 32 ) | (u64) (message[2*(D-2)+1] &
mask);
} else
{
M_D_2 = ( (u64) message[2*(D-2)] & mask) << 32 ;
}
V = EVAL ^ M_D_2;
EVAL = MUL64(V,P,c);
/* for D-1 */
EVAL ^= length;
/* Multiply by Q */
EVAL = MUL64(EVAL,Q,c);
MAC_I = (u32)(EVAL >> 32) ^ z[4];
return (u8*) &MAC_I;
}
/* End of uia2.c */
/*------------------------------------------------------------------------*/
