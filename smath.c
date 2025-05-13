#line 133 "a_build-system.md"
#include <stdbool.h>
#include <stdlib.h>
#line 92 "1_naturals.md"
#include <string.h>
#line 135 "a_build-system.md"

#include "smath.h"
#line 94 "1_naturals.md"

void sm_int_from_cstr(sm_int_p num, const char* cstr) {
	if (cstr) {
		sm_int_init(num, cstr, cstr + strlen(cstr));
	} else {
		sm_int_init(num, NULL, NULL);
	}
}
#line 109

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
#line 205

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
