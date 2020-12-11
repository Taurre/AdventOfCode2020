#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"

char (*next_seat)(struct layout *, size_t, int, int);
int tolerance;


void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


struct layout *
layout_copy(struct layout *self)
{
	assert(self != NULL);
	assert(self->map->len > 0);

	struct layout *copy = malloc(sizeof *copy);

	if (copy == NULL)
		fatal("malloc");

	copy->map = malloc(sizeof *copy->map);

	if (copy->map == NULL)
		fatal("malloc");

	copy->map->data = malloc(self->map->len);

	if (copy->map->data == NULL)
		fatal("malloc");

	memcpy(copy->map->data, self->map->data, self->map->len);
	copy->map->len = copy->map->cap = self->map->len;
	copy->line_size = self->line_size;
	return copy;
}


void
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


void
array_add(struct array *self, char c)
{
	assert(self != NULL);

	if (self->cap <= self->len) {
		if (!self->cap)
			self->cap = 16UL;
		else
			self->cap <<= 2;

		array_resize(self, self->cap);
	}

	self->data[self->len++] = c;
}


void
print_layout(struct layout *layout)
{
	assert(layout != NULL);

	for (size_t i = 0; i < layout->map->len; ++i) {
		if (i && i % layout->line_size == 0)
			putchar('\n');

		printf("%c", layout->map->data[i]);
	}
}


size_t
how_many_occupied_seats(struct layout *layout)
{
	assert(layout != NULL);

	size_t n = 0;

	for (size_t i = 0; i < layout->map->len; ++i)
		if (layout->map->data[i] == '#')
			++n;

	return n;
}


char
occupied_change(struct layout *layout, size_t current)
{
	assert(layout != NULL);

	int x[] = { 0, -1, 1 };
	int y[] = { 0, -layout->line_size, layout->line_size };
	size_t occupied = 0;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; ++j)
			if (next_seat(layout, current, x[i], y[j]) == '#')
				if (++occupied >= tolerance + 1)
					return 'L';

	return '#';
}


char
empty_change(struct layout *layout, size_t current)
{
	assert(layout != NULL);

	int x[] = { 0, -1, 1 };
	int y[] = { 0, -layout->line_size, layout->line_size };

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; ++j)
			if (next_seat(layout, current, x[i], y[j]) == '#')
				return 'L';

	return '#';
}


int
apply_rules(struct layout *layout)
{
        assert(layout != NULL);

        static struct layout *copy;

        if (copy == NULL)
                copy = layout_copy(layout);
        else
                memcpy(copy->map->data, layout->map->data, layout->map->len);

        for (size_t i = 0; i < layout->map->len; ++i) {
                switch (layout->map->data[i]) {
                case 'L':
                        layout->map->data[i] = empty_change(copy, i);
                        break;
                case '#':
                        layout->map->data[i] = occupied_change(copy, i);
                        break;
                }
        }

        return memcmp(layout->map->data, copy->map->data, layout->map->len);
}
