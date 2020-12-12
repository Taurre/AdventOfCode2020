#ifndef COMMON_H
#define COMMON_H

enum direction {
	DIRECTION_NORTH,
	DIRECTION_EAST,
	DIRECTION_SOUTH,
	DIRECTION_WEST
};

struct position {
	long north;
	long south;
	long east;
	long west;
	char direction;
};

void fatal(char const *);
void position_update(struct position *, char, long);
enum direction direction_rotate(enum direction, int);
void values_update(long *, long *);
void waypoint_rotate(struct position *, int);
void position_update_with_waypoint(struct position *, struct position *, char, long);
void position_print(struct position *);
long manathan_distance(struct position *);

#endif /* COMMON_H */
