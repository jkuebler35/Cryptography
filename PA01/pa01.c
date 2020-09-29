#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, const char* argv[]){
    if (argc != 2){
        return 1;
    }

    mpz_t p, q, n, d, e, c, m, c_p, d_p, p_p, q_p, m_p, n_p;
    mpz_inits(p, q, n, d, e, c, m, c_p, d_p, p_p, q_p, m_p, n_p, NULL);
    gmp_sscanf(argv[1], "%Zd,%Zd,%Zd,%Zd,%Zd", m, c_p, d_p, p_p, q_p);

    // Part 1 Setup
    mpz_t r;
    mpz_inits(r, NULL);

    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, 1234567890);

    mpz_rrandomb(r, state, 501);   // random number of at least 500 bits

    mpz_nextprime(p, r);  // finds the next prime number after r
    mpz_nextprime(q, p);  // finds another prime number

    mpz_mul(n, p, q);     // calculates n

    mpz_clears(r,NULL);


    // Part 1 Key Generation
    mpz_t phi, p1, q1, gcd;
    mpz_inits(phi, p1, q1, gcd, NULL);

    mpz_sub_ui(p1, p, 1);  // used to calculate phi
    mpz_sub_ui(q1, q, 1);

    mpz_mul(phi, p1, q1);  // calculates phi

    while (mpz_cmp_ui(gcd, 1) != 0){  // continues until e is valid
        mpz_urandomm(e, state, phi);
        mpz_gcd(gcd, e, phi);
    }
    gmp_randclear(state);

    mpz_invert(d, e, phi);  // calcs d by inverse mod of e mod phi

    mpz_clears(phi, p1, q1, gcd, NULL);


    // Part 1 Encryption
    mpz_powm(c, m, e, n);  // calcualtes c using m e and n

    // Part 2
    mpz_mul(n_p, p_p, q_p);  // calcualte n
    mpz_powm(m_p, c_p, d_p, n_p); // decode c_p

    gmp_printf("%Zd,%Zd,%Zd,%Zd,%Zd", c, e, d, n, m_p);

    mpz_clears(p, q, n, d, e, c, m, c_p, d_p, p_p, q_p, m_p, n_p, NULL);

    return 0;
}
