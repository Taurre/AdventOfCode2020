#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
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
how_many_bag_contain(struct bag *bag, char const *color)
{
	assert(bag != NULL);
	assert(color != NULL);

	struct bag *list = bag;
	uint64_t n = 0;

	while (list != NULL) {
		struct bag *inside = list->inside;

		if (list->visited)
			goto next;

		while (inside != NULL) {
			if (strcmp(inside->color, color) == 0) {
				++n;
				list->visited = true;
				n += how_many_bag_contain(bag, list->color);
				break;
			}

			inside = inside->next;
		}

	next:
		list = list->next;
	}

	return n;
}


uint64_t
how_many_bag_inside(struct bag *bag, char const *color)
{
	assert(bag != NULL);
	assert(color != NULL);

	struct bag *list = bag;
	uint64_t n = 0;

	while (list != NULL) {
		if (strcmp(list->color, color) == 0) {
			struct bag *inside = list->inside;

			while (inside != NULL) {
				n += inside->count;
				n += how_many_bag_inside(bag, inside->color) * inside->count;
				inside = inside->next;
			}

			break;
		}

		list = list->next;
	}

	return n;
}


struct bag *
bag_create(struct color *color)
{
	assert(color != NULL);

	struct bag *self = malloc(sizeof *self);

	if (self == NULL)
		fatal("malloc");

	self->color = malloc(strlen(color->color) + 1);

	if (self->color == NULL)
		fatal("malloc");

	strcpy(self->color, color->color);
	self->count = color->count;
	self->inside = NULL;
	self->next = NULL;
	self->visited = false;
	return self;
}


bool
read_color(struct color *color)
{
	assert(color != NULL);

	enum state state;
	*color = (struct color) { .color = { 0 } };

	while ((state = yylex()) > 0) {
		if (state == ST_NEXT) {
			if (!color->color[0])
				continue;
			else
				return true;
		}
		else if (state == ST_NUMBER) {
			if (sscanf(yytext, "%" SCNu8, &color->count) != 1)
				fatal("sscanf");
		}
		else if (state == ST_COLOR) {
			if (!color->color[0])
				strcpy(color->color, yytext);
			else {
				strcat(color->color, " ");
				strcat(color->color, yytext);
			}
		}
	}

	return false;
}
