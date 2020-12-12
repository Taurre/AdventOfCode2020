#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static void
fatal(char const *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}


int
main(void)
{
	FILE *fp = fopen("input.txt", "r");

	if (fp == NULL)
		fatal("fopen");

	unsigned first, second;
	char c;
	char password[255];
	unsigned valid = 0;

	while (fscanf(fp, "%u-%u %c: %254s", &first, &second, &c, password) == 4) {
		size_t len = strlen(password);

		if (!first || !second)
			fatal("Invalid input");
		if (first > len || second > len)
			fatal("Invalid input");
		if ((password[first - 1] == c) ^ (password[second - 1] == c))
			++valid;
	}

	if (ferror(fp))
		fatal("fscanf");

	printf("%u passwords are valid\n", valid);
	fclose(fp);
	return 0;
}
