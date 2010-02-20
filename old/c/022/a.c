#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

typedef struct {
        int a;
        int b;
        int c;
} foo_t;

int
main (void)
{
        foo_t foo = {
                .a = 1,
                .b = 1,
                .c = 1,
        };

        wchar_t *bar = L"bar";

        for (int i=0; i<1; i++) {
        }

        return 0;
}
