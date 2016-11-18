/* mersenne_prime_p(k) return true iff kth Mersenne number is prime

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
#ifndef ASSERT_NOCARRY
#define ASSERT_NOCARRY(x)	(x)
#endif
#ifndef count_leading_zeros
#define count_leading_zeros(c,y)	do{mp_limb_t __c=0,__x=(y);while(__x!=0){__c++;__x/=2;}(c)=GMP_LIMB_BITS-__c;}while(0)
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
void	mpn_mulmod_2expm1 (mp_ptr xp,mp_ptr yp,mp_ptr zp,mp_size_t k2,mp_ptr tp)
{mpz_t x,y,z,m;mp_size_t n,tn;

n=BITS_TO_LIMBS(k2);
mpz_init2(y,k2);mpz_init2(z,k2);
mpz_init2(m,k2);mpz_init2(x,2*k2);
mpz_set_ui(m,1);mpz_mul_2exp(m,m,k2);mpz_sub_ui(m,m,1);
MPN_COPY(y->_mp_d,yp,n);tn=n;MPN_NORMALIZE(y->_mp_d,tn);y->_mp_size=tn;
MPN_COPY(z->_mp_d,zp,n);tn=n;MPN_NORMALIZE(z->_mp_d,tn);z->_mp_size=tn;
mpz_mul(x,y,z);
mpz_mod(x,x,m);tn=x->_mp_size;if(tn>n)tn=n;
MPN_COPY(xp,x->_mp_d,tn);if(tn<n)MPN_ZERO(xp+tn,n-tn);
mpz_clear(x);mpz_clear(y);mpz_clear(z);mpz_clear(m);
return;}
#endif



static int
isprime (unsigned long x)
{
  unsigned long d, dd;

  if (x == 2 || x == 3 || x == 5)
    return 1;
  if (x == 1 || x % 2 == 0 || x % 3 == 0)
    return 0;
  for (dd = 2, d = 5;; d += dd, dd = 6 - dd)
    {
      if (x % d == 0)
	return 0;
      if (x / d < d)
	return 1;
    }
}

/*
Lucas-Lehmer Test for k>=1 where k is odd prime

2^k-1 is prime if and only if V(k-2)==0 mod 2^k-1
where
v(0)=4		v(i)=v(i-1)^2-2
*/

// trial division(or sieving) would eliminate trial numbers MUCH faster
// This code is for benchmarking , ie how quick can we run a Lucas-Lehmer test , not how quick can we find Mersenne primes
// perhaps we should call it lucas_lehmer_p() ?
int
mersenne_prime_p (unsigned long k)
{
  int r, cc;
  unsigned long c, lg;
  mp_ptr xp, tp, rp, pp, sp;
  mp_size_t n;

  if (k < 2)
    return 0;
  if (k == 2)
    return 1;
  if (!isprime (k))
    return 0;
  n = BITS_TO_LIMBS (k);
  count_leading_zeros (lg, k);
  lg = BITS_PER_ULONG - lg;
  pp = __GMP_ALLOCATE_FUNC_LIMBS (7 * n + 5 * lg);
  xp = pp;			// have n limbs
  rp = pp + n;			// have n limbs
  tp = pp + 2 * n;		// have 5n+5lg(k)
  MPN_ZERO (xp, n);
  xp[0] = 4;
  for (c = 1; c <= k - 2; c++)
    {
      mpn_mulmod_2expm1 (rp, xp, xp, k, tp);	//    mpn_sqrmod_2expm1 would be faster
      cc = mpn_sub_1 (rp, rp, n, 2);
      ASSERT_NOCARRY (mpn_sub_1 (rp, rp, n, cc));
      sp = xp;
      xp = rp;
      rp = sp;
    }
// Although there are in general two representations of zero
// we can only have the obvious one here
  r = 1;
  for (c = 0; c < n; c++)
    if (xp[c] != 0)
      {
	r = 0;
	break;
      }
  __GMP_FREE_FUNC_LIMBS (pp, 7 * n + 5 * lg);
  return r;
}

#ifdef STANDALONE
int
main (int argc, char *argv[])
{
  int k, p;

  if (argc != 2)
    {
      printf ("Usage: %s k\nDisplays primality of M(k)\n", argv[0]);
      return 1;
    }
  k = atoi (argv[1]);
  p = mersenne_prime_p (k);
  printf ("The Mersenne number M(%d)=2^%d-1 is ", k, k);
  if (p == 0)
    printf ("not ");
  printf ("prime\n");
  return 0;
}
#endif
