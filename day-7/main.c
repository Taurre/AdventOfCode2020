#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"


int
main(void)
{
	struct bag *list = bag_read_list("input.txt");
	uint64_t n = how_many_bags_contain(list, "shiny gold");

	printf("Total: %" PRIu64 "\n", n);
	return 0;
}
