#include <stdio.h>
#include <stdlib.h>


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

	unsigned min, max;
	char c;
	char password[255];
	unsigned valid = 0;

	while (fscanf(fp, "%u-%u %c: %254s", &min, &max, &c, password) == 4) {
		unsigned count = 0;

		 for (char *s = password; *s != '\0'; ++s)
			if (*s == c)
				++count;

		 if (count >= min && count <= max)
			 ++valid;

	}

	if (ferror(fp))
		fatal("fscanf");

	printf("%u passwords are valid\n", valid);
	fclose(fp);
	return 0;
}
