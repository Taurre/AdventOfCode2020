#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "line.h"

struct line *
line_create(uint32_t *ingredients, size_t len, uint32_t allergen)
{
	assert(ingredients != NULL);
	assert(len > 0);

	struct line *line = malloc(sizeof *line);

	if (line == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	line->ingredients = calloc(sizeof *ingredients, len);

	if (line->ingredients == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	for (size_t i = 0; i < len; ++i)
		line->ingredients[i] = ingredients[i];

	line->len = len;
	line->allergen = allergen;
	line->next = NULL;
	return line;
}


void
line_merge(struct line *dest, struct line *src)
{
	assert(dest != NULL);
	assert(src != NULL);

	size_t max = (dest->len > src->len) ? dest->len : src->len;
	uint32_t *ingredients = calloc(sizeof *ingredients, max);

	if (ingredients == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	size_t len = 0;

	for (size_t i = 0; i < src->len; ++i)
		for (size_t j = 0; j < dest->len; ++j)
			if (src->ingredients[i] == dest->ingredients[j])
				ingredients[len++] = src->ingredients[i];

	free(dest->ingredients);
	dest->ingredients = ingredients;
	dest->len = len;
	free(src->ingredients);
	free(src);
}
