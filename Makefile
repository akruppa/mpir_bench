MPIR_BASE=/var/tmp/kruppa/git/mpir/
MPIR_INC=$(MPIR_BASE)
MPIR_LIB=$(MPIR_BASE)/.libs
GMP_BASE=$(HOME)
GMP_INC=$(GMP_BASE)/include/
GMP_LIB=$(GMP_BASE)/lib/

all:bench_two

bench_two: 
	cc -DUSE_MPIR fermat_prime_p.c mersenne_prime_p.c pi.c trn.c wagstaff_bench.c bench_two.c -o $@ -I$(MPIR_INC) -L$(MPIR_LIB) -static -lmpir -lm
bench_two_gmp:
	cc fermat_prime_p.c mersenne_prime_p.c pi.c trn.c wagstaff_bench.c bench_two.c -o $@ -I$(GMP_INC) -L$(GMP_LIB) -static -lgmp -lm

.PHONY	: clean
clean	:
	rm -f bench_two bench_two_gmp
