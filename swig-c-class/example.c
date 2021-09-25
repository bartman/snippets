/* File : example.c */

//#include <Python.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "example.h"

/* --- Vector --- */

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

/* --- Vectors --- */

Vectors* new_Vectors(void)
{
	Vectors * vs;

	vs = malloc(sizeof(*vs));

	vs->count = 0;
	vs->vectors = NULL;

	return vs;
}

void delete_Vectors(Vectors *vs)
{
	free(vs->vectors);
	free(vs);
}

const char *Vectors___str__(const Vectors *v)
{
	static char buf[128];
	snprintf(buf, sizeof(buf), "Vectors{%zu}",
		 v->count);
	return buf;
}

void Vectors_append(Vectors *vs, Vector *v)
{
	size_t size;
	Vector *new;

	size = (vs->count+1) * sizeof(Vector);
	new = realloc(vs->vectors, size);

	new[vs->count] = *v;

	vs->vectors = new;
	vs->count ++;
}

size_t Vectors___len__(const Vectors *vs)
{
	return vs->count;
}

const Vector* Vectors___getitem__(const Vectors *vs, size_t i)
{
	assert(i < vs->count);
	return vs->vectors + i;
}
