/* example.i */
%module example
%{
/* Put header files here or function declarations like below */
#include "example.h"
%}

extern int fact(int n);
extern int my_mod(int x, int y);
extern char *get_time();
