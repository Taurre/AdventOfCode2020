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
array_add(struct array *self, struct bus *)
{
	assert(self != NULL);

	if (self->cap <= self->len) {
		if (!self->cap)
			self->cap = 16UL;
		else
			self->cap <<= 2;

		array_resize(self, self->cap);
	}

	self->data[self->len++] = c;
}
