#include <errno.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


void
fatal(char const *s)
{
	if (errno)
		perror(s);
	else
		fprintf(stderr, "%s\n", s);

	exit(EXIT_FAILURE);
}


enum type
instruction_invert(enum type type)
{
	return (type == CODE_JMP) ? CODE_NOOP : CODE_JMP;
}


void
code_extend(struct code *code)
{
	assert(code != NULL);

	if (!code->cap)
		code->cap = 8;
	else
		code->cap <<= 2;

	if (!code->cap)
		fatal("Integer overflow");
	if (SIZE_MAX / code->cap < sizeof *code->code)
		fatal("Integer overflow");

	struct instruction *tmp = realloc(code->code, code->cap * sizeof *code->code);

	if (tmp == NULL)
		fatal("realloc");

	code->code = tmp;
}


void
code_acc_and_counter_reset(struct code *code)
{
	assert(code != NULL);

	for (size_t i = 0; i < code->len; ++i)
		code->code[i].counter = 0;

	code->acc = 0;
}


struct code *
code_read(char const *filename)
{
	assert(filename != NULL);

	struct code *code = malloc(sizeof *code);

	if (code == NULL)
		fatal("malloc");

	*code = (struct code) { NULL };

	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		fatal("fopen");

	char instruction[4];
	int64_t value;

	while (fscanf(fp, "%3s %" SCNd64, instruction, &value) == 2) {
		if (code->cap <= code->len)
			code_extend(code);

		if (strcmp(instruction, "acc") == 0)
			code->code[code->len].type = CODE_ACC;
		else if (strcmp(instruction, "nop") == 0)
			code->code[code->len].type = CODE_NOOP;
		else if (strcmp(instruction, "jmp") == 0)
			code->code[code->len].type = CODE_JMP;

		code->code[code->len].counter = 0;
		code->code[code->len].value = value;
		++code->len;
	}

	fclose(fp);
	return code;
}


bool
code_exec(struct code *code)
{
	assert(code != NULL);

	size_t current = 0;

	while (current < code->len && !code->code[current].counter) {
		++code->code[current].counter;

		if (code->code[current].type == CODE_ACC) 
			code->acc += code->code[current].value;
		if (code->code[current].type == CODE_JMP) 
			current += code->code[current].value;
		else
			++current;
	}

	return code->code[current].counter ? false : true;
}
