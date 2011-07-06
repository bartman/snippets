
primes: primes.o
	ld -o primes primes.o
	strip primes

primes.o: primes.S
	as --gstabs -o primes.o primes.S

clean:
	rm -f primes.o primes *~ core

#	gcc -ggdb -o primes.o -c primes.S
