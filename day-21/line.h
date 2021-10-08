#ifndef LINE_H
#define LINE_H

struct line {
	uint32_t *ingredients;
	size_t len;
	uint32_t allergen;
	struct line *next;
};

extern struct line *line_create(uint32_t *, size_t, uint32_t);
extern void line_merge(struct line *, struct line *);

#endif /* LINE_H */
