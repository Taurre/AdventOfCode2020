#include <assert.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


void
panic(char const *s)
{
	if (errno)
		perror(s);
	else
		fprintf(stderr, "%s\n", s);

	exit(EXIT_FAILURE);
}


int
passport_field(char const *field_name)
{
	if (strcmp(field_name, "byr") == 0)
		return FIELD_BYR;
	if (strcmp(field_name, "iyr") == 0)
		return FIELD_IYR;
	if (strcmp(field_name, "eyr") == 0)
		return FIELD_EYR;
	if (strcmp(field_name, "hgt") == 0)
		return FIELD_HGT;
	if (strcmp(field_name, "hcl") == 0)
		return FIELD_HCL;
	if (strcmp(field_name, "ecl") == 0)
		return FIELD_ECL;
	if (strcmp(field_name, "pid") == 0)
		return FIELD_PID;
	if (strcmp(field_name, "cid") == 0)
		return FIELD_CID;
}


struct passport *
passport_read(char const *filename)
{
	assert(filename != NULL);

	FILE *fp = fopen(filename, "r");

	if (fp == NULL)
		panic("fopen");

	struct passport *passport = malloc(sizeof *passport);

	if (passport == NULL)
		panic("malloc");

	*passport = (struct passport) { 0 };
	char buf[255];

	while (fgets(buf, sizeof buf, fp) != NULL) {
		char name[FIELD_NAME];
		char value[FIELD_VALUE];

		if (buf[0] == '\n') {
			struct passport *new = malloc(sizeof *new);

			if (new == NULL)
				panic("malloc");

			*new = (struct passport) { 0 };
			new->next = passport;
			passport = new;
		}

		int n;
		char *s = buf;

		while (sscanf(s, SCN_FIELD_NAME ":" SCN_FIELD_VALUE "%n", \
		name, value, &n) == 2) {
			struct field *field = malloc(sizeof *field);

			if (field == NULL)
				panic("malloc");

			strcpy(field->name, name);
			strcpy(field->value, value);
			field->next = passport->fields;
			passport->fields = field;
			passport->present |= passport_field(name);
			s += n;
		}
	}

	if (ferror(fp))
		panic("fgets");

	fclose(fp);
	return passport;
}
