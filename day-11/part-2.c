#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


char
next_seat_part2(struct layout *layout, size_t current, int x, int y)
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

        char seat = layout->map->data[current + x + y];
        return (seat == '.') ? next_seat(layout, current + x + y, x, y) : seat;
}


int
main(void)
{
	struct layout layout = { .map = &(struct array) { .data = NULL } };
	int c;

	tolerance = 5;
	next_seat = next_seat_part2;

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

	printf("Part 2: %zu\n", how_many_occupied_seats(&layout));
	return 0;

}
