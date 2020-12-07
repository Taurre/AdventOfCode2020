#ifndef COMMON_H
#define COMMON_H

struct bag {
	char *name;
	uint8_t n;
	struct bag *inside;
	struct bag *next;
};

void fatal(char const *);
uint64_t how_many_bags_contain(struct bag *, char const *);
char *bag_name(char const *);
struct bag *bag_inside(char const *);
struct bag *bag_create(char const *, uint8_t, bool);
struct bag *bag_read_list(char const *);

#endif /* COMMON_H */
