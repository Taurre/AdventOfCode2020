#include <stdint.h>
#include <stdlib.h>

#include "common.h"


int
main(void)
{
	struct input *input = read_input("input.txt");
	struct input *current = input;

	while (current != NULL) {
		struct input *p = input;

		while (p != NULL) {
			struct input *q = input;

			while (q != NULL) {
				if (current->n == q->n || p->n == q->n)
					goto next;
				if (current->n + p->n + q->n == 2020) {
					print_product((uint64_t[]) { current->n, p->n, q->n }, 3);
					goto end;
				}

			next:
				q = q->next;
			}

			p = p->next;
		}

		current = current->next;
	}

end:
	return 0;
}
