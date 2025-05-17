#line 135 "a_build-system.md"
#include <stdbool.h>
#include <stdlib.h>
#line 99 "1_naturals.md"
#include <string.h>
#line 137 "a_build-system.md"

#include "smath.h"
#line 101 "1_naturals.md"

void sm_int_from_cstr(sm_int_p num, const char* cstr) {
	if (cstr) {
		sm_int_init(num, cstr, cstr + strlen(cstr));
	} else {
		sm_int_init(num, NULL, NULL);
	}
}
#line 116

void sm_int_init(sm_int_p num, const char* begin, const char* end) {
	if (! num) { return; }
	num->begin = num->end = NULL;
	if (! begin) { return; }
	while (begin < end && *begin == '0') { ++begin; }
	if (begin >= end) { return; }
	for (const char* i = begin; i < end; ++i) {
		if (*i < '0' || *i > '9') { return; }
	}
	num->begin = begin;
	num->end = end;
}
#line 192

bool sm_int_is_0(sm_int_p num) {
	return num && ! num->begin;
}
#line 259

char* sm_int_add(char* begin, char* end, const sm_int_p a, const sm_int_p b) {
	if (! begin || ! a || ! b || end < begin) { return NULL; }
	char *result = end;
	const char* cur_a = a->end;
	const char* cur_b = b->end;
	int value = 0;
	while (cur_a > a->begin || cur_b > b->begin || value) {
		if (cur_a > a->begin) { value += *--cur_a - '0'; }
		if (cur_b > b->begin) { value += *--cur_b - '0'; }
		if (result <= begin) { return NULL; }
		*--result = (value % 10) + '0';
		value /= 10;
	}
	return result;
}
#line 347

char* sm_int_sub(char* begin, char* end, const sm_int_p a, const sm_int_p b) {
	if (! begin || ! a || ! b || end < begin) { return NULL; }
	char *result = end;
	const char* cur_a = a->end;
	const char* cur_b = b->end;
	int borrow = 0;
	while (cur_a > a->begin || cur_b > b->begin) {
		int value = -borrow; borrow = 0;
		if (cur_a > a->begin) { value += *--cur_a - '0'; }
		if (cur_b > b->begin) { value -= *--cur_b - '0'; }
		if (value < 0) { value = 10 + value; borrow = 1; }
		if (result <= begin) { return NULL; }
		*--result = value + '0';
	}
	if (borrow) { return NULL; } // a < b;
	while (result < end && *result == '0') { ++result; }
	return result;
}
