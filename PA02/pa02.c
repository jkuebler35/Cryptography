#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, const char* argv[]){
    if (argc != 2){
        return 1;
    }

    mpz_t m, x, q, p, r, g, p_1, p_2, p_q, mod1, mod2, temp, h, hr, c1, c2;
    mpz_inits(m, x, q, p, r, g, p_1, p_2, p_q, mod1, mod2, temp, h, hr, c1, c2, NULL);
    gmp_sscanf(argv[1], "%Zd", m);

    // Part 1 Setup
    mpz_set_str(x, "1234567890123456789012345678901234567890", 10);
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, (unsigned int)time(NULL));

    bool p_valid = false;
    mpz_rrandomb(p, state, 2001);   // random number of at least 2000 bits

    while (!p_valid){
        mpz_nextprime(p, p);  // finds the next prime number after p

        mpz_sub_ui(p_1, p, 1);    // calculates p-1
        mpz_fdiv_q_ui(q, p_1, 2);  // calculates q as p-1/2 = q

        if (mpz_probab_prime_p(q, 30) != 0){
            p_valid = true;
        }
    }


    bool secure = false;
    mpz_sub_ui(p_1, p, 1);      // p_1 = p-1
    mpz_fdiv_q_ui(p_2, p_1, 2); // p_2 = p-1/2
    mpz_fdiv_q(p_q, p_1, q);    // p_q = p-1/q

    while (!secure){
        mpz_urandomm(g, state, p);

        mpz_powm(mod1, g, p_2, p);  // g^p_2 mod p
        mpz_powm(mod2, g, p_q, p);  // g^p_q mod p

        int m1 = mpz_cmp_ui(mod1, 1);
        int m2 = mpz_cmp_ui(mod1, 1);

        if(m1 !=0 && m2 != 0){
            secure = true;
        }
        // else{
        //    gmp_printf("%Zd did not work", g);
        // }
    }

    // Key generaation
    mpz_powm_ui(g, g, 2, p);  // generator is g^2
    mpz_powm(h, g, x, p); // h = g^x mod p

    // Encyption
    for(int index = 0; index < 3; index+= 1){
        mpz_urandomm(r, state, q);  // r is random number less that q (order)
        mpz_powm(c1, g, r, p);      // computes g^r = c1
        mpz_powm(hr, h, r, p);      // computes h^r
        mpz_mul(c2, m, hr);
        mpz_mod(c2, c2, p);

        gmp_printf("%Zd,%Zd,%Zd\n\n", c1, c2, p);
    }


    mpz_clears(m, x, q, p, r, g, p_1, p_2, p_q, mod1, mod2, temp, h, hr, c1, c2, NULL);

    return 0;
}
