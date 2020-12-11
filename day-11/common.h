#ifndef COMMON_H
#define COMMON_H

struct array {
	char *data;
	size_t len;
	size_t cap;
};


struct layout {
	struct array *map;
	size_t line_size;
};


extern char (*next_seat)(struct layout *, size_t, int, int);
extern int tolerance;

void fatal(char const *);
struct layout *layout_copy(struct layout *);
void array_resize(struct array *, size_t);
void array_add(struct array *, char);
void print_layout(struct layout *);
size_t how_many_occupied_seats(struct layout *);
char empty_change(struct layout *, size_t);
char occupied_change(struct layout *, size_t);
int apply_rules(struct layout *);

#endif /* COMMON_H */
