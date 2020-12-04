#include <assert.h>
#include <errno.h>
#include <stdbool.h>
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


bool
is_between(char const *value, int min, int max)
{
	int n;

	if (sscanf(value, "%d", &n) == 1)
		if (n >= min && n <= max)
			return true;

	return false;
}


bool
is_height(char const *value)
{
	int n;
	char unit[3];

	if (sscanf(value, "%d%2s", &n, unit) == 2) {
		if (strcmp(unit, "cm") == 0 && n >= 150 && n <= 193)
			return true;
		if (strcmp(unit, "in") == 0 && n >= 59 && n <= 76)
			return true;
	}

	return false;
}


bool
is_hex_color(char const *value)
{
	char hex[7] = { 0 };

	if (sscanf(value, "#%6[abcdef0123456789]", hex) == 1)
		if (strlen(hex) == 6)
			return true;

	return false;
}


bool
is_eye_color(char const *value)
{
	char color[4];
	char const *allowed[] = {
		"amb", "blu", "brn", "gry", "grn", "hzl", "oth"
	};

	if (sscanf(value, "%3s", color) == 1) {
		for (size_t i = 0; i < sizeof allowed / sizeof *allowed; ++i)
			if (strcmp(color, allowed[i]) == 0)
				return true;
	}

	return false;
}


bool
is_pid(char const *value)
{
	char pid[16];

	if (sscanf(value, "%15s", pid) == 1)
		if (strlen(pid) == 9)
			return true;

	return false;
}


enum field_flag
passport_field(char const *name)
{
	if (strcmp(name, "byr") == 0)
		return FIELD_BYR;
	if (strcmp(name, "iyr") == 0)
		return FIELD_IYR;
	if (strcmp(name, "eyr") == 0)
		return FIELD_EYR;
	if (strcmp(name, "hgt") == 0)
		return FIELD_HGT;
	if (strcmp(name, "hcl") == 0)
		return FIELD_HCL;
	if (strcmp(name, "ecl") == 0)
		return FIELD_ECL;
	if (strcmp(name, "pid") == 0)
		return FIELD_PID;
	if (strcmp(name, "cid") == 0)
		return FIELD_CID;
}


enum field_flag
field_check(enum field_flag field, char const *value)
{
	switch (field) {
	case FIELD_BYR:
		return is_between(value, 1920, 2002) ? field : 0;
	case FIELD_IYR:
		return is_between(value, 2010, 2020) ? field : 0;
	case FIELD_EYR:
		return is_between(value, 2020, 2030) ? field : 0;
	case FIELD_HGT:
		return is_height(value) ? field : 0;
	case FIELD_HCL:
		return is_hex_color(value) ? field : 0;
	case FIELD_ECL:
		return is_eye_color(value) ? field : 0;
	case FIELD_PID:
		return is_pid(value) ? field : 0;
	case FIELD_CID:
		return FIELD_CID;
	}
}


struct passport *
passport_read(char const *filename, bool check_fields)
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

		while (sscanf(s, SCN_FIELD_NAME ":" SCN_FIELD_VALUE "%n",\
		name, value, &n) == 2) {
			struct field *field = malloc(sizeof *field);

			if (field == NULL)
				panic("malloc");

			strcpy(field->name, name);
			strcpy(field->value, value);
			field->next = passport->fields;
			passport->fields = field;
			enum field_flag field_flag = passport_field(name);

			if (check_fields)
				passport->present |= field_check(field_flag, value);
			else
				passport->present |= field_flag;

			s += n;
		}
	}

	if (ferror(fp))
		panic("fgets");

	fclose(fp);
	return passport;
}
