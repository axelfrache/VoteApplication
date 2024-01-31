#ifndef __CRYPTO__
#define __CRYPTO__
#include <gmp.h>

void generate_keys(mpz_t n, mpz_t lambda, mpz_t g, mpz_t mu);
void encrypt(mpz_t c, mpz_t m, mpz_t n, mpz_t g);
void decrypt(mpz_t m, mpz_t c, mpz_t lambda, mpz_t mu, mpz_t n);

#endif
