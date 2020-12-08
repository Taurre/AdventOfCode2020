#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common.h"


int
main(void)
{
	struct bag *bag = NULL;
	struct bag **next = &bag;
	struct bag *current = NULL;
	struct color color;

	while (read_color(&color)) {
		struct bag *new = bag_create(&color);

		if (color.count == 0) {
			current = new;
			*next = new;
			next = &new->next;
			continue;
		}

		new->next = current->inside;
		current->inside = new;
	}

	printf("Part 1: %" PRIu64 "\n", how_many_bag_contain(bag, "shiny gold"));
	printf("Part 2: %" PRIu64 "\n", how_many_bag_inside(bag, "shiny gold"));
	return 0;
}
