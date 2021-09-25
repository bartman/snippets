/* example.i */
%module example
%{
/* Put header files here or function declarations like below */
#include "example.h"
%}

%include "example.h"
%extend Vector {
        Vector(double x, double y, double z) {
                Vector *v;
                v = (Vector *) malloc(sizeof(Vector));
                v->x = x;
                v->y = y;
                v->z = z;
                return v;
        }
        ~Vector() {
                free($self);
        }
};

