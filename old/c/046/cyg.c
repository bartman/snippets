#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <execinfo.h>

static char *get_symbol_string(void *pointer)
{
	void *buffer[2] = { pointer, NULL };
	char **strings, *p;
	char *file = NULL, *symbol = NULL, *location = NULL;
	int out_len;
	char *out;

	strings = backtrace_symbols(buffer, 1);
	if (strings == NULL) {
		perror("backtrace_symbols");
		exit(EXIT_FAILURE);
	}

	/* strings are either in format:
	 * a) file [0xLOCATION]
	 * b) file(symbol+0) [0xLOCATION]
	 * c) file(symbol+0xOFFSET) [0xLOCATION]
	 * ... want to convert that to ...
	 * a) 0xLOCATION(file)
	 * b) symbol(file)
	 * c) symbol+0xOFFSET(file)
	 */

	p = strings[0];
	file = p;

	while (*p) {
		if (*p == '(') {
			symbol = p + 1;
			*p = 0;

		} else if (*p == ')' || *p == ']' || *p == ' ') {
			*p = 0;

		} else if (*p == '[') {
			location = p + 1;
			*p = 0;
		}

		p++;
	}

	if (symbol) {
		char *plus0 = strstr(symbol, "+0");
		if (!plus0[2])
			*plus0 = 0;
	}

	out_len = strlen(file) + strlen(symbol) + strlen(location) + 16;

	out = malloc(out_len);
	snprintf(out, out_len, "%s(%s)",
			symbol ? symbol : location, file);

	free(strings);

	return out;
}

#define INDENT_MAX 50
static const char __indent_string[INDENT_MAX+3] = "| | | | | | | | | | | | | | | | | | | | | | | | | \0\0\0";
static const char *indent = __indent_string + INDENT_MAX;

void __cyg_profile_func_enter(void *this_fn, void *call_site)
{
	char *this_fn_str = get_symbol_string(this_fn);
	char *call_site_str = get_symbol_string(call_site);

	printf("%s,-< %s from %s\n", indent, this_fn_str, call_site_str);
	if ((indent-2) >= __indent_string)
		indent-=2;

	free(call_site_str);
	free(this_fn_str);
}

void __cyg_profile_func_exit(void *this_fn, void *call_site)
{
	char *this_fn_str = get_symbol_string(this_fn);
	char *call_site_str = get_symbol_string(call_site);

	if (indent <= __indent_string+INDENT_MAX)
		indent+=2;
	printf("%s`-> %s from %s\n", indent, this_fn_str, call_site_str);

	free(call_site_str);
	free(this_fn_str);
}

