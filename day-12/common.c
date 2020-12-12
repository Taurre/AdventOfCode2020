#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"

static char map[] = {
	[DIRECTION_NORTH] = 'N',
	[DIRECTION_SOUTH] = 'S',
	[DIRECTION_EAST] = 'E',
	[DIRECTION_WEST] = 'W'
};


void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


void
position_update(struct position *position, char action, long unit)
{
	assert(position != NULL);

action:
	switch (action) {
	case 'N':
		position->north += unit;
		break;
	case 'S':
		position->south += unit;
		break;
	case 'E':
		position->east += unit;
		break;
	case 'W':
		position->west += unit;
		break;
	case 'F':
		action = map[position->direction];
		goto action;
	case 'L':
		position->direction -= unit / 90;

		if (position->direction < 0)
			position->direction += 4;

		break;
	case 'R':
		position->direction += unit / 90;

		if (position->direction >= 4)
			position->direction -= 4;

		break;
	}
}


enum direction
direction_rotate(enum direction direction, int x)
{
	long new = (long)direction + x;

	if (new >= 4)
		return new - 4;
	if (new < 0)
		return new + 4;

	return new;
}


void
waypoint_rotate(struct position *waypoint, int x)
{
	assert(waypoint != NULL);

	long source[4] = {
		[DIRECTION_NORTH] = waypoint->north,
		[DIRECTION_SOUTH] = waypoint->south,
		[DIRECTION_EAST] = waypoint->east,
		[DIRECTION_WEST] = waypoint->west
	};
	long destination[4];

	destination[direction_rotate(DIRECTION_NORTH, x)] = source[DIRECTION_NORTH];
	destination[direction_rotate(DIRECTION_SOUTH, x)] = source[DIRECTION_SOUTH];
	destination[direction_rotate(DIRECTION_EAST, x)] = source[DIRECTION_EAST];
	destination[direction_rotate(DIRECTION_WEST, x)] = source[DIRECTION_WEST];
	waypoint->north = destination[DIRECTION_NORTH];
	waypoint->south = destination[DIRECTION_SOUTH];
	waypoint->east = destination[DIRECTION_EAST];
	waypoint->west = destination[DIRECTION_WEST];
}


void
values_update(long *a, long *b)
{
	assert(a != NULL);
	assert(b != NULL);

	long n = *a - *b;

	if (n == 0)
		*a = *b = 0;
	else if (n < 0) {
		*a = 0;
		*b = labs(n);
	} else {
		*a = labs(n);
		*b = 0;
	}
}


void
position_update_with_waypoint(struct position *position, struct position *waypoint, char action, long unit)
{
	assert(position != NULL);
	assert(waypoint != NULL);

	long x[4] = { 0 };

	switch (action) {
	case 'N':
		waypoint->north += unit;
		break;
	case 'S':
		waypoint->south += unit;
		break;
	case 'E':
		waypoint->east += unit;
		break;
	case 'W':
		waypoint->west += unit;
		break;
	case 'F':
		position->north += waypoint->north * unit;
		position->south += waypoint->south * unit;
		position->east += waypoint->east * unit;
		position->west += waypoint->west * unit;
		break;
	case 'L':
		waypoint_rotate(waypoint, -unit / 90);
		break;
	case 'R':
		waypoint_rotate(waypoint, unit / 90);
		break;
	}

	values_update(&position->north, &position->south);
	values_update(&position->east, &position->west);
	values_update(&waypoint->north, &waypoint->south);
	values_update(&waypoint->east, &waypoint->west);
}


void
position_print(struct position *position)
{
	assert(position != NULL);

	printf("North: %ld\n", position->north);
	printf("South: %ld\n", position->south);
	printf("East: %ld\n", position->east);
	printf("West: %ld\n", position->west);
	printf("Current direction: %c\n", map[position->direction]);
}


long
manathan_distance(struct position *position)
{
	assert(position != NULL);

	long north = labs(position->north - position->south);
	long east = labs(position->east - position->west);

	return north + east;
}
