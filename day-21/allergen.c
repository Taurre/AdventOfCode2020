#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allergen.h"

struct allergen *Allergens;
static uint32_t Index = 1;


char *
allergen_name(uint32_t id)
{
	assert(id > 0);

	struct allergen *it = Allergens;

	while (it != NULL) {
		if (it->id == id)
			return it->name;

		it = it->next;
	}

	return NULL;
}


uint32_t
allergen_index(char const *name)
{
	assert(name != NULL);

	struct allergen *it = Allergens;

	while (it != NULL) {
		if (strcmp(it->name, name) == 0)
			return it->id;

		it = it->next;
	}

	struct allergen *allergen = malloc(sizeof *allergen);

	if (allergen == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	strcpy(allergen->name, name);
	allergen->id = Index++;
	allergen->next = Allergens;
	Allergens = allergen;
	return allergen->id;
}
