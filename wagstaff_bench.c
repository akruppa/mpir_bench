/*
 * wagstaff_bench.c
 * June 16, 2009
 * by: Jeff Gilchrist [http://gilchrist.ca/jeff]
 * Code optimized by Robert Gerbicz using bitshift+bitmask instead of divisions
 *
 * This program uses Anton Vrba's conjecture for proving Wagstaff numbers are
 * prime [ p = (2^q + 1)/3 ].
 * For more details see:  http://www.mersenneforum.org/showthread.php?t=10737
 *
 * ## Note: This is only a conjecture, and has not been proven! ##
 *
 * Sn+1 = Sn^2 - 2 and p be prime larger than 2
 * Wp = 1/3 (1 + 2^p) is prime if and only if Sp = S2 (mod Wp) where S0 = 6
 *
 * First 40 values of q that generate Wagstaff primes: 3, 5, 7, 11, 13, 17, 19, 23, 31, 43, 61, 79,
 * 101, 127, 167, 191, 199, 313, 347, 701, 1709, 2617, 3539, 5807, 10501, 10691, 11279, 12391,
 * 14479, 42737, 83339, 95369, 117239, 127031, 138937, 141079, 267017, 269987, 374321, 986191
 *
 * Compile with:  gcc -O2 wagstaff.c -o wagstaff -lgmp
 * Needs MPIR installed (http://www.mpir.org/)
 */

#include <stdio.h>
#ifdef USE_MPIR
#include "mpir.h"
#else
#include "gmp.h"
#endif
 

void wagstaff(int q)
{
	  int i;
	  mpz_t n,res,Q,R;
	  
	  mpz_init(res);
	  mpz_init(n);
	  mpz_init(Q);
	  mpz_init(R);
	  
	  #ifdef DEBUG
	  printf("Using q = %d\n", q);
	  #endif

	  mpz_set_ui(n,1);
	  mpz_mul_2exp(n,n,q);
	  mpz_add_ui(n,n,1);
	  
	  mpz_set_ui(res,6);
	  for(i=1;i<=q;i++)
	  {
	      mpz_pow_ui(res,res,2);
	      mpz_sub_ui(res,res,2);
	      if(mpz_sgn(res)<0)
			mpz_add(res,res,n);
	      mpz_fdiv_q_2exp(Q,res,q);
	      mpz_fdiv_r_2exp(R,res,q);
	      mpz_sub(res,R,Q);
	      if(mpz_sgn(res)<0)
			mpz_add(res,res,n);
	  }
	  
	  mpz_sub_ui(res,res,1154);
	  mpz_mul_ui(res,res,3);
	  while(mpz_sgn(res)<0)
		mpz_add(res,res,n);
	  while(mpz_cmp(res,n)>0)
		mpz_sub(res,res,n);
	  
	  #ifdef DEBUG
	  if(mpz_sgn(res)==0)
		printf("S(q)==S(2) mod n\n");
	  else
		printf("S(q)!==S(2) mod n\n");
	  #endif
	  
	  mpz_clear(res);
	  mpz_clear(n);
	  mpz_clear(Q);
	  mpz_clear(R);
	  return;
}

#if 0

int main(int argc, char *argv[])
{
	mpz_t mpz_q;
	unsigned long q;
	int bit_size;

	mpz_init_set_str(mpz_q, argv[1], 0);
	// check if input is prime, if not, start at next prime
	if (mpz_probab_prime_p(mpz_q, 10) == 0)
		mpz_nextprime(mpz_q, mpz_q);
	q = mpz_get_ui(mpz_q);

	// call bench function
	wagstaff(q);

	mpz_clear(mpz_q);
	return 0;
}

#endif
