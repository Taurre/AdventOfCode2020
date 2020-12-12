#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "common.h"


int
main(int argc, char **argv)
{
	FILE *fp = fopen("input.txt", "r");
	bool everyone;

	if (strstr(argv[0], "part-1") != NULL)
		everyone = false;
	else
		everyone = true;

	if (fp == NULL)
		fatal("fopen");

	uint32_t total = 0;
	int32_t nb;

	while (nb = group_count_questions(fp, everyone), nb >= 0)
		total += nb;

	printf("Total: %" PRIu32 "\n", total);
	return 0;
}
