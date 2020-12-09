#ifndef COMMON_H
#define COMMON_H

enum {
	FIELD_BYR = 1 << 0,
	FIELD_IYR = 1 << 1,
	FIELD_EYR = 1 << 2,
	FIELD_HGT = 1 << 3,
	FIELD_HCL = 1 << 4,
	FIELD_ECL = 1 << 5,
	FIELD_PID = 1 << 6,
	FIELD_CID = 1 << 7
};

#define FIELD_NAME 4
#define FIELD_VALUE 16
#define SCN_FIELD_NAME "%3s"
#define SCN_FIELD_VALUE "%15s"
#define PASSPORT_VALID 0x7F

struct field {
	char name[FIELD_NAME];
	char value[FIELD_VALUE];
	struct field *next;
};

struct passport {
	struct field *fields;
	unsigned char present;
	struct passport *next;
};

void panic(char const *);
struct passport *passport_read(char const *);
int passport_field(char const *);

#endif /* COMMON_H */
