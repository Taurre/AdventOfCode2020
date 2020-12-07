#include <assert.h>
#include <errno.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


uint64_t
how_many_bags_contain(struct bag *bag, char const *description)
{
	uint64_t n = 0;
	struct bag *current = bag;
	struct bag *

	while (current != NULL) {
		struct bag *inside = current->inside;

		while (inside != NULL) {
			if (strcmp(description, inside->name) == 0) {
				++n;
				n += how_many_bags_contain(bag, current->name);
				break;
			}

			inside = inside->next;
		} 

		current = current->next;
	}

	return n;
}


char *
bag_name(char const *description)
{
	assert(description != NULL);

	char *end = strstr(description, "bag");

	if (end == NULL)
		fatal("bag_name");
	if (end > description && end[-1] == ' ')
		--end;

	char *name = malloc(end - description);

	if (name == NULL)
		fatal("malloc");

	ptrdiff_t i;

	for (i = 0; i < end - description; ++i)
		name[i] = description[i];

	name[i] = '\0';
	return name;
}


struct bag *
bag_inside(char const *description)
{
	assert(description != NULL);

	if (strstr(description, "contain") != NULL)
		return bag_inside(strstr(description, "contain") + 7 + 1);
	if (strncmp(description, "no other bag", 12) == 0)
		return NULL;

	uint8_t n;
	int len;

	if (sscanf(description, "%hhd%n", &n, &len) != 1)
		fatal("sscanf");

	description += len + 1;
	struct bag *bag = bag_create(description, n, false);
	char const *next = strchr(description, ',');

	if (next != NULL)
		bag->next = bag_inside(next + 2);
	else
		bag->next = NULL;

	return bag;
}


struct bag *
bag_create(char const *description, uint8_t n, bool inside)
{
	assert(description != NULL);

	struct bag *bag = malloc(sizeof *bag);

	if (bag == NULL)
		fatal("malloc");

	bag->name = bag_name(description);
	bag->n = n;
	bag->next = NULL;

	if (inside)
		bag->inside = bag_inside(description);
	else
		bag->inside = NULL;

	return bag;		
}


struct bag *
bag_read_list(char const *filename)
{
	assert(filename != NULL);

	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		fatal("fopen");

	struct bag *list;
	struct bag **current = &list;
	char buf[255];

	while (fgets(buf, sizeof buf, fp) != NULL) {
		struct bag *new = bag_create(buf, 0, true);
		*current = new;
		current = &new->next;
	}

	if (ferror(fp))
		fatal("fgets");

	return list;
}
