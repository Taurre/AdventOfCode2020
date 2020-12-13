#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>


struct bus {
	long id;
	long wait;
};


void
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
	long start;

	if (scanf("%ld\n", &start) != 1)
		fatal("scanf");

	struct bus bus = { .wait = start };
	long id;

	while (scanf("%ld", &id) == 1) {
		if (id - start % id < bus.wait) {
			bus.wait = id - start % id;
			bus.id = id;
		}

		int c;

		while (c = getchar(), c != EOF && c != '\n')
			if (isdigit(c)) {
				if (ungetc(c, stdin) == EOF)
					fatal("ungetc");

				break;
			}
	}

	if (ferror(stdin))
		fatal("scanf");

	printf("Part 1: %ld\n", bus.id * bus.wait);
	return 0;
}
