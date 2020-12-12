#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"


void
fatal(char const *s)
{
	if (errno)
		perror(s);
	else
		fprintf(stderr, "%s\n", s);

	exit(EXIT_FAILURE);
}


int
compute_row(char const *s)
{
	assert(s != NULL);

	int max = 64;
	struct range row = { .min = 0, .max = 127 };

	while (*s != '\0') {
		if (*s == 'F')
			row.max -= max;
		else
			row.min += max;

		max /= 2;
		++s;
	}

	return (row.min == row.max) ? row.min : -1;
}


int
compute_col(char const *s)
{
	assert(s != NULL);

	int max = 4;
	struct range col = { .min = 0, .max = 7 };

	while (*s != '\0') {
		if (*s == 'L')
			col.max -= max;
		else
			col.min += max;

		max /= 2;
		++s;
	}

	return (col.min == col.max) ? col.min : -1;
}
