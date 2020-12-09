#include <inttypes.h>
#include <stdio.h>
#include <stdint.h>

#include "common.h"


int
main(void)
{
	FILE *fp = fopen("input.txt", "r");

	if (fp == NULL)
		fatal("fopen");

	char row[8], col[4];
	int64_t id_max = 0;

	while (fscanf(fp, "%7s%3s", row, col) == 2) {
		struct seat seat = {
			.row = compute_row(row),
			.col = compute_col(col)
		};

		if (seat.row == -1 || seat.col == -1) {
			fprintf(stderr, "Error when parsing \"%s%s\"", row, col);
			break;
		}

		seat.id = seat.row * 8 + seat.col;

		if (seat.id > id_max)
			id_max = seat.id;
	}

	printf("The highest seat id is %" PRIu64 "\n", id_max);
	fclose(fp);
	return 0;
}

