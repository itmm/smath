# Das Build-System von `smath`

Mit dem Build-System automatisiere ich folgende Aufgaben:

* das Extrahieren des Source-Codes aus den Markdown-Dateien,
* das Bauen der C-Library,
* das Bauen der Unit-Tests und
* das Ausführen der Unit-Tests.

Ich habe das Build-System mit [`GNU Make`](https://www.gnu.org/software/make/)
umgesetzt. Für das Extrahieren des Source-Codes verwende ich mein
Projekt [`md-patcher`](https://github.com/itmm/md-patcher).

## Source-Code extrahieren

Der Source-Code ist in den Markdown-Dateien dieses Projekts eingebettet. Er
kann mit dem Befehl `mdp` extrahiert werden. Jedoch nur, wenn der `md-patcher`
installiert ist. Diese zusätzliche Abfrage ist notwendig, damit die
Bibliothek auch ohne den `md-patcher` gebaut werden kann: der Source-Code
kann dann bei Änderungen jedoch nicht aus den Markdown-Dateien generiert
werden.

Bei jeder Extraktion wird die Datei `extracted-ts` auf das aktuelle Datum
gesetzt, um zu häufiges Extrahieren zu verhindern. Es kann vorkommen, dass
mit der Änderung einer Markdown-Datei keine Datei verändert wird. Ohne diese
Timestamp-Datei wird die Extraktion unnötig oft aufgerufen.

Hier ist die Extraktion in der Datei `./Makefile`:

```make
.PHONY: run-tests

run-tests: extracted-ts
	@$(MAKE) tests

extracted-ts: $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@
```


## Unit-Tests bauen

Ich habe einen virtuellen Target `tests`, der nur für das Ausführen der
Unit-Tests zuständig ist:

```make
.PHONY: tests
// ...

// ...

// ...

tests: t_smath
	@echo running unit-tests
	@./t_smath
```

```make
// ...

// ...

// ...

// ...

t_smath: t_smath.o libsmath.a
	$(CC) $^ -o $@

t_smath.o: t_smath.c smath.h

libsmath.a: smath.o
	$(AR) -cr $@ $^

smath.o: smath.c smath.h

```

```make
.PHONY: clean
// ...

// ...

// ...

// ...

// ...

// ...

// ...

// ...

clean:
	@rm -f libsmath.a t_smath *.o extracted-ts
```
