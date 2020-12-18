#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>


static void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


int
main(void)
{
	static uint32_t numbers[30000001UL];
	int n;
	uint32_t turn = 1;
	uint32_t current = 0;

	while (scanf("%d", &n) == 1) {
		numbers[current] = turn;

		if (scanf(",") < 0)
			break;

		++turn;
		current = n;
	}

	if (ferror(stdin))
		fatal("scanf");

	uint32_t max = 2021;
	int part = 1;

compute:
	for (; turn < max; ++turn) {
		uint32_t next;

		if (numbers[current] && numbers[current] != turn)
			next = turn - numbers[current];
		else
			next = 0;
			
		numbers[current] = turn;
		current = next;
	}

	printf("Part %d: %" PRIu32 "\n", part, current);

	if (part == 1) {
		max = 30000001UL;
		part = 2;
		goto compute;
	}

	return 0;
}
