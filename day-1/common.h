#ifndef COMMON_H
#define COMMON_H

struct input {
	uint64_t n;
	struct input *next;
};

void fatal(char const *);
struct input *read_input(char const *);
void print_product(uint64_t *, size_t);

#endif /* COMMON_H */
