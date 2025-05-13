#line 97 "a_build-system.md"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "smath.h"

int test_count = 0;

#define ASSERT(...) do { ++test_count; assert(__VA_ARGS__); } while(false)

int main(void) {
	// TESTS
#line 68 "1_naturals.md"
	{
#line 143
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
#line 69
		struct sm_int i; sm_int_from_cstr(&i, "000");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
#line 109 "a_build-system.md"
	printf("%d tests ok\n", test_count);
}
