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
	
	printf("Waypoint:\n");
	position_print(&waypoint);
	printf("Ship position:\n");
	position_print(&position);

	while (scanf("%c%ld", &action, &unit) == 2) {
		printf("Applying %c %ld:\n", action, unit);
		position_update_with_waypoint(&position, &waypoint, action, unit);
		printf("Waypoint:\n");
		position_print(&waypoint);
		printf("Ship position:\n");
		position_print(&position);
		scanf("%*c");
	}

	printf("Waypoint:\n");
	position_print(&waypoint);
	printf("Ship position:\n");
	position_print(&position);
	printf("Part 2: %ld\n", manathan_distance(&position));
}
