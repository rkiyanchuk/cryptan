/*
 *	Optimized implementation of SKIPJACK algorithm
 *
 *	originally written by Panu Rissanen <bande@lut.fi> 1998.06.24
 *	optimized by Mark Tillotson <markt@chaos.org.uk> 1998.06.25
 *	optimized by Paulo Barreto <pbarreto@nw.com.br> 1998.06.30
 *	gnupg support by Werner Koch <dd9jn@amsat.org> 1998.07.02
 *
 *	Slightly refactored code so only the cipher implementation stuff is left,
 *	also split down to header and source files for ease of creating and using it
 *	as static library. No optimization added. 
 *	Zoresvit <zoresvit@gmail.com> 2011.05.08
 */

#ifndef __SKIPJACK_H__
#define __SKIPJACK_H__


/* configuration stuff */

#ifdef __x86_64__
#define SIZEOF_UNSIGNED_LONG 8
#else
#define SIZEOF_UNSIGNED_LONG 4
#endif

#if defined(__mc68000__) || defined (__sparc__) || defined (__PPC__) \
    || (defined(__mips__) && (defined(MIPSEB) || defined (__MIPSEB__)) )
#define BIG_ENDIAN_HOST 1
#else
#define LITTLE_ENDIAN_HOST 1
#endif

typedef unsigned long   ulong;
typedef unsigned short  ushort;
typedef unsigned char   byte;
typedef unsigned short  u16;

#if SIZEOF_UNSIGNED_LONG == 4
typedef unsigned long   u32;
typedef unsigned long   word32;
#else
typedef unsigned int    u32;
typedef unsigned int    word32;
#endif

/* end configurable stuff */


#ifndef DIM
#define DIM(v) (sizeof(v)/sizeof((v)[0]))
#define DIMof(type,member)   DIM(((type *)0)->member)
#endif


typedef struct {
    byte tab[10][256];
} SJ_context;


void makeKey(SJ_context *ctx, byte *key, unsigned keylen);
void encrypt_block(SJ_context* ctx, byte* in, byte* out);
void decrypt_block(SJ_context* ctx, byte* in, byte* out);

#endif /* __SKIPJACK_H__ */
