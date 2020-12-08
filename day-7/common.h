#ifndef COMMON_H
#define COMMON_H

enum state {
	ST_COLOR = 1,
	ST_NUMBER,
	ST_NEXT
};

struct color {
	char color[32];
	uint8_t count;
};

struct bag {
	char *color;
	uint8_t count;
	struct bag *inside;
	struct bag *next;
	bool visited;
};

int yylex(void);
uint64_t how_many_bag_contain(struct bag *, char const *);
uint64_t how_many_bag_inside(struct bag *, char const *);
struct bag *bag_create(struct color *);
bool read_color(struct color *);
void fatal(char const *);

char *yytext;

#endif /* COMMON_H */
