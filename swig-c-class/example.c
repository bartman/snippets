/* File : example.c */

//#include <Python.h>
#include <time.h>
#include <stdio.h>

#include "example.h"

void dump(Vector *v)
{
	printf("Vector{%f, %f, %f}\n",
	       v->x, v->y, v->z);
}
