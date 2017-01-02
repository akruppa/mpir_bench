/*  MPIR Benchmark Program v0.6

    Copyright 2003 Free Software Foundation, Inc.

    Copyright 2009 Brian Gladman

    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2.1 of the GNU General Public License 
    as published by the Free Software Foundation; it is not distributable 
    under version 3 (or any later version) of the GNU General Public License.
    
    This program is distributed in the hope that it will be useful, 
    but WITHOUT ANY WARRANTY; without even the implied warranty of 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
    GNU General Public License for more details.
*/

#define WIN32_LEAN_AND_MEAN

#if defined( _MSC_VER )
#include <windows.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef USE_MPIR
#include "mpir.h"
#else
#include "gmp.h"
#endif

#ifdef _MSC_VER
#  include "win_timing.h"
#  if 1
#    define timer_start start_timing
#    define timer_stop  end_timing
#  else
#    define LOCAL_TIMER
#  endif
#else
#  include <sys/time.h>
#  include <sys/resource.h>
#  define LOCAL_TIMER
#endif

#if defined( LOCAL_TIMER )

static double _st;

void timer_start()
{
  struct rusage rus;
  getrusage (0, &rus);
  _st = rus.ru_utime.tv_sec + rus.ru_utime.tv_usec * 1.0e-6;
}

double timer_stop()
{
  struct rusage rus;
  getrusage (0, &rus);
  return rus.ru_utime.tv_sec + rus.ru_utime.tv_usec * 1.0e-6 - _st;
}

#endif

#define period  1000

#define CALIBRATE(res, fun) do {    \
    unsigned long i, rep = 1;	    \
    double t;                       \
    { fun; }				        \
    do  {				    		\
	    rep <<= 1;					\
	    timer_start();       	    \
	    for(i = 0 ; i < rep ; ++i)  \
	        { fun; }			  	\
        t = timer_stop();		    \
      }							    \
    while(t < 0.25);			    \
    res = 1000.0 * t / (double)rep; \
  } while (0)

double run_multiply(unsigned long long m,  unsigned long long n)
{
    unsigned long long i, rep;
    double f;
    mpz_t x, y, z;
    gmp_randstate_t rs;
    mpz_ptr xptr, yptr;

    gmp_randinit_default(rs);
    mpz_init(x);
    mpz_init(y);
    mpz_init(z);

    if(n == 0)
    {
        mpz_urandomb(x, rs, m);
        xptr = x;
        yptr = x;
    }
    else
    {
        mpz_urandomb(x, rs, m);
        mpz_urandomb(y, rs, n);
        xptr = x;
        yptr = y;
    }
  
    CALIBRATE(f, mpz_mul(z, xptr, yptr));
    rep = 1 + period / f;

    if(rep > 100)
    {
        rep = (rep + 3) & ~3;
        timer_start();
        for( i = rep >> 2 ; i > 0 ; --i )
        {
            mpz_mul(z, xptr, yptr);
            mpz_mul(z, xptr, yptr);
            mpz_mul(z, xptr, yptr);
            mpz_mul(z, xptr, yptr);
        }
        f = timer_stop();
    }
    else
    {
        timer_start();
        for( i = rep ; i > 0 ; --i )
        {
            mpz_mul(z, xptr, yptr);
        }
        f = timer_stop();
    }
    return rep / f;
}

double run_divide(unsigned long long m, unsigned long long n)
{
    unsigned long long i, rep;
    double f;
    mpz_t x, y, z;
    gmp_randstate_t rs;

    gmp_randinit_default(rs);

    mpz_init(x);
    mpz_init(y);
    mpz_init(z);
    mpz_urandomb(x, rs, m);
    mpz_urandomb(y, rs, n);

    CALIBRATE(f, mpz_tdiv_q (z, x, y));
    rep = 1 + period / f;

    if(rep > 63)
    {
        rep = (rep + 3) & ~3;
        timer_start();
        for( i = rep >> 2 ; i > 0 ; --i )
        {
            mpz_tdiv_q(z, x, y);
            mpz_tdiv_q(z, x, y);
            mpz_tdiv_q(z, x, y);
            mpz_tdiv_q(z, x, y);
        }
        f = timer_stop();
    }
    else
    {
        timer_start();
        for( i = rep ; i > 0 ; --i )
        {
        mpz_tdiv_q(z, x, y);
        }
        f = timer_stop();
    }
    return rep / f;
}

double run_gcd(unsigned long long m, unsigned long long n)
{
    gmp_randstate_t rs;
    mpz_t x, y, z;
    unsigned long long i, rep;
    double f;

    gmp_randinit_default(rs);
    mpz_init(x);
    mpz_init(y);
    mpz_init(z);
    mpz_urandomb(x, rs, m);
    mpz_urandomb(y, rs, n);

    CALIBRATE(f, mpz_gcd (z, x, y));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        mpz_gcd(z, x, y);
    }
    f = timer_stop();
    return rep / f;
}

double run_gcdext(unsigned long long m, unsigned long long n)
{
    gmp_randstate_t rs;
    mpz_t x, y, z, s;
    unsigned long long i, rep;
    double f;

    gmp_randinit_default(rs);
    mpz_init(x);
    mpz_init(y);
    mpz_init(z);
    mpz_init(s);
    mpz_urandomb(x, rs, m);
    mpz_urandomb(y, rs, n);

    CALIBRATE(f, mpz_gcdext (z, s, NULL, x, y));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        mpz_gcdext(z, s, NULL, x, y);
    }
    f = timer_stop();
    return rep / f;
}

double run_root(unsigned long long m, unsigned long long n)
{
    gmp_randstate_t rs;
    mpz_t x,  z;
    unsigned long long i, rep;
    double f;

    gmp_randinit_default(rs);
    mpz_init(x);
    mpz_init(z);
    mpz_urandomb(x, rs, m);

    CALIBRATE(f, mpz_root (z, x, n));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        mpz_root(z, x, n);
    }
    f = timer_stop();
    return rep / f;
}

double run_fac_ui(unsigned long long m, unsigned long long n)
{
    mpz_t x;
    unsigned long long i, rep;
    double f;

    mpz_init(x);
    
    CALIBRATE(f, mpz_fac_ui (x, m));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        mpz_fac_ui(x, m);
    }
    f = timer_stop();
    return rep / f;
}


#define RSA_EXP 0x10001

void rsa_sign(mpz_t smsg, mpz_t msg, mpz_t p, mpz_t q, mpz_t pq,
	                    mpz_t p_i_q, mpz_t dp, mpz_t dq)
{
    mpz_t  t, o, pr, qr, qr_m_pr;

    mpz_init(pr);
    mpz_init(qr);
    mpz_init(qr_m_pr);
    mpz_init(t);
    mpz_init(o);

    mpz_powm(pr, msg, dp, p);
    mpz_powm(qr, msg, dq, q);
    mpz_sub(qr_m_pr, qr, pr);
    mpz_mul(t, qr_m_pr, p_i_q);
    mpz_mod(o, t, q);		/* slow mod */
    mpz_mul(t, o, p);
    mpz_add(smsg, pr, t);
    mpz_mod(smsg, smsg, pq);	/* fast mod */

    mpz_clear(o);
    mpz_clear(t);
    mpz_clear(qr_m_pr);
    mpz_clear(qr);
    mpz_clear(pr);
}

double run_rsa(unsigned long long m, unsigned long long n)
{
    gmp_randstate_t rs;
    mpz_t p, q, pq, pm1, qm1, phi, e, d, p_i_q, dp, dq, msg[1024], smsg;
    unsigned long long i, rep;
    double f;

    gmp_randinit_default(rs);
    mpz_init(p);
    mpz_init(q);
    mpz_init(pq);

    mpz_urandomb(p, rs, m/2);
    mpz_setbit(p, m / 2 - 1);
    mpz_setbit(p, m / 2 - 2);
    mpz_nextprime(p, p);

    mpz_urandomb(q, rs, m/2);
    mpz_setbit(q, m / 2 - 1);
    mpz_setbit(q, m / 2 - 2);
    mpz_nextprime(q, q);

    mpz_mul(pq, p, q);

    mpz_init_set_ui(e, RSA_EXP);
    mpz_init(d);
    mpz_init(pm1);
    mpz_init(qm1);
    mpz_init(phi);

    mpz_sub_ui(pm1, p, 1);
    mpz_sub_ui(qm1, q, 1);
    mpz_mul(phi, pm1, qm1);
    if(mpz_invert(d, e, phi) == 0)
        abort();

    mpz_init (p_i_q);
    if(mpz_invert (p_i_q, p, q) == 0)
        abort();

    mpz_init(dp);
    mpz_init(dq);
    mpz_mod(dp, d, pm1);
    mpz_mod(dq, d, qm1);

    for (i = 0; i < 1024; i++)
    {
        mpz_init(msg[i]);
        mpz_urandomb(msg[i], rs, m);
    }
    
    mpz_init (smsg);

    CALIBRATE(f, rsa_sign(smsg, msg[0], p, q, pq, p_i_q, dp, dq));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        rsa_sign(smsg, msg[i % 1024], p, q, pq, p_i_q, dp, dq);
    }
    f = timer_stop();
    return rep / f;
}

void picomp(long int d, int out);

double run_pi(unsigned long long m, unsigned long long n)
{
    unsigned long long i, rep;
    double f;
      
    int out = 0;
    long int d = (long)m;

    CALIBRATE(f, picomp(d, out));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        picomp (d, out);
    }
    f = timer_stop();
    return rep / f;
}

#include "trn.h"

int iBPSW(mpz_t mpz_n, int iStrong)
{
    int iComp2;
    unsigned long ulDiv;

    /* First eliminate all N < 3 and all even N. */
    iComp2=mpz_cmp_si(mpz_n, 2);
    if(iComp2 < 0)
        return(0);
    if(iComp2==0)
        return(1);
    if(mpz_even_p(mpz_n))
        return(0);

    /* Check for small prime divisors p < 1000. */
    ulDiv=ulPrmDiv(mpz_n, 1000);
    if(ulDiv==1)
        return(1);
    if(ulDiv > 1)
        return(0);

    /* Carry out the Miller-Rabin test with base 2. */
    if(iMillerRabin(mpz_n, 2)==0)return(0);

    /* The rumored strategy of Mathematica could be imitated here by
     * performing additional Miller-Rabin tests. One could also
     * carry out one or more extra strong Lucas tests. See the
     * routine iPrP in trn.c for an implementation.
     *
     * Now N is a prime, or a base-2 strong pseudoprime with no prime
     * divisor < 1000. Apply the appropriate Lucas-Selfridge primality
     * test.
     */

    if(iStrong)
        return(iStrongLucasSelfridge(mpz_n));
    return(iLucasSelfridge(mpz_n));
}

void bpsw(mpz_t mpz_n)
{
    int prime, prime_bpsw, prime_sbpsw, prime_mr2, prime_ls, prime_sls, prime_xsl, prime_fermat2;
    mpz_t mpz_rem, mpz_two;

    mpz_init(mpz_rem);
    mpz_init_set_ui(mpz_two, 2);
    prime = (mpz_probab_prime_p(mpz_n, 13) > 0);
    prime_bpsw = iBPSW(mpz_n, 0);    /* standard Lucas-Selfridge test */
    prime_sbpsw = iBPSW(mpz_n, 1);   /* strong Lucas-Selfridge test */
    mpz_powm(mpz_rem, mpz_two, mpz_n, mpz_n);
    prime_fermat2 = (mpz_cmp_ui(mpz_rem, 2) == 0);
    prime_mr2 = iMillerRabin(mpz_n, 2);
    prime_ls = iLucasSelfridge(mpz_n);
    prime_sls = iStrongLucasSelfridge(mpz_n);
    prime_xsl = iExtraStrongLucas(mpz_n, 3);
}

double run_bpsw(unsigned long long m)
{
    gmp_randstate_t rs;
    mpz_t mpz_n;
    unsigned long long i, t, rep;
    double f;

    gmp_randinit_default(rs);
    mpz_init(mpz_n);
    mpz_urandomb(mpz_n, rs, m);

    CALIBRATE(f, bpsw(mpz_n));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        bpsw(mpz_n);
    }
    f = timer_stop();
    return rep / f;
}

void wagstaff(int q);

double run_wagstaff(unsigned long long q)
{
	mpz_t mpz_q;
	int bit_size;
    unsigned long long i, rep;
    double f;

	mpz_init_set_ui(mpz_q, q);
	// check if input is prime, if not, start at next prime
	if (mpz_probab_prime_p(mpz_q, 10) == 0)
		mpz_nextprime(mpz_q, mpz_q);
	q = mpz_get_ui(mpz_q);

    CALIBRATE(f, wagstaff(q));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        wagstaff(q);
    }
    f = timer_stop();

	mpz_clear(mpz_q);
    return rep / f;
}

double run_mersenne(unsigned long long m, unsigned long long n)
{
    int ret;
    unsigned long long i, t, rep;
    double f;

    CALIBRATE(f, mersenne_prime_p(m));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        ret=mersenne_prime_p(m);
    }
    f = timer_stop();
    return rep / f;
}

double run_fermat(unsigned long long m, unsigned long long n)
{
    int ret;
    unsigned long long i, t, rep;
    double f;

    CALIBRATE(f, fermat_prime_p(m));
    rep = 1 + period / f;

    timer_start();
    for( i = rep ; i > 0 ; --i )
    {
        ret=fermat_prime_p(m);
    }
    f = timer_stop();
    return rep / f;
}


void version()
{
  printf("gmp version: %s\n", gmp_version);
}

typedef struct 
{   unsigned long long a1, a2;
} pair;

pair mul_args[] = 
{
    { 128, 0 }, { 512, 0 }, { 8192, 0 }, { 131072, 0 }, { 2097152, 0 }, 
    { 128, 128 }, { 512, 512 }, { 8192, 8192 }, { 131072, 131072 },
    { 2097152, 2097152 }, { 15000, 10000 }, { 20000, 10000 },
    { 30000, 10000 }, { 16777216, 512 }, { 16777216, 262144 }, { 0, 0 }
};

pair div_args[] = 
{
    { 8192, 32 }, { 8192, 64 }, { 8192, 128 }, { 8192, 4096 }, { 131072, 65536 },
    { 8388608, 4194304 }, { 8192, 8064 }, { 16777216, 262144 }, { 0, 0 }
};

pair gcd_args[] =
{
    { 128, 128 }, { 512, 512 }, { 8192, 8192 }, { 131072, 131072 }, { 1048576, 1048576 }, { 0, 0 }
};

pair gcdext_args[] =
{
    { 128, 128 }, { 512, 512 }, { 8192, 8192 }, { 131072, 131072 }, { 1048576, 1048576 }, { 0, 0 }
};

pair root_args[] =
{
    { 128, 5 }, { 512, 3 }, { 8192, 11 }, { 131072, 3 }, { 1048576, 3 }, { 0, 0 }
};

pair fac_ui_args[] =
{
    { 128, 0 }, { 1512, 0 }, { 15000, 0 }, { 1000010, 0 }, { 2123456, 0 }, { 0, 0 }
};

pair rsa_args[] = 
{
    { 512, 0 }, { 1024, 0 }, { 2048, 0 }, { 0, 0 }
};

pair pi_args[] = 
{
    { 10000, 0 }, { 100000, 0 }, { 1000000, 0 }, { 0, 0 }
};

pair bpsw_args[] = 
{
    { 1024, 0 }, { 4096, 0 }, { 16384, 0 }, { 0, 0 }
};

pair wagstaff_args[] = 
{
    { 1024, 0 }, { 4096, 0 }, { 16384, 0 }, { 0, 0 }
};

pair mersenne_args[] =
{
     { 3217, 0 }, { 4253, 0 }, { 4423, 0 } , { 9689, 0 }, { 11213, 0 }, { 0, 0 }
};

pair fermat_args[] =
{
    { 8, 0 }, { 10, 0 }, { 12, 0 }, { 0, 0 }
};



typedef double (*fptr)(unsigned long long, unsigned long long);

typedef struct
{   char *name;
    fptr fp;
    int  npar;
    pair *a_ptr;
    double  wght;
} scat_str;

typedef struct 
{
    char    *name;
    scat_str  sc_arr[7];
} cat_str;

cat_str cc_str[] = 
{ 
    { "base",
        { 
            { "multiply", run_multiply, 2, mul_args, 1.0  },
            { "divide", run_divide, 2, div_args, 1.0 },
            { "gcd", run_gcd, 2, gcd_args, 0.5 },
            { "gcdext", run_gcdext, 2, gcdext_args, 0.5 },
            { "root", run_root, 2, root_args, 0.3 }, 
            { "fac_ui", run_fac_ui, 2, fac_ui_args, 0.2 }, 
            { 0 }
        }
    },
    {   "app",
        {
            { "rsa", run_rsa, 1, rsa_args, 1.0 },
            { "pi", run_pi, 1, pi_args, 1.0 },
            { "bpsw", run_bpsw, 1, bpsw_args, 1.0 },
            { "wagstaff", run_wagstaff, 1, wagstaff_args, 1.0 },
            { "mersenne", run_mersenne, 1, mersenne_args, 1.0 },
            { "fermat", run_fermat, 1, fermat_args, 1.0 },
            

            { 0 }
        }
    },
    { 0 }
};

void out_res(double r, int wdth, double cps)
{   double f;
    int i;    
    
    if(r != 0.0)
        for( i = 0, f = 100.0; ; ++i )
        {
            if(r > f)
	            break;
            f *= 0.1;
        }
    printf(" => %*.*f", wdth, i, r);

    if(cps != 0.0)
    {
        r = r / (1.0e-9 * cps);
        if(r != 0.0)
            for( i = 0, f = 100.0 ; ; ++i )
            {
                if(r > f)
	                break;
                f *= 0.1;
            }
        printf(",%*.*f", wdth, i, r);
    }
}

#if defined( linux )

int get_processor_info(char *cpu_id, char *cpu_name, double  *cycles_per_second)
{
    FILE *fp;
    char buf[512], vendor_id[256], *p;
    int model = 0, family = 0, stepping = 0;
    double cpu_MHz = 0.0;

    fp = fopen("/proc/cpuinfo", "rt");
    if(fp == 0)
        return EXIT_FAILURE;

    while(fgets(buf, 512, fp) != 0)
    {
        p = strstr(buf,":");
        if(p == 0)
            continue;
        if(strstr(buf, "cpu family") != 0)
        {
            family = atoi(p + 2);
        }
        else if(strstr(buf, "model name") != 0)
        {
            strcpy(cpu_name, p + 2);
            cpu_name[strlen(cpu_name) - 1] = 0;
        }
        else if(strstr(buf, "model") != 0)
        {
            model = atoi(p + 2);
        }
        else if(strstr(buf, "stepping") != 0 )
        {
            stepping = atoi(p + 2);
        }
        else if(strstr(buf, "vendor_id") != 0)
        {
            strcpy(vendor_id, p + 2);
            vendor_id[strlen(vendor_id) - 1] = 0;
        }
        else if(strstr(buf, "cpu MHz") != 0)
        {
            cpu_MHz = atof(p + 2);
        }
    }

    fclose(fp);
    sprintf(cpu_id, "%s Family %d Model %d Stepping %d", vendor_id, family, model, stepping);
    *cycles_per_second = 1.0e6 * cpu_MHz;
    return EXIT_SUCCESS;
}

#endif

#if ! defined( _MSC_VER )
#define _MAX_PATH	1024
#endif

int main(void)
{   double r, v, acc, acc1, acc2, n, n1, n2, cps, mcps;
    pair   *pars;
    cat_str  *cp;
    scat_str *scp;
    char id_bfr[_MAX_PATH], n_bfr[_MAX_PATH];

    printf("\nRunning MPIR benchmark");

#if defined( linux ) || defined( _MSC_VER ) 
    get_processor_info(id_bfr, n_bfr, &cps);
    printf("\n%s", id_bfr);
    printf("\n%s", n_bfr);
#endif

#if defined( _MSC_VER )
    set_timing_seconds();
    speed_time_init();
    printf("\nSpeed: %.2f GHz (reported), %.2f GHz (measured)", 1.0e-9 * cps, 1.0e-9 / seconds_per_cycle);
#else
    printf("\nSpeed: %.2f GHz (reported)", 1.0e-9 * cps);
#endif
    acc2 = 1.0;
    n2   = 0.0;
    for( cp = cc_str ; cp->name ; ++cp )
    {
        printf("\n Category %s", cp->name);
        acc1 = 1.0;
        n1   = 0.0;
        for( scp = cp->sc_arr; scp->name ; ++scp )
        {
            printf("\n  Program %s (weight %.2f)", scp->name, scp->wght);
            acc = 1.0;
            n   = 0.0;
            for( pars = scp->a_ptr ; pars->a1 ; ++pars )
            {
                if(scp->npar == 1)
                {
                    r = (scp->fp)(pars->a1, 0);
                    printf("\n             %9llu", pars->a1);
                }
                else if(scp->npar == 2 && pars->a2 == 0)
                {
                    r = (scp->fp)(pars->a1, pars->a2);
                    printf("\n   %9llu %9llu", pars->a1, pars->a1);
                }
                else
                {
                    r = (scp->fp)(pars->a1, pars->a2);
                    printf("\n   %9llu %9llu", pars->a1, pars->a2);
                }
                out_res(r, 8, 0.0);
                acc *= r;
                n += 1.0;
            }
            v = pow(acc, 1.0 / n);
            out_res(v, 5, cps);
            acc1 *= pow(v, scp->wght);
            n1 += scp->wght;
        }
        v = pow(acc1, 1.0 / n1);
        out_res(v, 5, cps);
        acc2 *= v;
        n2 += 1.0;
    }
    out_res(pow(acc2, (1.0 / n2)), 5, cps);
    printf("\n\n");
}
