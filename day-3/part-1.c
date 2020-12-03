#include <stddef.h>
#include <stdio.h>

#include "common.h"


int
main(void)
{
	struct map *map = map_load("input.txt");
	struct map *current = map->next;
	unsigned move = 3;
	unsigned trees = 0;

	while (current != NULL) {
		if (what_is_there(current->line, move) == '#')
			++trees;

		current = current->next;
		move += 3;
	}

	printf("You encounter %u tree(s)\n", trees);
	return 0;
}
