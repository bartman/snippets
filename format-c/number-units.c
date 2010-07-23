static char* num_convert(uint64_t num, char str[32])
{
	static const char *num_units = " kMGTPEZY";
	const char *unit = num_units;
	uint32_t tmp;

	while (num > 100000ULL) {
		num /= 1000;
		unit++;
		if (!*unit)
			return "?";
	}

	tmp = (uint32_t)num;
	*str = 0;
	snprintf (str, 32, "%u%c", tmp, *unit);

	return str;
}

