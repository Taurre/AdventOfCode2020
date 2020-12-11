#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct array {
	char *data;
	size_t len;
	size_t cap;
};


struct layout {
	struct array *map;
	size_t line_size;
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


static char
occupied_change(char *map, size_t current, size_t line_size, size_t max)
{
	assert(map != NULL);

	size_t modulo = current % line_size;
	int moveh[3] = { 0 };
	int movev[3] = { 0 };
	int n = 1;
	int m = 1;
	int occupied = 0;

	if (modulo)
		moveh[n++] = -1;
	if (modulo < line_size - 1) 
		moveh[n++] = 1;
	if (current >= line_size)
		movev[m++] = -line_size;
	if (current + line_size < max)
		movev[m++] = line_size;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; ++j)
			if (map[current + moveh[i] + movev[j]] == '#')
				if (++occupied >= 5)
					return 'L';

	return '#';
}


static char
empty_change(char *map, size_t current, size_t line_size, size_t max)
{
	assert(map != NULL);

	size_t modulo = current % line_size;
	int moveh[3] = { 0 };
	int movev[3] = { 0 };
	int n = 1;
	int m = 1;

	if (modulo)
		moveh[n++] = -1;
	if (modulo < line_size - 1) 
		moveh[n++] = 1;
	if (current >= line_size)
		movev[m++] = -line_size;
	if (current + line_size < max)
		movev[m++] = line_size;

	for (int i = 0; i < n; i++)
		for (int j = 0; j < m; ++j)
			if (map[current + moveh[i] + movev[j]] == '#')
				return 'L';

	return '#';
}


static int
apply_rules(struct layout *layout)
{
	assert(layout != NULL);

	char *current = layout->map->data;
	static char *copy;

	if (copy == NULL) {
		copy = malloc(layout->map->len);

		if (copy == NULL)
			fatal("malloc");
	}

	memcpy(copy, current, layout->map->len);

	for (size_t i = 0; i < layout->map->len; ++i) {
		switch (current[i]) {
		case 'L':
			current[i] = empty_change(copy, i, layout->line_size, layout->map->len);
			break;
		case '#':
			current[i] = occupied_change(copy, i, layout->line_size, layout->map->len);
			break;
		}
	}

	return memcmp(current, copy, layout->map->len);
}


static void
print_layout(struct layout *layout)
{
	assert(layout != NULL);

	for (size_t i = 0; i < layout->map->len; ++i) {
		if (i && i % layout->line_size == 0)
			putchar('\n');

		printf("%c", layout->map->data[i]);
	}
}


static size_t how_many_occupied_seats(struct layout *layout)
{
	assert(layout != NULL);

	size_t n = 0;

	for (size_t i = 0; i < layout->map->len; ++i)
		if (layout->map->data[i] == '#')
			++n;

	return n;
}


int
main(void)
{
	struct layout layout = { .map = &(struct array) { .data = NULL } };
	int c;

	while ((c = getchar()) != EOF) {
		if (c == '\n') {
			if (!layout.line_size)
				layout.line_size = layout.map->len;

			continue;
		}

		array_add(layout.map, c);
	}

	if (ferror(stdin))
		fatal("getchar");

	for (size_t i = 0; apply_rules(&layout) != 0; ++i)
		;

	printf("Part 1: %zu\n", how_many_occupied_seats(&layout));
	return 0;

}
