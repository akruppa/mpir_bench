

all:bench_two

bench_two: 
	cc -DUSE_MPIR fermat_prime_p.c mersenne_prime_p.c pi.c trn.c wagstaff_bench.c bench_two.c -o $@ -I.. -L../.libs -static -lmpir -lm
bench_two_gmp:
	cc fermat_prime_p.c mersenne_prime_p.c pi.c trn.c wagstaff_bench.c bench_two.c -o $@ -I.. -L../.libs -static -lgmp -lm

.PHONY	: clean
clean	:
	rm -f bench_two bench_two_gmp
