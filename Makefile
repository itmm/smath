.PHONY: tests
.PHONY: run-tests

run-tests: extracted-ts
	@$(MAKE) tests

extracted-ts: $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@

tests: t_smath
	@echo running unit-tests
	@./t_smath

t_smath: t_smath.o libsmath.a
	$(CC) $^ -o $@

t_smath.o: t_smath.c smath.h

libsmath.a: smath.o
	$(AR) -cr $@ $^

smath.o: smath.c smath.h

