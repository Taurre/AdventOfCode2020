#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct value {
	uint64_t address;
	uint64_t value;
};

struct array {
	struct value *data;
	size_t len;
	size_t cap;
};


static void
fatal(char const *message)
{
	if (errno)
		perror(message);
	else
		fprintf(stderr, "%s\n", message);

	exit(EXIT_FAILURE);
}


static void
array_resize(struct array *self, size_t newsize)
{
        assert(self != NULL);
        assert(newsize != 0);
        assert(newsize >= self->cap);

        if (SIZE_MAX / sizeof *self->data < newsize)
                fatal("Integer overflow");

        void *tmp = realloc(self->data, newsize * sizeof *self->data);

        if (tmp == NULL)
                fatal("realloc");

        self->data = tmp;
        self->cap = newsize;
}


static void
array_add(struct array *self, struct value *value)
{
        assert(self != NULL);
	assert(value != NULL);

        if (self->cap <= self->len) {
                if (!self->cap)
                        self->cap = 16UL;
                else
                        self->cap <<= 2;

                array_resize(self, self->cap);
        }

        self->data[self->len++] = *value;
}


static struct value *
array_search(struct array *self, struct value *value)
{
	assert(self != NULL);
	assert(value != NULL);

	for (size_t i = 0; i < self->len; ++i)
		if (self->data[i].address == value->address)
			return &self->data[i];

	return NULL;
}


static void
compute_addresses(struct array *addresses, uint64_t address, char const *s)
{
	assert(addresses != NULL);
	assert(s != NULL);

	uint64_t mask_one = 0;

	for (size_t i = 0; s[i] != '\n' && s[i] != '\0' && i < 36U; ++i)
		if (s[i] == '1')
			mask_one |= (uint64_t)1 << (35U - i);

	address |= mask_one;
	array_add(addresses, &(struct value) { address, 0 });

	for (size_t i = 0; s[i] != '\n' && s[i] != '\0' && i < 36U; ++i) {
		if (s[i] == 'X')
			for (size_t j = 0; j < addresses->len; ++j) {
				uint64_t one = addresses->data[j].address;
				one |= (uint64_t)1 << (35U - i);
				uint64_t zero = addresses->data[j].address;
				zero &= ~((uint64_t)1 << (35U - i));

				if (array_search(addresses, &(struct value) { one, 0 }) == NULL)
					array_add(addresses, &(struct value) { one, 0 });
				if (array_search(addresses, &(struct value) { zero, 0 }) == NULL)
					array_add(addresses, &(struct value) { zero, 0 });

			}
	}
}


int
main(void)
{
	char buf[255];
	struct array memory = { .data = NULL };
	struct array addresses = { .data = NULL };

	while (fgets(buf, sizeof buf, stdin) != NULL) {
		char smask[37];
		struct value value = { 0 };

		if (sscanf(buf, "mask = %36s", smask) == 1) {
			continue;
		}
		else if (sscanf(buf, "mem[%" SCNu64 "] = %" SCNu64,\
		&value.address, &value.value) == 2) {
			addresses.len = 0;
			compute_addresses(&addresses, value.address, smask);

			for (size_t i = 0; i < addresses.len; ++i) {
				struct value *p = array_search(&memory, &addresses.data[i]);

				if (p == NULL)
					array_add(&memory, &(struct value) { addresses.data[i].address, value.value });
				else
					p->value = value.value;
			}
		}
		else
			fatal("Bad input");
	}
	
	if (ferror(stdin))
		fatal("fgets");

	uint64_t sum = 0;

	for (size_t i = 0; i < memory.len; ++i)
		sum += memory.data[i].value;

	printf("Part 2: %" PRIu64 "\n", sum);
	return 0;
}
