*
* Prime number generator in fortran
*
* vvvv

      PROGRAM PRIMES
      implicit none
      
      integer request
      integer prime
      integer i
      integer counter
      
      integer not_primes_size
      integer not_primes(200000)

* this has to be read from the command line somehow
      request = 10000
      not_primes_size = 200000

      prime = 2
      counter = 0
      
      WRITE(UNIT=*, FMT=100) '+-----------------------------+'
      WRITE(UNIT=*, FMT=100) '|      Count            Prime |'
      WRITE(UNIT=*, FMT=100) '+-----------------------------+'
      
      do while ( counter .lt. request )
        counter = counter + 1

        WRITE(UNIT=*, FMT=200) counter, prime
        
        i = prime * 2
        do while ( i .lt. not_primes_size )
          not_primes(i) = 1
          i = i + prime
        end do
        
        prime = prime + 1
        do while( not_primes(prime) .eq. 1 )
          prime = prime + 1
        end do
        
      end do

      WRITE(UNIT=*, FMT=100) '+-----------------------------+'
      
 100  FORMAT (A31)
 200  FORMAT ('|',I11,I17,' |')

      END


