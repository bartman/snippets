#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

/* create a new buffer and populate with concatenation of all strings */
char * strjoin(const char *glue, ...)
{
	va_list ap;
	unsigned total, count, len, glue_len;
	const char *str;
	char *res;

	va_start(ap, glue);
	total = count = 0;
	while ((str = va_arg(ap, const char *))) {
		total += strlen(str);
		count ++;
	}
	va_end(ap);

	glue_len = glue ? strlen(glue) : 0;
	total += (count - 1) * glue_len;
	res = (char*)malloc(total + 1);
	if (!res)
		return NULL;

	va_start(ap, glue);
	total = 0;
	while ((str = va_arg(ap, const char *))) {
		if (total && glue_len) {
			memcpy(res + total, glue, glue_len);
			total += glue_len;
		}
		strcpy(res + total, str);
		total += strlen(str);
	}
	va_end(ap);

	return res;
}

int main(int argc, char * argv[])
{
	char *s;

	s = strjoin("-", "a", "b", "c", NULL);
	printf("joined: %s\n", s);
	free(s);

	s = strjoin(
		"----------------------------------------------------------------------------------------------------",
		"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA",
		"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
		"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC",
		"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD",
		NULL);
	printf("joined: %s\n", s);
	free(s);

	return 0;
}
