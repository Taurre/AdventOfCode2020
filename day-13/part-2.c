#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>


struct array {
	struct bus **data;
	size_t len;
	size_t cap;
};


struct bus {
	unsigned id;
	unsigned offset;
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
compare(void const *a, void const *b)
{
	const struct bus *x = *(struct bus **)a;
	const struct bus *y = *(struct bus **)b;

	if (x->id == y->id)
		return 0;

	return (x->id < y->id) ? -1 : 1;
}


static void
array_resize(struct array *self, size_t newsize)
{
	assert(self != NULL);
	assert(newsize != 0);
	assert(newsize >= self->cap);

	if (SIZE_MAX / sizeof *self->data < newsize)
		fatal("Integer overflow");

	void *tmp = realloc(self->data, newsize * sizeof *self->data);

	if (tmp == NULL)
		fatal("realloc");

	self->data = tmp;
	self->cap = newsize;
}


static void
array_add(struct array *self, struct bus *bus)
{
	assert(self != NULL);

	if (self->cap <= self->len) {
		if (!self->cap)
			self->cap = 16UL;
		else
			self->cap <<= 2;

		array_resize(self, self->cap);
	}

	self->data[self->len++] = bus;
}


int
main(void)
{
	if (scanf("%*u\n") == EOF)
		fatal("scanf");

	struct array array = { .data = NULL };
	unsigned id;

	for (unsigned i = 0; scanf("%u", &id) == 1; ++i) {
		struct bus *new = malloc(sizeof *new);

		if (new == NULL)
			fatal("malloc");

		new->id = id;
		new->offset = i;
		array_add(&array, new);
		
		int c;

		while (c = getchar(), c != EOF && c != '\n') {
			if (isdigit(c)) {
				if (ungetc(c, stdin) == EOF)
					fatal("ungetc");

				break;
			}
			if (c == 'x')
				++i;
		}
	}

	if (ferror(stdin))
		fatal("scanf");

	qsort(array.data, array.len, sizeof *array.data, &compare);
	unsigned long n = array.data[0]->id;

	for (; n < ULONG_MAX - array.data[0]->id; n += array.data[0]->id) {
		size_t i = 1;

		for (; i < array.len; ++i)
			if ((n + array.data[i]->offset) % array.data[i]->id != 0)
				break;

		if (i == array.len) {
			printf("%lu\n", n);
			break;
		}
	}

	return 0;
}
