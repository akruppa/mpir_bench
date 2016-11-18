MPIR Benchmark Two v0.6

This benchmark is intended to test the performance of MPIR by 
measuring the speed of basic arithmetic operations and the 
speed of a number of applications.

It tests the following basic operations:

   multiplication
   squaring
   division
   gcd
   extended gcd
   roots
   factorial
   
The application tests are:

    rsa             - RSA public key cryptography operation
    pi              - Calculate digits of pi
    bpsw            - The Baillie-PSW Primality Test
    wagstaff        - Anton Vrba's conjecture for Wagstaff numbers
    mersenne primes - Test primality of Mersenne numbers
    fermat primes   - Test primality of Fermat numbers

Building on Windows with Microsoft Visual Studio 2008
=====================================================

The VC++ solution is designed to sit at the same level as the mpir
root directory:

   mpir root directory (named 'mpir')
   benchmark (any name)

After unpacking it into this directory it can be built with Visual 
Studio 2008 by opening the solution file bench_two.sln in the Visual
Studio 2008 IDE. Remember to set the either the Release|win32 or 
Release|x64 configuration depending on the MPIR version you wish to 
test.

Building on Linux with GCC
==========================

The benmark should be unpacked into a top level sub-directory of
MPIR, after which the makefile will build the benchmark with the
local or an installed version of MPIR.  Specifying: 

make bench_two_gmp 

will compile the benchmark with GMP rather than MPIR

Test Output
===========

The tests output performance figures for each caategory of tests, 
weighted performance figures for groups of tests and an overall 
figure. Two values are given, the first being a measure of
operations/second and the second the operations/second per GHz
of machine speed.

Acknowledgements
================

1. The authors of the first GMP benchmark, aspects of which 
   have been used in this benchmark.
  
2. Code contributions from:

      Hanhong Xue
      Thomas R. Nicely
      Jeff Gilchist
      Bill Hart
      Jason Moxham


  Brian Gladman, October 2009
