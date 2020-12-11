#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


static char
next_seat_part1(struct layout *layout, size_t current, int x, int y)
{
        assert(layout != NULL);

        size_t modulo = current % layout->line_size;

        if (x < 0 && !modulo)
                return '?';
        if (x > 0 && modulo == layout->line_size - 1)
                return '?';
        if (y < 0 && current < layout->line_size)
                return '?';
        if (y > 0 && current + layout->line_size >= layout->map->len)
                return '?';

        return layout->map->data[current + x + y];
}


int
main(void)
{
	struct layout layout = { .map = &(struct array) { .data = NULL } };
	int c;

	tolerance = 4;
	next_seat = &next_seat_part1;

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
