/* example.i */
%module example
%{
/* Put header files here or function declarations like below */
#include "example.h"

/* these are not exported, but used by the 'extend Vector' code */
extern Vector* new_Vector(double x, double y, double z);
void delete_Vector(Vector *v);
const char *Vector___str__(const Vector *v);
%}

%include "example.h"
%extend Vector {
        Vector(double x, double y, double z);   // calls new_Vector()
        ~Vector();                              // calls delete_Vector()
        const char * __str__() const;           // calls Vector___str__()
};

