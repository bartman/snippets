/* example.i */
%module example

%{
        static int myErr = 0;
%}

%{
/* Put header files here or function declarations like below */
#include "example.h"

/* these are not exported, but used by the 'extend Vector' code */
extern Vector* new_Vector(double x, double y, double z);
void delete_Vector(Vector *v);
const char *Vector___str__(const Vector *v);

extern Vectors* new_Vectors(void);
void delete_Vectors(Vectors *vs);
const char* Vectors___str__(const Vectors *vs);
void Vectors_append(Vectors *vs, Vector *v);
size_t Vectors___len__(const Vectors *vs);
const Vector* Vectors___getitem__(const Vectors *vs, size_t i);
%}

%exception VectorsIter::__next__ {
        assert(!myErr);
        $action
        if (myErr) {
                myErr = 0;
                PyErr_SetString(PyExc_StopIteration, "End of iterator");
                return NULL;
        }
}

%inline %{
        typedef struct VectorsIter {
                const Vectors *vectors;
                size_t pos;
        } VectorsIter;
%}

%include "example.h"

%extend VectorsIter {
        VectorsIter * __iter__() {
                return $self;
        }

        Vector* __next__() {
                if ($self->pos < $self->vectors->count)
                        return $self->vectors->vectors + ($self->pos ++);
                myErr = 1;
                return NULL;
        }
}

%extend Vector {
        Vector(double x, double y, double z);   // calls new_Vector()
        ~Vector();                              // calls delete_Vector()
        const char * __str__() const;           // calls Vector___str__()
}

%extend Vectors {
        Vectors();                              // calls new_Vectors()
        ~Vectors();                             // calls delete_Vectors()
        const char * __str__() const;           // calls Vectors___str__()
        void append(Vector *v);                 // calls Vectors_append()
        size_t __len__() const;                 // calls Vectors___len__()
        Vector *__getitem__(size_t i) const;    // calls Vectors___getitem__()
        VectorsIter __iter__() const {
                struct VectorsIter ret = { $self, 0 };
                return ret;
        }
}

