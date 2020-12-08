#ifndef COMMON_H
#define COMMON_H

enum type {
	CODE_NOOP,
	CODE_ACC,
	CODE_JMP
};

struct instruction {
	enum type type;
	int64_t value;
	uint8_t counter;
};

struct code {
	struct instruction *code;
	size_t len;
	size_t cap;
	int64_t acc;
};

void fatal(char const *);
enum type instruction_invert(enum type);
void code_extend(struct code *);
void code_acc_and_counter_reset(struct code *);
struct code *code_read(char const *s);
bool code_exec(struct code *);

#endif /* COMMON_H */
