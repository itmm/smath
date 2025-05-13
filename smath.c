#line 153 "a_build-system.md"
#include <stdbool.h>
#include <stdlib.h>
#line 97 "1_naturals.md"
#include <string.h>
#line 155 "a_build-system.md"

#include "smath.h"
#line 101 "1_naturals.md"

void sm_int_from_cstr(sm_int_p num, const char* cstr) {
	if (cstr) {
		sm_int_init(num, cstr, cstr + strlen(cstr));
	} else {
		sm_int_init(num, NULL, NULL);
	}
}
//...
#line 121

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
