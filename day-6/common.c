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


int32_t
group_count_questions(FILE *fp, bool everyone)
{
	unsigned char questions[255] = { 0 };
	char buf[255];
	uint32_t members;

	for (members = 0; fgets(buf, sizeof buf, fp) != NULL; ++members) {
		if (buf[0] == '\n')
			break;

		for (char *s = buf; *s != '\n' && *s != '\0'; ++s)
			++questions[*s];
	}

	if (ferror(fp))
		fatal("fgets");

	uint32_t nb = 0;

	if (!everyone) {
		for (uint32_t i = 0; i < sizeof questions; ++i)
			if (questions[i])
				++nb;
	} else {
		for (uint32_t i = 0; i < sizeof questions; ++i)
			if (questions[i] == members)
				++nb;
	}

	return (members) ? nb : -1;
}
