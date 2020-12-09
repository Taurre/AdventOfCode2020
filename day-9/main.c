#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PREAMBLE 25


static void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


static int
issum(long *preamble, long n)
{
	assert(preamble != NULL);

	for (unsigned i = 0; i < PREAMBLE; ++i)
		for (unsigned j = 0; j < PREAMBLE; ++j) {
			if (preamble[i] == preamble[j])
				continue;
			if (preamble[i] + preamble[j] == n)
				return 1;
		}

	return 0;
}


static void
preamble_read(long *preamble)
{
	assert(preamble != NULL);

	for (unsigned i = 0; i < PREAMBLE; ++i)
		if (scanf("%ld", preamble + i) != 1)
			fatal("scanf");
}


int
main(void)
{
	long numbers[PREAMBLE + 1];
	long *preamble = numbers + 1;
	long *last = numbers + PREAMBLE;

	preamble_read(preamble);

	long n;

	while (scanf("%ld", &n) == 1) {
		if (!issum(preamble, n)) {
			printf("Part 1: %ld\n", n);
			break;
		}

		memmove(numbers, preamble, sizeof (long[PREAMBLE]));
		*last = n;
	}

	if (ferror(stdin))
		fatal("scanf");

	return 0;
}
