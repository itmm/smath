#line 88 "a_build-system.md"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#line 211 "1_naturals.md"
#include <string.h>
#line 91 "a_build-system.md"

#include "smath.h"

int test_count = 0;

#define ASSERT(...) do { ++test_count; assert(__VA_ARGS__); } while(false)

int main(void) {
	// TESTS
#line 345 "1_naturals.md"
	{ // simple sub
		char buffer[3];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "100");
		struct sm_int b; sm_int_from_cstr(&b, "56");
		char* start = sm_int_sub(buffer, buffer_end, &a, &b);
		ASSERT(start == buffer + 1);
		ASSERT(memcmp(start, "44", 2) == 0);
		start = sm_int_sub(buffer, buffer_end, &b, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end);
	}
	{ // sub with NULL arguments
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_sub(buffer, buffer_end, &a, NULL);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, NULL, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, NULL, &a, &a);
		ASSERT(start == NULL);
		start = sm_int_add(NULL, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
	{ // subtracting zero
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "99");
		struct sm_int z; sm_int_from_cstr(&z, "0");
		char* start = sm_int_sub(buffer, buffer_end, &a, &z);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_sub(buffer, buffer_end, &z, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, &z, &z);
		ASSERT(start == buffer_end);
		start = sm_int_sub(buffer, buffer, &z, &z);
		ASSERT(start == buffer);
	}
	{ // not enough room in sub
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "200");
		char* start = sm_int_sub(buffer, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
#line 264
	{ // add with NULL arguments
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, NULL);
		ASSERT(start == NULL);
		start = sm_int_add(buffer, buffer_end, NULL, &a);
		ASSERT(start == NULL);
		start = sm_int_add(buffer, NULL, &a, &a);
		ASSERT(start == NULL);
		start = sm_int_add(NULL, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
	{ // adding zero
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "99");
		struct sm_int z; sm_int_from_cstr(&z, "0");
		char* start = sm_int_add(buffer, buffer_end, &a, &z);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_add(buffer, buffer_end, &z, &a);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_add(buffer, buffer_end, &z, &z);
		ASSERT(start == buffer_end);
		start = sm_int_add(buffer, buffer, &z, &z);
		ASSERT(start == buffer);
	}
	{ // not enough room in add
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "50");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
#line 214
	{ // add number to itself
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end - 4);
		ASSERT(memcmp(start, "1024", 4) == 0);
	}
#line 193
	{ // check sm_int_is_0
		struct sm_int z; sm_int_from_cstr(&z, "000");
		struct sm_int v; sm_int_from_cstr(&v, "1");
		ASSERT(sm_int_is_0(&z));
		ASSERT(! sm_int_is_0(NULL));
		ASSERT(! sm_int_is_0(&v));
	}
#line 133
	{ // parse simple number
		const char* c = "123";
		struct sm_int i; sm_int_from_cstr(&i, c);
		ASSERT(i.begin == c);
		ASSERT(i.end == c + 3);
	}
	{ // parse NULL
		struct sm_int i; sm_int_from_cstr(&i, NULL);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // parse empty string
		struct sm_int i; sm_int_from_cstr(&i, "");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // parse string with non-digits
		struct sm_int i; sm_int_from_cstr(&i, "03x");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // init with end before begin
		const char* c = "123";
		struct sm_int i; sm_int_init(&i, c + 2, c);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // init with non-NULL end
		struct sm_int i; sm_int_init(&i, NULL, "1");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
#line 69
	{ // parse zero digits
		struct sm_int i; sm_int_from_cstr(&i, "000");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
#line 100 "a_build-system.md"
	printf("%d tests ok\n", test_count);
}
