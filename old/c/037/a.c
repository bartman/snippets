#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

int
main (void)
{
#define TPS2_DMAx_CPP_NCP_MASK ((((uint32_t)0xFFFFFFFF)>>(32-27))<<(5))

        printf ("%d (32-27)\n", (32-27));
        printf ("%08x (((uint32_t)0xFFFFFFFF)>>(32-27))\n", 
                        (((uint32_t)0xFFFFFFFF)>>(32-27)));
        printf ("%08x \n", TPS2_DMAx_CPP_NCP_MASK);


}
