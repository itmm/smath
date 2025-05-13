#line 97 "a_build-system.md"
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "smath.h"

int test_count = 0;

#define ASSERT(...) do { ++test_count; assert(__VA_ARGS__) } while(false)

int main(void) {
	// TESTS
	printf("%d tests ok\n", test_count);
}
