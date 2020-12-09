#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREAMBLE 25U

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
array_sum(struct array *self)
{
	assert(self != NULL);

	uint64_t res = 0;

	for (size_t i = 0; i < self->len; ++i)
		res += self->data[i];

	return res;
}


static uint64_t
array_min(struct array *self)
{
	assert(self != NULL);
	assert(self->len != 0);

	uint64_t min = self->data[0];

	for (size_t i = 1; i < self->len; ++i)
		if (self->data[i] < min)
			min = self->data[i];

	return min;
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


static int
issum(uint64_t *preamble, uint64_t n)
{
	assert(preamble != NULL);

	for (unsigned i = 0; i < PREAMBLE; ++i)
		for (unsigned j = 0; j < PREAMBLE; ++j) {
			if (preamble[i] == preamble[j])
				continue;
			if (preamble[i] + preamble[j] == n)
				return 1;
		}

	return 0;
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

	for (size_t i = PREAMBLE; i < array.len; ++i)
		if (!issum(array.data + i - PREAMBLE, array.data[i])) {
			n = array.data[i];
			printf("Part 1: %" SCNu64 "\n", n);
			break;
		}

	struct array bloc = { .data = NULL };

	for (size_t i = 0; i < array.len; ++i) {
		array_add(&bloc, array.data[i]);

		for (size_t j = i + 1; j < array.len; ++j) {
			array_add(&bloc, array.data[j]);

			if (array_sum(&bloc) == n && bloc.len > 2) {
				printf("Part 2: %" PRIu64 "\n", array_min(&bloc) + array_max(&bloc));
				break;
			}
		}

		bloc.len = 0;
	}

	return 0;
}
