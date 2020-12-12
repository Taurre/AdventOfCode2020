#include <stdint.h>
#include <stdlib.h>

#include "common.h"


int
main(void)
{
	struct input *input = read_input("input.txt");
	struct input *current = input;

	while (current != NULL) {
		struct input *it = input;

		while (it != NULL) {
			if (current->n == it->n)
				goto next;
			if (current->n + it->n == 2020) {
				print_product((uint64_t[]) { current->n, it->n }, 2);
				goto end;
			}

		next:
			it = it->next;
		}

		current = current->next;
	}

end:
	return 0;
}
