#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct array {
	uint64_t *data;
	size_t len;
	size_t cap;
};


static void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


static int
compare(void const *p, void const *q)
{
	uint64_t a = *(uint64_t *)p;
	uint64_t b = *(uint64_t *)q;

	if (a == b)
		return 0;

	return a < b ? -1 : 1;
}


static void
array_add(struct array *self, uint64_t n)
{
	assert(self != NULL);

	if (self->cap <= self->len) {
		if (!self->cap)
			self->cap = 16UL;
		else
			self->cap <<= 2;

		if (!self->cap || SIZE_MAX / sizeof *self->data < self->cap)
			fatal("Integer overflow");

		void *tmp = realloc(self->data, self->cap * sizeof *self->data);

		if (tmp == NULL)
			fatal("realloc");

		self->data = tmp;
	}

	self->data[self->len++] = n;
}


static uint64_t
array_max(struct array *self)
{
        assert(self != NULL);
        assert(self->len != 0);

        uint64_t max = self->data[0];

        for (size_t i = 1; i < self->len; ++i)
                if (self->data[i] > max)
                        max = self->data[i];

        return max;
}


static struct array *
joltage_next(struct array *array, uint64_t n)
{
	assert(array != NULL);

	struct array *next = malloc(sizeof *next);

	if (next == NULL)
		fatal("malloc");

	*next = (struct array) { .data = NULL };

	for (size_t i = 0; i < array->len; ++i) {
		if (array->data[i] < n + 1)
			continue;
		if (array->data[i] > n + 3)
			break;

		array_add(next, array->data[i]);
	}

	return next;
}


static uint64_t
how_many_ways(struct array *array, unsigned char (*matrix)[array->len], size_t current)
{
	assert(matrix != NULL);
	uint64_t n = 0;

	for (size_t i = 0; i < array->len; ++i)
		if (matrix[current][i]) {
			if (i == 0)
				return 1;

			n += how_many_ways(array, matrix, i);
		}

	return n;
}


int
main(void)
{
	struct array array = { .data = NULL };
	uint64_t n;

	while (scanf("%" SCNu64 "\n", &n) == 1)
		array_add(&array, n);

	if (ferror(stdin))
		fatal("scanf");

	qsort(array.data, array.len, sizeof *array.data, &compare);
	uint64_t diff1 = 0, diff3 = 0;
	uint64_t previous = 0;

	for (size_t i = 0; i < array.len; ++i) {
		switch (array.data[i] - previous) {
		case 1:
			++diff1;
			break;
		case 3:
			++diff3;
			break;
		}

		previous = array.data[i];
	}

	++diff3;
	printf("Part 1: %" PRIu64 "\n", diff1 * diff3);

	array_add(&array, 0);
	array_add(&array, array_max(&array) + 3);
	qsort(array.data, array.len, sizeof *array.data, &compare);
	unsigned char (*matrix)[array.len] = calloc(sizeof *matrix, array.len);

	n = 0;

	for (size_t i = 0; i < array.len; ++i)
		for (size_t j = 0; j < array.len; ++j) {
			if (array.data[i] == array.data[j])
				continue;
			if (array.data[j] - array.data[i] > 3)
				continue;

			matrix[i][j] = 1;
			++n;
		}

	for (size_t i = 0; i < array.len; ++i) {
		for (size_t j = 0; j < array.len; ++j)
			printf("%d ", matrix[i][j]);

		putchar('\n');
	}

	printf("%" PRIu64 "\n", n);
	return 0;
}
