
#ifndef __GOST_H__
#define __GOST_H__

typedef unsigned int word32;
typedef unsigned long word64;

void kboxinit(void); 
void gostcrypt(word32* const in, word32* out, word32* const key);
void gostdecrypt(word32* const in, word32* out, word32* const key);

#endif /* __GOST_H__ */
