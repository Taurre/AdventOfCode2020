#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"


int
main(void)
{
	struct code *code = code_read("input.txt");
	(void)code_exec(code);

	printf("Part 1: %" PRId64 "\n", code->acc);
	code_acc_and_counter_reset(code);

	for (size_t i = 0; i < code->len; ++i) {
		if (code->code[i].type == CODE_ACC)
			continue;

		code->code[i].type = instruction_invert(code->code[i].type);

		if (code_exec(code))
			break;

		code_acc_and_counter_reset(code);
		code->code[i].type = instruction_invert(code->code[i].type);
	}

	printf("Part 2: %" PRId64 "\n", code->acc);
	return 0;
}
