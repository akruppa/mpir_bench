/* trn.h                 Thomas R. Nicely          2009.05.24.1145
 *                    http://www.trnicely.net
 *
 * Freeware copyright (c) 2009 Thomas R. Nicely
 * <http://www.trnicely.net>. No warranties expressed or implied.
 * Distributed under the terms of the GNU GPL and GNU FDL licenses;
 * see <http://www.gnu.org/licenses/licenses.html>. Source,
 * binaries, and license terms available upon request.
 *
 * Header for custom library routines written by Thomas R. Nicely.
 * The source code is in trn.c. It is assumed the compiler and
 * libraries have __non-trivial__ support for the long double
 * data type, and that the processor is Intel 386 compatible
 * (the macro __i386__ is defined).
 *
 * The macro __DJGPP__ is used to detect DJGPP (v2.03 and v2.04).
 * The macro __DMC__ is used to detect 32-bit Digital Mars.
 * The macro __LINUX__ is used to detect SUSE GNU/Linux 10.x.
 * The macro __CYGWIN__ is used to detect CygWin.
 * The macro __MINGW__ is used to detect MinGW.
 * The macro __BORLANDC__ is used to detect Borland/Inprise C.
 * The macro __MSVC__ is used to detect Microsoft Visual C++.
 * The macro __WIN32__ is used to detect the Win32 API, which
 *     includes CygWin, MinGW, Digital Mars in its default mode,
 *     and Borland C in its default mode.
 *
 * The macro __MPIR__ indicates support for GMP, the GNU Multiple
 * Precision library. It is assumed to be present on all platforms
 * except Digital Mars, Borland C, and MSVC. To compile without GMP
 * on other platforms, add the command-line parameter "-D__NOGMP__"
 * (or the equivalent). If you _do_ have GMP with Digital Mars,
 * Borland C, or MSVC, activate the directive below defining
 *  __MPIR__---and send me a copy of your libgmp files!
 *
 * The macro __MPFR__ indicates support for MPFR, the GNU Multiple
 * Precision Floating-point library with reliable rounding, version
 * 2.2.1 or later. It is assumed to be present on all platforms
 * except DJGPP, Digital Mars, Borland C, and MSVC. To compile
 * without MPFR on other platforms, add the command-line parameter
 * "-D__NOMPFR__" (or the equivalent). If you _do_ have MPFR with
 * DJGPP, Digital Mars, Borland C, or MSVC, activate the directive
 * below defining  __MPFR__---and send me a copy of your libmpfr
 * files!
 *
 * Support for platforms other than DJGPP is at an "early beta"
 * level. Support for MSVC is at alpha level.
 *
 * If support for DJGPP/Borland C style conio console functions
 * is required for compilers and platforms other than DJGPP and
 * Borland C, #include conio3.h and compile and link conio3.c (q.v.).
 *
 * MinGW and CygWin codes are targeted to run in an ordinary Windows
 * DOS box, _not_ within the MSYS/CygWin UNIX emulation environments,
 * where the executables exhibit different behavior. Note that CygWin
 * code compiled with the -mno-cygwin option exhibits its own
 * eccentricities, different from those of standalone CygWin or
 * standalone MinGW.
 *
 */

#ifndef _TRN_H_
#define _TRN_H_ 1

/**********************************************************************/
#ifndef _CONIO3_H_  /* avoid duplication */
/**********************************************************************/

#if defined(__linux__) && !defined(__i386__) && !defined(__x86_64__)
  #error ...Intel/AMD 386 compatible processor (with FPU) required...
#endif

#ifndef _GNU_SOURCE
  #define _GNU_SOURCE 1
#endif
#ifndef __USE_GNU
  #define __USE_GNU 1
#endif
#ifndef _ISOC99_SOURCE
  #define _ISOC99_SOURCE 1
#endif

#undef __DJ203__
#undef __DJ204__
#if defined(__DJGPP__) && __DJGPP__==2 && defined(__DJGPP_MINOR__)
  #if __DJGPP_MINOR__ == 3
    #define __DJ203__ 203
  #elif __DJGPP_MINOR__ == 4
    #define __DJ204__ 204
  #endif
#endif

#undef __LINUX__
#undef __SUSE__
#undef __SUSE10__
#ifdef __linux__
  #define __LINUX__ 1
  #define __SUSE__ 1
  #define __SUSE10__ 1
#endif

#undef __MINGW__
#ifdef __MINGW32__
  #define __MINGW__ 1
#endif

#undef __DM__
#ifdef __DMC__
  #define __DM__ 1
#endif

#undef __MSVC__
#ifdef _MSC_VER
  #define __MSVC__ 1
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <ctype.h>
#include <signal.h>
#include <sys/stat.h>
#if defined(__DJ203__)
  typedef long long int64_t;
  typedef unsigned long long uint64_t;
  typedef long int32_t;
  typedef unsigned long uint32_t;
  typedef char int8_t;
  typedef unsigned char uint8_t;
#elif defined(__BORLANDC__) || defined(__MSVC__)
  typedef __int64 int64_t;
  typedef unsigned __int64 uint64_t;
  typedef long int32_t;
  typedef unsigned long uint32_t;
  typedef char int8_t;
  typedef unsigned char uint8_t;
#else
  #include <stdint.h>
#endif
#ifdef __MSVC__ 
#define _USE_MATH_DEFINES
#define log2l(x) (M_LOG2E * log(x))
#endif
#include <math.h>
#include <float.h>
#include <limits.h>
#include <sys/stat.h>

#if !(defined( __BORLANDC__ ) || defined( __MSVC__ ))
  #include <sys/time.h>
#endif
#ifdef __LINUX__
  #include <sys/ioctl.h>
  #include <sys/sysinfo.h>
  #include <sys/mman.h>
  #include <termios.h>
#endif
#if !(defined(__DMC__) || defined(__BORLANDC__) || defined( __MSVC__ ))
  #include <unistd.h>
#endif
#ifdef __DJGPP__
  #include <io.h>
  #include <conio.h>
  #include <dos.h>
  #include <dpmi.h>
  #include <sys/mman.h>
#endif
#ifdef __BORLANDC__
  #include <io.h>
  #include <conio.h>
  #include <dos.h>
#endif
#ifdef __DMC__
  #include <io.h>
  #include <conio.h>
  #include <disp.h>
  #include <dos.h>
#endif
#ifdef __MINGW__
  #include <io.h>
  #include <conio.h>
  #include <dos.h>
#endif

#if defined(__WIN32__) || defined(_WIN32) || defined(__CYGWIN__) \
    || defined(__MINGW__) || defined(__MSVC__)
  #undef __WIN32__
  #define __WIN32__ 1
  #include <windows.h>
  #include <winbase.h>
#define S_ISREG(x)  ((x & _S_IFMT) == _S_IFREG) 
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define S_IXUSR _S_IEXEC
#define strtoull _strtoui64
#endif

/**********************************************************************/
#endif  /* not _CONIO3_H_ */
/**********************************************************************/

/* Alternate printf format specifiers for 64-bit integers, employing
   the syntax specified in the C0x draft of 6 May 2005. NOTE: The
   '%' prefixes have been removed to conform with the proposed
   standard. */

#undef PRId64
#undef PRIu64
#undef PRIx64
#undef PRIdMAX
#undef PRIuMAX
#undef PRIxMAX

#if defined(__MINGW__) || defined(__MSVC__) || defined(__BORLANDC__)
  #define PRId64 "I64d"
  #define PRIu64 "I64u"
  #define PRIx64 "I64x"
#else
  #define PRId64 "lld"
  #define PRIu64 "llu"
  #define PRIx64 "llx"
#endif

#define PRIdMAX PRId64
#define PRIuMAX PRIu64
#define PRIxMAX PRIx64

/* Alternate specifiers for 64-bit integer literal constants, to
   account for differing suffixes in various compilers. Thus
   __LL(-123456789123456789) will expand to the correct long long
   literal constant for the integer -123456789123456789 on any of
   the supported platforms. */

#ifdef __BORLANDC__
  #define __LL(n) n##i64
  #define __ULL(n) n##ui64
#else
  #define __LL(n) n##LL
  #define __ULL(n) n##ULL
#endif

#if defined(__DJGPP__) || defined (__BORLANDC__)
  #include <conio.h>
#elif defined(__DMC__)
  #include <conio.h>
  #include <disp.h>
#endif

/**************************** GMP support ****************************/

/* Change the following "#if 0" to "#if 1" if you do not have or do not
   want GMP support. */

#if 0
  #define __NOGMP__ 1
  #undef  __MPIR__
#endif

/* The following constitutes the default assumption about GMP support. */

#undef  __MPIR__
#ifndef __NOGMP__
  #if defined(__DJGPP__) || defined(__LINUX__) || defined(__CYGWIN__) \
      || defined(__MINGW__)
    #define __MPIR__ 1
    #ifdef USE_MPIR
    #include "mpir.h"
    #else
    #include "gmp.h"
    #endif
  #endif
#endif  /* not NOGMP */

/* Change the following "#if 0" to "#if 1" if you _do_ have GMP support
   in Digital Mars, Borland C, or MSVC---in which case please send me a
   copy of your libgmp and libgmpxx files! */

#if 1
  #undef  __NOGMP__
  #define __MPIR__ 1
  #ifdef USE_MPIR
  #include "mpir.h"
  #else
  #include "gmp.h"
  #endif
#endif

/**************************** MPFR support ***************************/

/* Change the following "#if 0" to "#if 1" if you do not have or do not
   want MPFR support (version 2.2.1 or later). */

#if 1
  #define __NOMPFR__ 1
  #undef  __MPFR__
#endif

/* The following constitutes the default assumption about MPFR support. */

#undef __MPFR__
#if !defined(__NOMPFR__) && defined(__MPIR__)
  #if defined(__LINUX__) || defined(__CYGWIN__) || defined(__MINGW__)
    #define __MPFR__ 1
    #include <mpfr.h>
  #endif
#endif  /* not NOMPFR */

/* Change the following "#if 0" to "#if 1" if you _do_ have MPFR 2.2.1+
   support in DJGPP, Digital Mars, Borland C, or MSVC---in which case
   please send me a copy of your libmpfr files! */

#if 0
  #undef  __NOMPFR__
  #define __MPFR__ 1
  #include <mpfr.h>
#endif

/* MPFR version must be 2.2.1 or later. */

#if !defined(MPFR_VERSION_NUM)  /* works around DJGPP preprocessor bug */
  #define MPFR_VERSION_NUM(a,b,c) 0
#endif

#if !defined(__MPIR__) || !defined(MPFR_VERSION) || \
    (MPFR_VERSION < MPFR_VERSION_NUM(2,2,1))
  #undef  __MPFR__
  #define __NOMPFR__ 1
#endif

/**********************************************************************/
/******************** MANIFEST CONSTANTS CORRECTED ********************/
/**********************************************************************/

/* Redefine the manifest constants in math.h to properly implement
   long double precision. This fixes an oversight (or a misguided
   commitment to 53-bit double precision) in many versions of math.h. */

/* From GNU C 4.12 */

#undef M_E             /*  e           */
#undef M_LOG2E         /*  log_2(e)    */
#undef M_LOG10E        /*  log_10(e)   */
#undef M_LN2           /*  ln(2)       */
#undef M_LN10          /*  ln(10)      */
#undef M_PI            /*  pi          */
#undef M_PI_2          /*  pi/2        */
#undef M_PI_4          /*  pi/4        */
#undef M_1_PI          /*  1/pi        */
#undef M_2_PI          /*  2/pi        */
#undef M_2_SQRTPI      /*  2/sqrt(pi)  */
#undef M_SQRT2         /*  sqrt(2)     */
#undef M_SQRT1_2       /*  sqrt(1/2)   */
#undef PI              /*  in accordance with C99 */
#undef PI2             /*  in accordance with C99 */

/* Corrected values are given to 50 decimal places (at least 165-bit
   precision). */

#define M_E             2.71828182845904523536028747135266249775724709369996L
#define M_LOG2E	        1.44269504088896340735992468100189213742664595415299L
#define M_LOG10E        0.43429448190325182765112891891660508229439700580367L
#define M_LN2           0.69314718055994530941723212145817656807550013436026L
#define M_LN10          2.30258509299404568401799145468436420760110148862877L
#define M_PI            3.14159265358979323846264338327950288419716939937511L
#define M_PI_2          1.57079632679489661923132169163975144209858469968755L
#define M_PI_4          0.78539816339744830961566084581987572104929234984378L
#define M_1_PI          0.31830988618379067153776752674502872406891929148091L
#define M_2_PI          0.63661977236758134307553505349005744813783858296183L
#define M_2_SQRTPI      1.12837916709551257389615890312154517168810125865800L
#define M_SQRT2         1.41421356237309504880168872420969807856967187537695L
#define M_SQRT1_2       0.70710678118654752440084436210484903928483593768847L

/* From Borland C++ 4.52 */

#undef M_1_SQRTPI      /*  1/sqrt(pi)  */
#undef M_SQRT_2        /*  sqrt(1/2)   */

#define M_1_SQRTPI      0.56418958354775628694807945156077258584405062932900L
#define M_SQRT_2        M_SQRT1_2

/* From ... Microsoft C ? */

#undef M_SQRT3         /*  sqrt(3)     */
#undef M_TWOPI         /*  2*pi        */
#undef M_3PI_4         /*  3*pi/4      */
#undef M_SQRTPI        /*  sqrt(pi)    */
#undef M_LOGE          /*  log_10(e)   */
#undef M_IVLN10        /*  1/ln(10)    */
#undef M_LOG2_E        /*  ln(2)       */
#undef M_INVLN2        /*  1/ln(2)     */
#undef M_LOG2          /*  log_10(2)   */

#define	M_SQRT3         1.73205080756887729352744634150587236694280525381038L
#define	M_TWOPI         6.28318530717958647692528676655900576839433879875021L
#define	M_3PI_4         2.35619449019234492884698253745962716314787704953133L
#define	M_SQRTPI        1.77245385090551602729816748334114518279754945612239L
#define M_LOGE          M_LOG10E
#define	M_IVLN10        M_LOG10E
#define	M_LOG2_E        M_LN2
#define	M_INVLN2        M_LOG2E
#define M_LOG2          0.30102999566398119521373889472449302676818988146211L

/* Nicely constants */

#undef M_LOG_2_BASE10
#undef M_LOG_E_BASE2
#undef M_LOG_E_BASE10
#undef M_LI2
#undef M_C2
#undef M_C3
#undef M_C4
#undef M_HL2
#undef M_HL3
#undef M_HL4
#undef M_ACC2
#undef M_ACC3
#undef M_ACC4
#undef M_LN2_SQUARED
#undef M_LN2_CUBED

/* The constants M_LI2, M_C2, M_C3, M_C4, M_HL2, M_HL3, M_HL4, M_ACC2,
   M_ACC3, and M_ACC4 appear in calculations involving the
   Hardy-Littlewood integrals, twin primes, prime triplets, and
   prime quadruplets. */

#define M_LOG_2_BASE10  M_LOG2
#define M_LOG_E_BASE2   M_LOG2E
#define M_LOG_E_BASE10  M_LOG10E
#define M_LI2  1.045163780117492784844588889194613136522615578L /* Li(2) */
#define M_C2   0.660161815846869573927812110014555778432623360L /* twins */
#define M_C3   0.635166354604271207206696591272522417342065687L /* trplts */
#define M_C4   0.307494878758327093123354486071076853022178520L /* quads */
#define M_HL2  1.320323631693739147855624220029111556865246721L
#define M_HL3  2.858248595719220432430134660726350878039295593L
#define M_HL4  4.151180863237415757165285561959537515799410019L
#define M_ACC2 2.640647263387478295711248440058223113730493441L
#define M_ACC3 4.287372893578830648645201991089526317058943389L
#define M_ACC4 5.534907817649887676220380749279383354399213359L
#define M_LN2_SQUARED 0.48045301391820142466710252632666497173055295159455L
#define M_LN2_CUBED   0.33302465198892947971885358261173054415612648534861L

/* Long double constants such as, and including those defined in
   GNU extensions. */

#undef  M_El
#undef  M_LOG2El
#undef  M_LOG10El
#undef  M_LN2l
#undef  M_LN10l
#undef  M_PIl
#undef  M_PI_2l
#undef  M_PI_4l
#undef  M_1_PIl
#undef  M_2_PIl
#undef  M_2_SQRTPIl
#undef  M_SQRT2l
#undef  M_SQRT1_2l
#undef  M_1_SQRTPIl
#undef  M_SQRT_2l
#undef 	M_SQRT3l
#undef 	M_TWOPIl
#undef 	M_3PI_4l
#undef 	M_SQRTPIl
#undef  M_LOGEl
#undef 	M_IVLN10l
#undef 	M_LOG2_El
#undef 	M_INVLN2l
#undef  M_LOG2l
#undef  M_LOG_2_BASE10l
#undef  M_LOG_E_BASE2l
#undef  M_LOG_E_BASE10l
#undef  M_LN2_SQUAREDl
#undef  M_LN2_CUBEDl

#define M_El		M_E
#define M_LOG2El        M_LOG2E
#define M_LOG10El       M_LOG10E
#define M_LN2l		M_LN2
#define M_LN10l         M_LN10l
#define M_PIl		M_PI
#define M_PI_2l	        M_PI_2
#define M_PI_4l         M_PI_4
#define M_1_PIl         M_1_PI
#define M_2_PIl         M_2_PI
#define M_2_SQRTPIl     M_2_SQRTPI
#define M_SQRT2l        M_SQRT2
#define M_SQRT1_2l      M_SQRT1_2
#define M_1_SQRTPIl     M_1_SQRTPI
#define M_SQRT_2l       M_SQRT_2
#define	M_SQRT3l        M_SQRT3
#define	M_TWOPIl        M_TWOPI
#define	M_3PI_4l        M_3PI_4
#define	M_SQRTPIl       M_SQRTPI
#define M_LOGEl         M_LOGE
#define	M_IVLN10l       M_IVLN10
#define	M_LOG2_El       M_LOG2_E
#define	M_INVLN2l       M_INVLN2
#define M_LOG2l         M_LOG2
#define M_LOG_2_BASE10l M_LOG2_BASE10
#define M_LOG_E_BASE2l  M_LOG_E_BASE2
#define M_LOG_E_BASE10l M_LOG_E_BASE10
#define M_LI2l          M_LI2
#define M_LN2_SQUAREDl  M_LN2_SQUARED
#define M_LN2_CUBEDl    M_LN2_CUBED

/* Some compilers declare invalid values for some of the following
   manifest constants. */

#ifndef CHAR_BIT
  #define CHAR_BIT 8
#endif

#undef INT32_MIN
#define INT32_MIN (-2147483647L - 1L)

#undef INT32_MAX
#define INT32_MAX   2147483647L

#undef UINT32_MAX
#define UINT32_MAX  4294967295UL

#undef INT64_MIN
#undef INT64_MAX
#undef UINT64_MAX
#ifdef __BORLANDC__
  #define INT64_MIN  (-9223372036854775807i64 - 1i64)
  #define INT64_MAX    9223372036854775807i64
  #define UINT64_MAX  18446744073709551615ui64
#else
  #define INT64_MIN  (-9223372036854775807LL - 1LL)
  #define INT64_MAX    9223372036854775807LL
  #define UINT64_MAX 18446744073709551615ULL
#endif

/**********************************************************************/
/****************** END MANIFEST CONSTANTS CORRECTED ******************/
/**********************************************************************/

/* Miscellaneous defines, including the maximum number of decimal digits
   for which mpz_t storage is to be reserved, and the corresponding
   maximum number of bits; __MAX_BITS__=ceil(__MAX_DIGITS__/log_10(2)). */

#undef __MAX_DIGITS__
#undef __MAX_BITS__

#define __MAX_DIGITS__ 1001000UL
#define __MAX_BITS__   (3*__MAX_DIGITS__ + __MAX_DIGITS__/3 + 1)

#define  NUM_16BIT_PRIMES    6542
#define  NUM_32BIT_PRIMES    203280221UL

/**********************************************************************/
/*********************** GMP BASED ROUTINES ***************************/
/**********************************************************************/

#ifdef __MPIR__

/* Custom GMP routines */

int                 __mpz_set_str(mpz_t mpz, char *sz, int iBase);
uint64_t            __mpz_get_ull(mpz_t mpz);
void                __mpz_set_ull(mpz_t mpz, uint64_t ull);
long double         __mpz_get_ld(mpz_t mpz);
void                __mpz_set_ld(mpz_t mpz, long double ld);
int                 __mpz_cmp_ld(mpz_t mpz, long double ld);
long double         __mpz_log10l(mpz_t mpz);
long double         __mpz_logl(mpz_t mpz);
void                __mpz_powl(mpz_t mpz, long double ldBase,
                      long double ldExp);
void                __mpz_expl(mpz_t mpz, long double ldExp);
int                 __mpf_set_str(mpf_t mpf, char *sz, int iBase);
long double         __mpf_get_ld(mpf_t mpf);
void                __mpf_set_ld(mpf_t mpf, long double ld);
void                __mpf_set_ld2(mpf_t mpf, long double ld);

/* Prime number generation and testing using GMP */

int     iPrP(mpz_t mpzN, unsigned long ulNMR, unsigned long ulMaxDivisor);
int     iIsPrime64(uint64_t ullN, unsigned long ulMaxDivisor);
unsigned long ulPrmDiv(mpz_t mpzN, unsigned long ulMaxDivisor);
int     iMillerRabin(mpz_t mpzN, unsigned long ulB);
int     iLucasSelfridge(mpz_t mpzN);
int     iStrongLucasSelfridge(mpz_t mpzN);
int     iExtraStrongLucas(mpz_t mpzN, long lB);
int64_t sllLML(uint64_t ullx);  /* pi(x) using LML algorithm; see lml.c */

/* Expression parser for mpz bigints. iEvalExpr and iParseMPZ are
   deprecated identifiers. Uses GMP. */

int     iEvalExprMPZ(mpz_t mpzResult, char *szExpression);
#define iEvalExpr iEvalExprMPZ
#define iParseMPZ iEvalExprMPZ

#endif /* __MPIR__ */

/**********************************************************************/
/********************* END GMP BASED ROUTINES *************************/
/**********************************************************************/

/* Routines for analyzing prime gap records */

int         iRecordValidExt(char *sz);
int         iGetGapRecExt(char *szGapRec, FILE *fpIn);
void        vGapContExt(char *szContRec, char *szGapRec);
int         iGetGapRec(char *szGapRec, FILE *fpIn);
int         iRecordValid(char *szRec);
void        vGapCont(char *szContRec, char *szGapRec);

/* Prime number generation and testing without GMP */

void    vGenPrimesDiv(unsigned long *ulPrime, unsigned long *nPrimes,
	  unsigned long *ulUB);
#define vGenPrimes vGenPrimesDiv
void    vGenPrimes16(void);
void    vGenPrimesSieve(unsigned long *ulPrime, unsigned long *nPrimes,
	  unsigned long *ulUB);
void    vSieve(unsigned char *uchSieve, unsigned long *ulLB,
	  unsigned long *ulUB, unsigned long *ulPrime);
int     iIsPrime32(unsigned long ulN);

/* Functions returning (for x >= 2) Li(x); the Hardy-Littlewood integral
   approximations for the counts of twin primes, triplets, and
   quadruplets; Riemann's prime counting function R(x); and Riemann's
   zeta function. */

long double ldLogInt(long double ldx, long double *ldHL2,
  long double *ldHL3, long double *ldHL4);
long double ldLi(long double ldx);
long double ldRPCF(long double ldx);
void vDefineZetaArray(void);
long double ldZeta(long double ldx);

/* String editing.  MSVC equivalents unknown. */

char *szTrimMWS(char *pch);
char *szTrimTWS(char *pch);
char *szTrimLWS(char *pch);
char *__strlwr(char *sz);
char *__strupr(char *sz);

#ifdef __DJGPP__
  #define strcmpi     strcasecmp
  #define strncmpi    strncasecmp
#endif

#ifdef __LINUX__
  #define stricmp     strcasecmp
  #define strnicmp    strncasecmp
  #define strcmpi     strcasecmp
  #define strncmpi    strncasecmp
  #define strlwr      __strlwr
  #define strupr      __strupr
#endif

#ifdef __DMC__
  #define strcasecmp  stricmp
  #define strncasecmp strnicmp
#endif

/* Miscellaneous */

#ifndef SIGTERM
  #define SIGTERM  SIGINT
#endif
#ifndef SIGBREAK
  #define SIGBREAK SIGINT
#endif
#ifndef SIGQUIT
  #define SIGQUIT  SIGINT
#endif
#ifndef SIGKILL
  #define SIGKILL  SIGINT
#endif

unsigned long  __ulPhysicalMemoryAvailable(void);
int            iSignum(long double ldArg);
void           vFlush(void);
void           __vSleep(double lfSeconds);
int            __iXOpen(FILE **fpp, char *szFullName,
		 const char *szBaseName, const char *szMode);
long           __lFile(char *szFileName);
long           __lRFile(char *szFileName);
long           __lRWFile(char *szFileName);
double         lfSeconds2(void);
unsigned long  ulSqrt(uint64_t ull);
void           vAtExit(void);

#undef  _iSignum
#define _iSignum                     iSignum

/* Be aware of unexpected side effects with the following macros;
   volatile arguments in a and b may change values across instances. */

#undef __MIN2
#define __MIN2(a,b) (((a) < (b))?(a):(b))
#undef __MAX2
#define __MAX2(a,b) (((a) > (b))?(a):(b))

/* Functions no longer implemented. */

#define __iLockMemory(arg1, arg2)    -1
#define _vZeroFile(arg1, arg2)       -1
#ifndef __LINUX__
  #define munlock(arg1, arg2)        -1
#endif

/**********************************************************************/
/*********************** LONG DOUBLE FUNCTIONS ************************/
/**********************************************************************/

/* Custom long double routines, implemented in trn.c with inline assembly
   specific to the x87 coprocessors. The corresponding intrinsic long
   double routines, mandated by C99 and gnu99, appear to be missing
   from DJGPP (through 2.04), MinGW, and CygWin. MSVC lacks _any_
   support for 80-bit long doubles.

   Function identifiers have been changed on more than one occasion,
   to resolve clashes with various compiler libraries. */

long double         __CEILL(long double ldArg);
long double         __EXPL(long double ldArg);
long double         __FABSL(long double ldArg);
long double         __FLOORL(long double ldArg);
long double         __FMODL(long double ldTop, long double ldBottom);
long double         __LOGL(long double ldArg);
long double         __LOG10L(long double ldArg);
long double         __LOG2L(long double ldArg);
long double         __POWL(long double ldBase, long double ldExp);
long double         __SQRTL(long double ldArg);
long double         __NEARBYINTL(long double ld);
long double         __NEXTTOWARDL(long double ld, long double ldDirection);
char *              szLDtoHex(char *sz, long double ld);

#if defined(__DJGPP__) || defined(__MINGW__) || defined(__CYGWIN__) \
    || defined(__LINUX__)  /* No native long double transcendentals */

#undef ceill
#undef expl
#undef fabsl
#undef floorl
#undef fmodl
#undef logl
#undef log10l
#undef log2l
#undef powl
#undef sqrtl

#define ceill       __CEILL
#define expl        __EXPL
#define fabsl       __FABSL
#define floorl      __FLOORL
#define fmodl       __FMODL
#define logl        __LOGL
#define log10l      __LOG10L
#define log2l       __LOG2L
#define powl        __POWL
#define sqrtl       __SQRTL

#endif

#ifdef __BORLANDC__
  #define log2l(x) (M_LOG2E*logl(x))
  #define log2(x)  ((double)(log2l(x)))
#endif

long double ___strtold(const char *sz, char **ppch);  /* uses mpf */
#undef __strtold
#define __strtold ___strtold

/* The above hack is resorted to because (1) A lot of my existing code
   uses the function identifier __strtold, and (2) CygWin code compiled
   as MinGW code using the switch -mno-cygwin insists it already has
   an intrinsic function named __strtold (although I can't find a
   prototype for it, and it appears to be dysfunctional). The hack
   thus avoids a linker clash while allowing the continued use of
   the traditional identifier __strtold. The identifier used in trn.c
   and in the prototype is ___strtold. */

/* Define aliases so that strtold is always available as the highest
   precision native string-to-fp conversion function, with _strtold
   as a synonym and _atold as a single-argument equivalent. CygWin
   and MSVC are special cases; CygWin has no native strtold, and
   MSVC has no 80-bit long double support at all. */

#if defined(__DJ203__) || defined(__BORLANDC__)
  #define  strtold         _strtold
#elif defined(__DMC__)  /* _atold is by default atof (!) */
  #undef  _atold
  #define _atold(x)        strtold(x, NULL)
#elif defined(__MINGW__)
  #define _atold(x)        strtold(x, NULL)
  #define _strtold         strtold
#elif defined(__LINUX__)
  #define _atold(x)        strtold(x, NULL)
  #define _strtold         strtold
#elif defined(__CYGWIN__) || defined(__MSVC__)
  #ifdef __MPIR__
    #define _atold(x)      ___strtold(x, NULL)
    #define  strtold       ___strtold
    #define _strtold       ___strtold
  #else
    #define _atold         atof
    #define  strtold       strtod
    #define _strtold       strtod
  #endif
#endif

#ifdef __BORLANDC__
  #define strtoll(sz, ep, b) _atoi64(sz)
#endif

char *__szLD(char *sz, long double ld, char *szFMT);

/**********************************************************************/
/********************* END LONG DOUBLE FUNCTIONS **********************/
/**********************************************************************/

/* Printout functions for printf-challenged environments, such as
   MinGW. Use along with __szLD above. */

char *__szLL(char *sz, int64_t ll);
char *__szULL(char *sz, uint64_t ull);

/**********************************************************************/
/************************* CONIO EXTENSIONS ***************************/
/**********************************************************************/

/* DJGPP and Borland C provide nearly complete support for the
   traditional DOS console conio functions, such as wherex, getch,
   kbhit, clreol, etc. If you need support for these on other
   platforms, refer to the library files conio3.h and conio3.c. */

#ifndef _CONIO3_H_  /* avoid duplication */

#undef __OBSL__  /* Output blank screen line (usually at program exit) */
#if defined(__LINUX__)
  #define __OBSL__ {printf("\n"); fprintf(stderr, "\n");}
#else
  #define __OBSL__ {printf("\n");}
#endif

#if defined(__DJGPP__) || defined (__BORLANDC__)
  #define __clearline()  {cputs("\r"); clreol();}
#elif defined(__DMC__)
  #define __clearline()  \
    {cputs("\r"); disp_open(); disp_eeol(); disp_close();}
#else
#define __clearline()  \
  { \
  char *pch; int iColumns, i; \
  pch=getenv("COLUMNS"); \
  if(pch)iColumns=atof(pch); else iColumns=80; \
  fflush(stderr); \
  putc('\r', stderr); \
  for(i=1; i < iColumns; i++)putc(' ', stderr); \
  putc('\r', stderr); \
  fflush(stderr); \
  }
#endif

#define __CLEAR_LINE__ __clearline()

/* clrscr in CygWin must be kludged; use the conio3 version instead */

#if defined(__MINGW__) || defined(__LINUX__)
  #define clrscr() {system("clear");}
#elif defined(__DMC__)
  #define clrscr() {system("cls");}
#elif defined(__CYGWIN__)  /* system calls fail within DOS/Windows */
  #define clrscr() {register int i; for(i=0;i<50;i++)fprintf(stderr,"\n");}
#endif

#ifdef __MINGW__

/*
 * These defines enable the use of all MinGW conio.h functions without
 * the underscore. However, _getch, _getche, and _kbhit are unreliable.
 */

#undef cgets
#undef cprintf
#undef cputs
#undef cscanf
#undef putch
#undef ungetch
#undef getch
#undef getche
#undef kbhit

#define cgets   _cgets
#define cprintf _cprintf
#define cputs   _cputs
#define cscanf  _cscanf
#define putch   _putch
#define ungetch _ungetch
#define getch   getchar
#define getche  getchar
#define kbhit   getchar

#endif  /* MINGW */

#if defined(__CYGWIN__) || defined(__LINUX__)

/* No valid implementation of these functions is currently available
   for CygWin or Linux. */

#define getch    getchar
#define putch    putchar
#define cputs    puts
#define cprintf  printf
#define cscanf   scanf
#define kbhit    getchar

#endif  /* CYGWIN or LINUX */

#endif  /* not _CONIO3_H_ */

/**********************************************************************/
/*************************** END CONIO EXTENSIONS *********************/
/**********************************************************************/

/**********************************************************************/
/*************************** MPFR support *****************************/
/**********************************************************************/

/* Simplified aliases for MPFR functions with "nearest" rounding mode. */

#ifdef __MPFR__

#define FN_add(mpfrSum, mpfr1, mpfr2) \
      mpfr_add(mpfrSum, mpfr1, mpfr2, GMP_RNDN)
#define FN_add_ui(mpfrSum, mpfr1, ui) \
      mpfr_add_ui(mpfrSum, mpfr1, ui, GMP_RNDN)
#define FN_div(mpfrQ, mpfrT, mpfrB) \
      mpfr_div(mpfrQ, mpfrT, mpfrB, GMP_RNDN)
#define FN_div_ui(mpfrQ, mpfrT, uiB) \
      mpfr_div_ui(mpfrQ, mpfrT, uiB, GMP_RNDN)
#define FN_get_d(mpfr) \
      mpfr_get_d(mpfr, GMP_RNDN)
#define FN_get_f(mpf, mpfr) \
      mpfr_get_f(mpf, mpfr, GMP_RNDN)
#define FN_get_ld(mpfr) \
      mpfr_get_ld(mpfr, GMP_RNDN)
#define FN_init_set_d(mpfr, d) \
      mpfr_init_set_d(mpfr, d, GMP_RNDN)
#define FN_init_set_ld(mpfr, ld) \
      mpfr_init_set_ld(mpfr, ld, GMP_RNDN)
#define FN_init_set_str(mpfr, sz, iBase) \
      mpfr_init_set_str(mpfr, sz, iBase, GMP_RNDN)
#define FN_init_set_ui(mpfr, ui) \
      mpfr_init_set_ui(mpfr, ui, GMP_RNDN)
#define FN_inp_str(mpfr, fp, iBase) \
      mpfr_inp_str(mpfr, fp, iBase, GMP_RNDN)
#define FN_log(mpfrLog, mpfrArg) \
      mpfr_log(mpfrLog, mpfrArg, GMP_RNDN)
#define FN_mul(mpfrP, mpfr1, mpfr2) \
      mpfr_mul(mpfrP, mpfr1, mpfr2, GMP_RNDN)
#define FN_out_str(fp, iBase, iDigits, mpfr) \
      mpfr_out_str(fp, iBase, iDigits, mpfr, GMP_RNDN)
#define FN_set_d(mpfr, d) \
      mpfr_set_d(mpfr, d, GMP_RNDN)
#define FN_set_ld(mpfr, ld) \
      mpfr_set_ld(mpfr, ld, GMP_RNDN)
#define FN_set_str(mpfr, sz, iBase) \
      mpfr_set_str(mpfr, sz, iBase, GMP_RNDN)
#define FN_set(mpfrOut, mpfrIn) \
      mpfr_set(mpfrOut, mpfrIn, GMP_RNDN)
#define FN_set_ui(mpfr, ui) \
      mpfr_set_ui(mpfr, ui, GMP_RNDN)
#define FN_set_uj(mpfr, ull) \
      mpfr_set_uj(mpfr, ull, GMP_RNDN)
#define FN_set_z(mpfr, mpz) \
      mpfr_set_z(mpfr, mpz, GMP_RNDN)
#define FN_sqr(mpfrSquare, mpfrArg) \
      mpfr_sqr(mpfrSquare, mpfrArg, GMP_RNDN)
#define FN_strtofr(mpfr, sz, ppch, iBase) \
      mpfr_strtofr(mpfr, sz, ppch, iBase, GMP_RNDN)
#define FN_sub(mpfrDiff, mpfr1, mpfr2) \
      mpfr_sub(mpfrDiff, mpfr1, mpfr2, GMP_RNDN)
#define FN_zeta(mpfrZeta, mpfrArg) \
      mpfr_zeta(mpfrZeta, mpfrArg, GMP_RNDN)

/**********************************************************************/

void mpfrLIRZ(mpfr_t mpfrx, mpfr_t mpfrLi, mpfr_t mpfrHL2,
  mpfr_t mpfrHL3, mpfr_t mpfrHL4, mpfr_t mpfrR, int iLocalPrec);
void mpfrRRF(mpfr_t mpfrOut, mpfr_t mpfrIn, int iLocalPrec);

#endif /* MPFR */

/**********************************************************************/
/************************* END MPFR aliases ***************************/
/**********************************************************************/

#endif  /* not _TRN_H_ */
