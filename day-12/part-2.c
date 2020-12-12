#include <stdio.h>
#include <stdlib.h>

#include "common.h"


int
main(void)
{
	struct position position = { .north = 0 };
	struct position waypoint = { .north = 1, .east = 10, .direction = DIRECTION_EAST };
	char action;
	long unit;
	
	while (scanf("%c%ld", &action, &unit) == 2) {
		position_update_with_waypoint(&position, &waypoint, action, unit);
		scanf("%*c");
	}

	printf("Part 2: %ld\n", manathan_distance(&position));
}
