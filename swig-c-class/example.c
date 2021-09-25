/* File : example.c */

//#include <Python.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "example.h"

Vector* new_Vector(double x, double y, double z)
{
	Vector * v;

	v = malloc(sizeof(*v));

	v->x = x;
	v->y = y;
	v->z = z;

	return v;

}
void delete_Vector(Vector *v)
{
	free(v);
}
const char *Vector___str__(const Vector *v)
{
	static char buf[128];
	snprintf(buf, sizeof(buf), "Vector{%f, %f, %f}",
		 v->x, v->y, v->z);
	return buf;
}

