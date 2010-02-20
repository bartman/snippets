#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __atoi(buf,val) \
({    \
	typeof(val) v = (val); \
        size_t len; \
        char *start = (buf); \
        char *ptr = start; \
        do { v = v/10; ptr++; } while(v); \
        v = (val); \
        len = ptr-start; \
        *ptr = 0; \
        do { ptr--; *ptr = '0' + v%10; v=v/10; } while(v); \
        len; \
})

#define __atoi2(buf,val) \
({    \
	typeof(val) v = (val); \
        size_t len; \
	char ch; \
        char *start = (buf); \
        char *ptr = start; \
	/* generate a reversed string first */ \
        do { *ptr = '0' + v%10; v=v/10; ptr++; } while(v); \
        *ptr = 0; 		/* terminate string */ \
        len = ptr-start;	/* string length */ \
	ptr--; 			/* last character printed */ \
	/* now reverse the string */ \
	while (start < ptr) { \
		ch = *ptr; \
		*(ptr--) = *start; \
		*(start++) = ch; \
	} \
        len; 			/* return the length */ \
})




int 
main(void)
{
	int x, l1, l2;
	char b1[1024];
	char b2[1024];

	for (x=-1000; x<100000; x++) {
		l1 = __atoi (b1, x);
		l2 = __atoi2 (b2, x);

		printf ("%u: %s %s\n", x, b1, b2);
		if (l1 != l2)
			fprintf (stderr, "%u: lengths differ: %u %u\n", 
					x, l1, l2);
		if (strncmp (b1,b2,1023))
			fprintf (stderr, "%u: strings differ: %s %s\n", 
					x, b1, b2);
	}

	return 0;
}

