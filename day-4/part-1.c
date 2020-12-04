#include <stdio.h>

#include "common.h"


int
main(void)
{
	struct passport *passport = passport_read("input.txt");
	unsigned valid = 0;

	while (passport != NULL) {
		if ((passport->present & PASSPORT_VALID) == PASSPORT_VALID)
			++valid;

		passport = passport->next;
	}

	printf("%u passport(s) are valid\n", valid);
	return 0;
}
