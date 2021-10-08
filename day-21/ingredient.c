#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ingredient.h"

struct ingredient *Ingredients;
static uint32_t Index = 1;


uint32_t
ingredient_index(char const *name)
{
	assert(name != NULL);

	struct ingredient *it = Ingredients;

	while (it != NULL) {
		if (strcmp(it->name, name) == 0) {
			++it->counter;
			return it->id;
		}

		it = it->next;
	}

	struct ingredient *ingredient = malloc(sizeof *ingredient);

	if (ingredient == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	strcpy(ingredient->name, name);
	ingredient->id = Index++;
	ingredient->counter = 1;
	ingredient->next = Ingredients;
	Ingredients = ingredient;
	return ingredient->id;
}


uint32_t
ingredient_max(void)
{
	return Index;
}
