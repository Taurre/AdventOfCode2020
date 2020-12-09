#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>

#include "common.h"

#define SEAT_ID_MAX 989

bool seats[SEAT_ID_MAX];


int
main(void)
{
	FILE *fp = fopen("input.txt", "r");

	if (fp == NULL)
		fatal("fopen");

	char row[8], col[4];

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
		seats[seat.id] = true;
	}

	for (int64_t i = 0; i < SEAT_ID_MAX; ++i)
		if (!seats[i] && i && i != SEAT_ID_MAX - 1 && seats[i + 1] && seats[i - 1])
			printf("Seat %" PRIu64 " is missing\n", i);

	fclose(fp);
	return 0;
}

