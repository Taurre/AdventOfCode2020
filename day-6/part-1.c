#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"


int
main(void)
{
	FILE *fp = fopen("input.txt", "r");
	uint32_t total = 0;
	uint32_t nb;

	if (fp == NULL)
		fatal("fopen");

	while (nb = group_count_questions(fp), nb != 0)
		total += nb;

	printf("Total: %" PRIu32 "\n", total);
	return 0;
}
