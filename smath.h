#line 124 "a_build-system.md"
#if !defined(smath_h)
#define smath_h

#line 18 "1_naturals.md"
	struct sm_int {
		const char* begin;
		const char* end;
	};

	typedef struct sm_int* sm_int_p;
#line 127 "a_build-system.md"
#endif
#line 81 "1_naturals.md"

	void sm_int_from_cstr(sm_int_p num, const char* cstr);
	void sm_int_init(sm_int_p num, const char* begin, const char* end);
#line 194

	char* sm_int_add(
		char* begin, char* end, const sm_int_p a, const sm_int_p b
	);
