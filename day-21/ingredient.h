#ifndef INGREDIENT_H
#define INGREDIENT_H

struct ingredient {
	char name[16];
	uint32_t id;
	uint32_t counter;
	struct ingredient *next;
};

extern uint32_t ingredient_index(char const *);
extern uint32_t ingredient_max(void);

extern struct ingredient *Ingredients;

#endif /* INGREDIENT_H */
