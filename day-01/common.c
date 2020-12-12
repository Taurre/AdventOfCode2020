#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "common.h"


void
fatal(char const *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}


struct input *
read_input(char const *filename)
{
	assert(filename != NULL);

	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		fatal("fopen");

	uint64_t n;
	struct input *input = NULL;

	while (fscanf(fp, "%" PRIu64, &n) == 1) {
		struct input *node = malloc(sizeof *node);

		if (node == NULL)
			fatal("malloc");

		node->n = n;
		node->next = input;
		input = node;
	}

	if (ferror(fp))
		fatal("fscanf");

	fclose(fp);
	return input;
}


void
print_product(uint64_t *factors, size_t size)
{
#define FMT "%" PRIu64
	assert(size >= 2);

	uint64_t product = factors[0];
	printf(FMT, factors[0]);

	for (size_t i = 1; i < size; ++i) {
		printf(" * " FMT, factors[i]);
		product *= factors[i];
	}

	printf(" = " FMT "\n", product);
#undef FMT
}
