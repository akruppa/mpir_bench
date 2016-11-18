/* fermat_prime_p(k) return true iff kth Fermat number is prime

Copyright 2009 Jason Moxham

This file is part of the MPIR Library.

The MPIR Library is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation; either version 2.1 of the License, or (at
your option) any later version.

The MPIR Library is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
License for more details.

You should have received a copy of the GNU Lesser General Public License
along with the MPIR Library; see the file COPYING.LIB.  If not, write
to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
Boston, MA 02110-1301, USA.
*/

#include <stdio.h>
#include <stdlib.h>
#ifdef USE_MPIR
#include "mpir.h"
#else
#include "gmp.h"
#endif

#define BITS_PER_ULONG	(8*sizeof(unsigned long))
#define BITS_TO_LIMBS(x)	(((x)+GMP_NUMB_BITS-1)/GMP_NUMB_BITS)
#ifndef MPN_ZERO
#define MPN_ZERO(xp,xn)	do{mp_size_t __i;for(__i=(xn)-1;__i>=0;__i--)(xp)[__i]=0;}while(0)
#endif
#ifndef __GMP_ALLOCATE_FUNC_LIMBS
#define __GMP_ALLOCATE_FUNC_LIMBS(x)	malloc((x)*sizeof(mp_limb_t))
#endif
#ifndef __GMP_FREE_FUNC_LIMBS
#define __GMP_FREE_FUNC_LIMBS(x,y)	free(x)
#endif

#ifndef __MPIR_VERSION
// we are gmp
#define NEED_MULMOD
#elif __MPIR_VERSION < 1 || (__MPIR_VERSION == 1 && __MPIR_VERSION_MINOR < 3)
#define NEED_MULMOD
#endif

#ifdef NEED_MULMOD
#define MPN_NORMALIZE(pp,pn)	do{while(pn>0 && pp[pn-1]==0)pn--;}while(0)
#include <string.h>
#define MPN_COPY(ap,bp,pn)	memcpy(ap,bp,pn*sizeof(ap[0]))

int	mpn_mulmod_2expp1 (mp_ptr xp,mp_ptr yp,mp_ptr zp,int c,mp_size_t k2,mp_ptr tp)
{mpz_t x,y,z,m;mp_size_t n,tn;int ret;

n=BITS_TO_LIMBS(k2);
mpz_init2(y,k2+1);mpz_init2(z,k2+1);
mpz_init2(m,k2+1);mpz_init2(x,2*k2+2);
mpz_set_ui(m,1);mpz_mul_2exp(m,m,k2);mpz_add_ui(m,m,1);
MPN_COPY(y->_mp_d,yp,n);tn=n;MPN_NORMALIZE(y->_mp_d,tn);y->_mp_size=tn;
if((c&2)!=0){mpz_add(y,y,m);mpz_sub_ui(y,y,1);}
MPN_COPY(z->_mp_d,zp,n);tn=n;MPN_NORMALIZE(z->_mp_d,tn);z->_mp_size=tn;
if((c&1)!=0){mpz_add(z,z,m);mpz_sub_ui(z,z,1);}
mpz_mul(x,y,z);
mpz_mod(x,x,m);tn=x->_mp_size;if(tn>n)tn=n;
MPN_COPY(xp,x->_mp_d,tn);if(tn<n)MPN_ZERO(xp+tn,n-tn);
ret=0;mpz_sub_ui(m,m,1);
if(mpz_cmp(x,m)>=0)ret=1;
mpz_clear(x);mpz_clear(y);mpz_clear(z);mpz_clear(m);
return ret;}
#endif

/*
Pepin's Test for k>=1

F_k = 2^(2^k)+1 is prime if and only if

3^((F_k-1)/2) == -1 mod F_k
*/

// should return true for k=0,1,2,3,4 and false for 5,...,32 
// tested upto k=17 (1m12s) k=18 (5m20s) on K8 1800Mhz with gmp-4.2 ?
// for k>23? then trial division would give an answer faster
// but this is really for benchmarks etc
// according to Prime Numbers , A computational Perspective , k=24 is the largest Pepin test ever run 2002
// Going from k to k+1 we expect the runtime to increase by a factor of 4+epsilon , So runtime=A*4^k
int
fermat_prime_p (unsigned long k)
{
  unsigned long i, k2;
  int c;
  mp_size_t n;
  mp_ptr tp, xp, yp, sp;

  if (k >= BITS_PER_ULONG)
    k = BITS_PER_ULONG - 1;	// this should force a out of memory rather than some sort of crash
  if (k == 0)
    return 1;
  k2 = 1;
  k2 <<= k;			// k2=2^k
// doing calcs mod F_k = 2^(2^k)+1= 2^k2+1 , so need k2 bits
  n = BITS_TO_LIMBS (k2);
  tp = __GMP_ALLOCATE_FUNC_LIMBS (4 * n);
  xp = tp;
  yp = tp + 2 * n;
  MPN_ZERO (xp, n);
  xp[0] = 3;
  c = 0;
  for (i = 1; i < k2; i++)
    {
      if (c != 0)
	c = 3;			// as we are squaring , dont need to do this , as for Pepin test it doesn't matter
      c = mpn_mulmod_2expp1 (yp, xp, xp, c, k2, yp);	// or better a mpn_sqrmod_2expp1
      sp = xp;
      xp = yp;
      yp = sp;
    }
  __GMP_FREE_FUNC_LIMBS (tp, 4 * n);
  // we could return the low limb of the computation as a check
  return c;
}

#ifdef STANDALONE
int
main (int argc, char *argv[])
{
  int k, p;

  if (argc != 2)
    {
      printf ("Usage: %s k\nDisplays primality of F(k)\n", argv[0]);
      return 1;
    }
  k = atoi (argv[1]);
  p = fermat_prime_p (k);
  printf ("The Fermat number F(%d)=2^(2^%d)+1 is ", k, k);
  if (p == 0)
    printf ("not ");
  printf ("prime\n");
  return 0;
}
#endif
