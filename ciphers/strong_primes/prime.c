/* ================================================================= 
 * DESCRIPTION: Strong primes generation using modified Gordon's method
 *     CREATED: 2010 Nov 24
 *      AUTHOR: Zoresvit                     zoresvit@gmail.com
 *              Kharkiv National University of Radioelectronics
 * =================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "prime.h"

void genPrime(mpz_t prime, size_t length, gmp_randstate_t rand_state)
{
    mpz_init(prime);
    do 
    {
        mpz_urandomb(prime, rand_state, length);
        mpz_nextprime(prime, prime);
    } while (mpz_sizeinbase(prime, 2) != length);
}

void gen2LevelPrime(mpz_t prime, size_t length, gmp_randstate_t rand_state)
{
    size_t sublen = findSubLength(length);
    mpz_t r, s;

    mpz_init(r);
    mpz_init(s);

    genPrime(r, sublen, rand_state);
    genPrime(s, sublen, rand_state);

    genNextLevelPrime(prime, r, s, length, rand_state);
    mpz_clear(r);
    mpz_clear(s);
}

void genNextLevelPrime(mpz_t prime, mpz_t r, mpz_t s, size_t length, gmp_randstate_t rand_state)
{
    size_t k;

    mpz_t sinv;     /* s^(-1) - inverse of s (mod r) */
    mpz_t pow;      /* 2^(n-1) */
    mpz_t dprod;    /* 2rs */
    mpz_t Q;        /* [pow / dprod] - integer part */
    mpz_t R;        /* remainder of pow/dprod */
    mpz_t cond;     /* temporary variable for 2*s*s^(-1) - 1 to check condition */
    mpz_t Po;
    /* Po = 2*s*s^(-1) - 1 + Q*2rs, if 2*s*s^(-1) - 1 > R */
    /* Po = 2*s*s^(-1) - 1 + (Q+1)*2rs, if 2*s*s^(-1) - 1 < R*/

    mpz_init(prime);
    mpz_init(sinv);
    mpz_init(pow);
    mpz_init(dprod);
    mpz_init(Q);
    mpz_init(R);
    mpz_init(cond);
    mpz_init(Po);

    inverse(sinv, s, r);

    mpz_ui_pow_ui(pow, 2, length-1);

    /* find product 2rs */
    mpz_mul_ui(dprod, r, 2);
    mpz_mul(dprod, dprod, s);

    /* compute Q and R */
    mpz_fdiv_qr(Q, R, pow, dprod);

    /* compute 2*s*s^(-1) - 1 for condition check */
    mpz_mul_ui(cond, s, 2);
    mpz_mul(cond, cond, sinv);
    mpz_sub_ui(cond, cond, 1);

    if(mpz_cmp(cond, R) > 0)
    {
        mpz_mul(Po, Q, dprod);
        mpz_add(Po, Po, cond);
    }
    else /* cond < R */
    {
        mpz_add_ui(Q, Q, 1);
        mpz_mul(Po, Q, dprod);
        mpz_add(Po, Po, cond);
    }

    /* TODO: fix the magic number and use 2k < 2^t limit  */
    for(k = 0; k < 0.35*(length-1); ++k)
    {
        mpz_mul_ui(prime, dprod, k);
        mpz_add(prime, prime, Po);

        /* exit loop if the number is definately prime (return value is 2) */
        if(mpz_probab_prime_p(prime, 40))
            break;
    }

    mpz_clear(sinv);
    mpz_clear(pow);
    mpz_clear(dprod);
    mpz_clear(Q);
    mpz_clear(R);
    mpz_clear(cond);
    mpz_clear(Po);
}

void genStrongPrime(mpz_t prime, size_t length, gmp_randstate_t rand_state)
{
    size_t lev2len = findSubLength(length);
    mpz_t lev2r, lev2s;

    mpz_init(lev2r);
    mpz_init(lev2s);

    do
    {
        /* generate two level 2 primes */
        gen2LevelPrime(lev2r, lev2len, rand_state);
        gen2LevelPrime(lev2s, lev2len, rand_state);

        /* generate level 1 prime out of two generated level 2 primes */
        genNextLevelPrime(prime, lev2r, lev2s, length, rand_state);
    }while(mpz_sizeinbase(prime, 2) != length);

    mpz_clear(lev2r);
    mpz_clear(lev2s);
}

void inverse(mpz_t inv, mpz_t n, mpz_t mod)
{
    mpz_t d, y;
    mpz_init(d);
    mpz_init(y);
    mpz_init(inv);

    mpz_gcdext(d, inv, y, n, mod);

    if(mpz_cmp_ui(d, 1) > 0)
    {
        /* inverse element doesn't exist */
        mpz_set_ui(inv, 0);
    }

    /* keep inverse value positive */
    mpz_mod(inv, inv, mod);

    mpz_clear(d);
    mpz_clear(y);
    return;
}

size_t findSubLength(size_t length)
{
    /* magic 0.693147 is Ln(2) described in the Gordon's algorithm */
    double success_trait = 0.693147 * ((double)length - 1);
    size_t t = 1;

    do
    {
        ++t;
    }
    while((2 << (t-1)) <= success_trait); /* this horror just compares 2^t with success_trait */
    return ((length - t) / 2);
}
