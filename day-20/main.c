#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct image {
	struct tile *tile;
	uint32_t ntiles;
};

struct border {
	uint16_t up;
	uint16_t left;
	uint16_t right;
	uint16_t down;
};

struct tile {
	uint32_t id;
	struct border border[5];
	struct tile *next;
};


int
main(void)
{
	char buf[255];
	struct image *image = &(struct image) { .tile = NULL };
	struct tile *current = NULL;
	struct border border = { .up = 0 }, rborder = { .up = 0 };
	unsigned nline = 0;

	while (fgets(buf, sizeof buf, stdin) != NULL) {
		uint32_t id;

		if (*buf == '\n') {
			current->border[0] = border;
			current->border[1] = (struct border) {
				.up = rborder.left, 
				.right = border.up,
				.down = rborder.right,
				.left = border.down
			};
			current->border[2] = (struct border) {
				.up = border.down,
				.right = rborder.left,
				.down = border.up,
				.left = rborder.right
			};
			current->border[3] = (struct border) {
				.up = border.right,
				.right = rborder.down,
				.down = border.left,
				.left = rborder.up
			};
			current->border[4] = (struct border) {
				.up = rborder.up,
				.right = border.left,
				.down = rborder.down,
				.left = border.right
			};

			++image->ntiles;
			continue;
		}

		if (sscanf(buf, "Tile %" SCNu32 ":", &id) == 1) {
			current = malloc(sizeof *current);

			if (current == NULL) {
				perror("malloc");
				abort();
			}

			current->id = id;
			current->next = image->tile;
			image->tile = current;

			rborder = border = (struct border) { .up = 0 };
			nline = 0;
			continue;
		}

		for (unsigned i = 0; i < 10; ++i)
			if (buf[i] == '#') {
				if (nline == 0) {
					border.up |= 1 << i;
					rborder.up |= 1 << (9 - i);
				} else if (nline == 9) {
					border.down |= 1 << i;
					rborder.down |= 1 << (9 - i);
				}
				if (i == 0) {
					border.left |= 1 << nline;
					rborder.left |= 1 << (9 - nline);
				} else if (i == 9) {
					border.right |= 1 << nline;
					rborder.right |= 1 << (9 - nline);
				}
			}

		++nline;
	}

	if (ferror(stdin)) {
		perror("fgets");
		exit(EXIT_FAILURE);
	}


	uint64_t corner[4] = { 0 };
	uint16_t corner_index = 0;
	struct tile *p = image->tile;

	while (p != NULL) {
		struct tile *q = image->tile;
		uint16_t total_lineup = 0;

		while (q != NULL) {
			uint16_t lineup = 0;

			if (p == q)
				goto next;

			for (unsigned i = 0; i < 5; ++i) {
				for (unsigned j = 0; j < 5; ++j) {
					if (p->border[i].up == q->border[j].down)
						++lineup;
					if (p->border[i].down == q->border[j].up)
						++lineup;
					if (p->border[i].left == q->border[j].right)
						++lineup;
					if (p->border[i].right == q->border[j].left)
						++lineup;
				}

				if (lineup)
					break;
			}

			if (lineup)
				++total_lineup;

		next:
			q = q->next;
		}

		if (total_lineup == 2)
			corner[corner_index++] = p->id;

		p = p->next;
	}

	printf("%" PRIu64 "\n", corner[0] * corner[1] * corner[2] * corner[3]);
	return 0;
}
