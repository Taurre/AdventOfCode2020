#include <stdio.h>
#include <stdlib.h>

#include "common.h"


int
main(void)
{
	struct position position = { .direction = DIRECTION_EAST };
	char action;
	long unit;

	while (scanf("%c%ld", &action, &unit) == 2) {
		position_print(&position);
		position_update(&position, action, unit);
		scanf("%*c");
	}

	position_print(&position);
	printf("Part 1: %ld\n", manathan_distance(&position));
}
