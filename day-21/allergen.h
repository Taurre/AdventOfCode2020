#ifndef ALLERGEN_H
#define ALLERGEN_H

struct allergen {
	char name[16];
	uint32_t id;
	struct allergen *next;
};

extern uint32_t allergen_index(char const *);
extern char *allergen_name(uint32_t);

extern struct allergen *Allergens;

#endif /* ALLERGEN_H */
