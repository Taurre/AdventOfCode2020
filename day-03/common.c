#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


void
fatal(char const *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}


void
chomp(char *s)
{
	while (*s != '\n' && *s != '\0')
		++s;

	*s = '\0';
}


char *
xstrdup(char const *s)
{
	assert(s != NULL);

	char *copy = malloc(strlen(s) + 1);

	if (copy == NULL)
		fatal("malloc");

	strcpy(copy, s);
	return copy;
}


struct map *
map_load(const char *filename)
{
	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		fatal("fopen");

	struct map *map = malloc(sizeof *map);

	if (map == NULL)
		fatal("malloc");

	char buf[255];
	struct map **current = &map;

	while (fgets(buf, sizeof buf, fp) != NULL) {
		struct map *new = malloc(sizeof *new);

		if (new == NULL)
			fatal("malloc");

		chomp(buf);
		new->line = xstrdup(buf);
		new->next = NULL;
		
		*current = new;
		current = &new->next;
	}

	if (ferror(fp))
		fatal("fgets");

	fclose(fp);
	return map;
}


char
what_is_there(char const *line, unsigned move)
{
	char const *s = line;

	for (; move; --move, ++s) {
		if (*s == '\0')
			s = line;
	}

	return (*s == '\0') ? *line : *s;
}
