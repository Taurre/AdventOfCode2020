#include <assert.h>
#include <stddef.h>
#include <stdio.h>

#include "common.h"


static unsigned
how_many_trees_when_moving(struct map *map, unsigned right, unsigned down)
{
	assert(map != NULL);

	unsigned move = 0;
	unsigned trees = 0;

	while (map != NULL) {
		if (!move)
			goto next;

		if (what_is_there(map->line, move) == '#')
			++trees;

	next:
		for (unsigned i = down; i; --i)
			if (map != NULL)
				map = map->next;

		move += right;
	}

	return trees;
}


int
main(void)
{
	struct map *map = map_load("input.txt");
	unsigned long trees = 1;

	trees *= how_many_trees_when_moving(map, 1, 1);
	trees *= how_many_trees_when_moving(map, 3, 1);
	trees *= how_many_trees_when_moving(map, 5, 1);
	trees *= how_many_trees_when_moving(map, 7, 1);
	trees *= how_many_trees_when_moving(map, 1, 2);
	printf("%lu\n", trees);
	return 0;
}
