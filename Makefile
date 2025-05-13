.PHONY: tests

extracted-ts: $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@
	@$(MAKE) tests

tests:
	@echo tests
