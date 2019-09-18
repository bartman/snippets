#!/usr/bin/env bash
#if 0
set -e -x
tail -n +2 "$0" | gcc -x c -o $0.bin -
exec ./$0.bin
#endif

#include <stdio.h>

int main(void)
{
        printf("Hello world\n");
        return 0;
}
