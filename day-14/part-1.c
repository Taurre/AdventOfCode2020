#include <assert.h>
#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

struct mask {
	uint64_t one;
	uint64_t zero;
};

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
mask_set(struct mask *mask, char const *s)
{
	for (size_t i = 0; s[i] != '\n' && s[i] != '\0' && i < 36U; ++i) {
		switch (s[i]) {
		case '1':
			mask->one |= (uint64_t)1 << (35U - i);
			break;
		case '0':
			mask->zero |= (uint64_t)1 << (35U - i);
			break;

		}
	}

	mask->zero = ~mask->zero;
}


int
main(void)
{
	char buf[255];
	struct array memory = { .data = NULL };
	struct mask mask;

	while (fgets(buf, sizeof buf, stdin) != NULL) {
		char smask[37];
		struct value value = { 0 };

		if (sscanf(buf, "mask = %36s", smask) == 1) {
			mask.one = mask.zero = 0;
			mask_set(&mask, smask);
			continue;
		}
		else if (sscanf(buf, "mem[%" SCNu64 "] = %" SCNu64,\
		&value.address, &value.value) == 2) {
			value.value |= mask.one;
			value.value &= mask.zero;
			struct value *p = array_search(&memory, &value);

			if (p != NULL)
				p->value = value.value;
			else
				array_add(&memory, &value);
			
		}
		else
			fatal("Bad input");
	}
	
	if (ferror(stdin))
		fatal("fgets");

	uint64_t sum = 0;

	for (size_t i = 0; i < memory.len; ++i)
		sum += memory.data[i].value;

	printf("Part 1: %" PRIu64 "\n", sum);
	return 0;
}
