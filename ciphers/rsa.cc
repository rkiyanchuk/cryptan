//  Created on: Mar 25, 2010
//  Copyright (c) 2010 - Zoresvit
//	email: zoresvit@gmail.com

#include <cstdlib>
#include <iostream>
#include <cctype>
#include <string>
#include <gmp.h>
#include <gmpxx.h>

using namespace std;

//============================================================
// Generate P, Q and N
//============================================================
void genMod(mpz_class &prime_p, mpz_class &prime_q, mpz_class &mod, unsigned long bits)
{
    mpz_t temp;
    mpz_init(temp);

    gmp_randclass rand (gmp_randinit_mt);       // init rand generator

    // not reliable. Need to change for LCG or LRR
    rand.seed(std::rand());                     // get random seed (system time)

    prime_p = rand.get_z_bits(bits);            // get random number of bits length
    mpz_nextprime(temp, prime_p.get_mpz_t());   // find prime next after rand number
    prime_p = mpz_class (temp);                 // record the prime

    prime_q = rand.get_z_bits(bits);
    mpz_nextprime(temp, prime_q.get_mpz_t());
    prime_q = mpz_class (temp);

    mod = prime_p * prime_q;                    // find the modulus value
    mpz_clear(temp);
}

//============================================================
// Generate private and public keys
//============================================================
void genKeys(mpz_class &_pubkey, mpz_class &_private, mpz_class &prime_p, mpz_class &prime_q)
{
    mpz_t temp;
    mpz_t x;    // x from Diophantine equation
    mpz_t y;    // y from Diophantine equation

    mpz_init(temp);
    mpz_init(x);
    mpz_init(y);

    mpz_class gcd;
    mpz_class totient = (prime_p - 1) * (prime_q - 1);  // find Euler's totient
    mpz_class mod = prime_p * prime_q;

    gmp_randclass rand(gmp_randinit_default);
    rand.seed(std::rand());

    bool found = false;

    // generate random public key (key > 65535 and key < totient)
    while(found != true)
    {
        _pubkey = rand.get_z_range(totient);
        mpz_nextprime(temp, _pubkey.get_mpz_t());
        _pubkey = mpz_class(temp);

        // key needs to be larger than 65535 and GCD with totient = 1
        mpz_gcd(temp, _pubkey.get_mpz_t(), totient.get_mpz_t());
        gcd = mpz_class(temp);

        if((_pubkey > 0xFFFF) && (gcd == 1))
        {
            found = true;
        }
    }

    // Solve Diophantine equation
    mpz_set_ui(temp, 1);
    mpz_gcdext(temp, x, y, totient.get_mpz_t(), _pubkey.get_mpz_t());

    _private = mpz_class(y);

    if(_private < 0)
    {
        _private += totient;
    }

    mpz_mod(temp, _private.get_mpz_t(), totient.get_mpz_t());
    _private = mpz_class(temp);


    mpz_clear(temp);
    mpz_clear(x);
    mpz_clear(y);
}

void encipher(  mpz_class &ciph, const mpz_class &msg,
        const mpz_class &key, const mpz_class &mod)
{
    mpz_t temp;
    mpz_init(temp);

    mpz_powm(temp, msg.get_mpz_t(), key.get_mpz_t(), mod.get_mpz_t());
    ciph = mpz_class(temp);

    mpz_clear(temp);
}

void decipher(  mpz_class &msg, const mpz_class &ciph,
        const mpz_class &key, const mpz_class &mod)
{
    mpz_t temp;
    mpz_init(temp);

    mpz_powm(temp, ciph.get_mpz_t(), key.get_mpz_t(), mod.get_mpz_t());
    msg = mpz_class(temp);

    mpz_clear(temp);
}



int main (void)
{
    srand(time(NULL));

    mpz_class p;
    mpz_class q;
    mpz_class m;
    mpz_class pub;
    mpz_class priv;
    mpz_class msg;
    mpz_class cip;
    mpz_class temp;

    cout << "Enter message (numbers): ";
    char strtemp[1000] = {0};
    cin >> strtemp;
    msg = strtemp;

    int len;
    cout << "Enter module length: ";
    cin >> len;

    genMod(p, q, m, len);
    genKeys(pub, priv, p, q);

    encipher(cip, msg, pub, m);
    decipher(temp, cip, priv, m);

    cout    << "\n============================================================" << endl
        << "RSA Parameters:"
        << "\n============================================================" << endl
        << "P: \n" << hex << p << endl
        << "\n------------------------------------------------------------" << endl
        << "Q: \n" << q << endl
        << "\n------------------------------------------------------------" << endl
        << "Mod:\n" << m << endl
        << "\n------------------------------------------------------------" << endl
        << "Public:\n" << pub << endl
        << "\n------------------------------------------------------------" << endl
        << "Private:\n" << priv << endl
        << "\n------------------------------------------------------------" << endl;

    cout    << "\n============================================================" << endl
        << "Plain Text:\n" << dec << msg << endl
        << "------------------------------------------------------------" << endl
        << "Cipher Text\n" << hex << cip << endl
        << "------------------------------------------------------------" << endl
        << "Deciphered Text\n" << dec << temp << endl
        << "------------------------------------------------------------" << endl;



    return 0;
}
