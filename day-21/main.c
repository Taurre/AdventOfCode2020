#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "allergen.h"
#include "ingredient.h"
#include "lexer.h"
#include "line.h"

#define STACK_MAX 128U

struct stack {
	uint32_t data[STACK_MAX];
	size_t len;
};

struct dangerous_node {
	char name[16];
	char allergen[16];
	struct dangerous_node *next;
};


static struct dangerous_node *
node_insert(struct dangerous_node *list, struct dangerous_node *node)
{
	assert(node != NULL);

	if (list == NULL)
		return node;

	struct dangerous_node *it = list;
	struct dangerous_node *previous = list;

	while (it != NULL) {
		if (strcmp(it->allergen, node->allergen) > 0)
			break;

		previous = it;
		it = it->next;
	}

	node->next = previous->next;
	previous->next = node;
	return list;
}


static struct dangerous_node *
node_create(char const *ingredient, char const *allergen)
{
	assert(ingredient != NULL);
	assert(allergen != NULL);

	struct dangerous_node *self = malloc(sizeof *self);

	if (self == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	strcpy(self->name, ingredient);
	strcpy(self->allergen, allergen);
	self->next = NULL;
	return self;
}


int
main(void)
{
	int status;
	struct stack ingredients = { .len =  0 };
	struct line *lines = NULL;

next_token:
	while (status = yylex(), status > 0) {
		struct line *line;

		switch (status) {
		case LX_INGREDIENT:
			if (STACK_MAX <= ingredients.len) {
				fprintf(stderr, "Only %d ingredients per line are supported\n", STACK_MAX);
				exit(EXIT_FAILURE);
			}

			ingredients.data[ingredients.len++] = ingredient_index(yytext);
			break;

		case LX_ALLERGEN:
			line = line_create(ingredients.data, ingredients.len, allergen_index(yytext));
			struct line *it = lines;

			while (it != NULL) {
				if (it->allergen == line->allergen) {
					line_merge(it, line);
					goto next_token;
				}

				it = it->next;
			}

			line->next = lines;
			lines = line;
			break;

		case LX_EOL:
			ingredients.len = 0;
			break;
		}
	}

	struct ingredient *p = Ingredients;
	struct dangerous_node *list = NULL;
	uint32_t sum = 0;

	while (p != NULL) {
		struct line *q = lines;

		while (q != NULL) {
			for (size_t i = 0; i < q->len; ++i) {
				if (p->id == q->ingredients[i]) {
					printf("%" PRIu32 ": %s\n", p->id, allergen_name(q->allergen));
					goto next;
#if 0
					struct dangerous_node *node = node_create(p->name, allergen_name(q->allergen));
					list = node_insert(list, node);
					goto next;
#endif
				}
			}

			q = q->next;
		}

		sum += p->counter;
	next:
		p = p->next;
	}

	printf("Part 1: %" PRIu32 "\n", sum);
	return 0;
}
