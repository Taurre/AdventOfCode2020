#ifndef COMMON_H
#define COMMON_H

struct map {
	char *line;
	struct map *next;
};


void fatal(char const *);
void chomp(char *);
char *xstrdup(char const *);
struct map *map_load(char const *);
char what_is_there(char const *, unsigned);

#endif /* COMMON_H */
