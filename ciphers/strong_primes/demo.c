#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gmp.h>

#include "prime.h"

int main(int argc, char* argv[]) 
{
    mpz_t sprime;
    gmp_randstate_t rand_state;

    mpz_init(sprime);
    gmp_randinit_default(rand_state);
    gmp_randseed_ui (rand_state, time(NULL));

    printf("Strong prime generation demo\n");

    genStrongPrime(sprime, 128, rand_state);
    mpz_out_str(stdout, -16, sprime);
    printf("\n");

    return 0;
}

