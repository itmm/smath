.PHONY: clean
.PHONY: all tests

CFLAGS += -std=c17 -Wall -pedantic

%.o:%.c
	@echo "  building" $@
	@$(CC) $(CFLAGS) -c $< -o $@

all: extracted-ts
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
	@echo building $@
	@$(CC) $^ -o $@

t_smath.o: t_smath.c smath.h

libsmath.a: smath.o
	@echo building $@
	@$(AR) -cr $@ $^

smath.o: smath.c smath.h

clean:
	@echo "remove temopraries"
	@rm -f libsmath.a t_smath *.o extracted-ts
	@[ -x "$$(command -v mdp)" ] && rm -f t_smath.c smath.h smath.c
