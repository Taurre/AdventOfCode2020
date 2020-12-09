#include <stdio.h>

#include "common.h"


int
main(void)
{
	struct passport *passport = passport_read("input.txt");
	unsigned n = 1;

	while (passport != NULL) {
		struct field *field = passport->fields;
		printf("Passport #%u\n", n);	

		while (field != NULL) {
			printf("\t%s = %s\n", field->name, field->value);
			field = field->next;
		}

		if ((passport->present & PASSPORT_VALID) == PASSPORT_VALID)
			printf("This passport is valid\n");
		else
			printf("This passport is not valid\n");

		putchar('\n');
		passport = passport->next;
		++n;
	}

	return 0;
}
