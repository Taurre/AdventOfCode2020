#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "common.h"


int
main(int argc, char **argv)
{
	struct passport *passport;
	unsigned valid = 0;

	if (strstr(argv[0], "part-1") != NULL)
		passport = passport_read("input.txt", false);
	else
		passport = passport_read("input.txt", true);

	while (passport != NULL) {
		if ((passport->present & PASSPORT_VALID) == PASSPORT_VALID)
			++valid;

		passport = passport->next;
	}

	printf("%u passport(s) are valid\n", valid);
	return 0;
}
