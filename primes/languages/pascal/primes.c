/* Output from p2c 1.21alpha-07.Dec.93, the Pascal-to-C translator */
/* From input file "primes.pas" */


#include <p2c/p2c.h>


Static long tmp, parm;


Static Void compute(request, estimate)
long request, estimate;
{
  long prime = 2;
  long i;
  long counter = 0, not_primes_size = estimate;
  uchar not_primes[estimate + 1];

  /* initialize variables */


  /* show header */
  printf("+-----------------------------+\n");
  printf("|      Count            Prime |\n");
  printf("+-----------------------------+\n");

  /* run loop */
  while (counter < request) {
    counter++;

    printf("|%11ld%17ld |\n", counter, prime);

    /* mark primes */
    i = prime * 2;
    while (i < not_primes_size) {
      not_primes[i] = 1;
      i += prime;
    }

    /* find next prime */
    do {
     prime++;
    }while (not_primes[prime]);

  }

  printf("+-----------------------------+\n");
}


main(argc, argv)
int argc;
Char *argv[];
{
  Char STR2[256], *STR3;

  PASCAL_MAIN(argc, argv);
  printf("ParamCount: %12d\n", P_argc - 1);
  if (P_argc == 1) {
    printf("Specify number of primes to run.\n");
    _Escape(1);
  }
  /* read the command line parameter */
  tmp = 1;
  strcpy(STR2, P_argv[1]);
  parm = strtol(STR2 + tmp - 1, &STR3, 10);
  tmp = STR3 - STR2 + 1;

  /* guesstimate the size of the array needed */
  tmp = (parm + 1) * (log((double)parm) + log(log((double)parm)));

  compute(parm, tmp);


  exit(EXIT_SUCCESS);
}



/* End. */
