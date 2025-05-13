#line 87 "a_build-system.md"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#line 175 "1_naturals.md"
#include <string.h>
#line 90 "a_build-system.md"

#include "smath.h"

int test_count = 0;

#define ASSERT(...) do { ++test_count; assert(__VA_ARGS__); } while(false)

int main(void) {
	// TESTS
#line 68 "1_naturals.md"
	{
#line 179
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end - 4);
		ASSERT(memcmp(start, "1024", 4) == 0);
	}
	{
#line 131
		const char* c = "123";
		struct sm_int i; sm_int_from_cstr(&i, c);
		ASSERT(i.begin == c);
		ASSERT(i.end == c + 3);
	}
	{
		struct sm_int i; sm_int_from_cstr(&i, NULL);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
		struct sm_int i; sm_int_from_cstr(&i, "");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
		struct sm_int i; sm_int_from_cstr(&i, "03x");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
		const char* c = "123";
		struct sm_int i; sm_int_init(&i, c + 2, c);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
		struct sm_int i; sm_int_init(&i, NULL, "1");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
#line 69
		struct sm_int i; sm_int_from_cstr(&i, "000");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
#line 99 "a_build-system.md"
	printf("%d tests ok\n", test_count);
}
