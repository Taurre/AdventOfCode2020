#ifndef COMMON_H
#define COMMON_H

struct seat {
	int row;
	int col;
	int64_t id;
};

struct range {
	int min;
	int max;
};

void fatal(char const *);
int compute_row(char const *);
int compute_col(char const *);

#endif /* COMMON_H */
