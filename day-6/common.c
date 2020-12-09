#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"


void
fatal(char const *s)
{
	if (errno)
		perror(s);
	else
		fprintf(stderr, "%s\n", s);

	exit(EXIT_FAILURE);
}


uint32_t
group_count_questions(FILE *fp)
{
	bool questions[255] = { 0 };
	char buf[255];
	uint32_t nb = 0;

	while (fgets(buf, sizeof buf, fp) != NULL) {
		if (buf[0] == '\n')
			break;

		for (char *s = buf; *s != '\n' && *s != '\0'; ++s)
			if (questions[*s] == false) {
				questions[*s] = true;
				++nb;
			}

	}

	if (ferror(fp))
		fatal("fgets");

	return nb;
}
