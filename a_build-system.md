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
.PHONY: all tests

all: extracted-ts
	@$(MAKE) tests

extracted-ts: $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@
```

Das sieht etwas umständlich aus. Leider kann `make` nicht automatisch
erkennen, welche Dateien sich in einem Schritt geändert haben. Es geht nur
von der Target-Datei aus.

Die Extraktion kann aber einen ganzen Berg an Dateien ändern. Daher muss
nach der Extraktion `make` noch einmal neu aufgerufen werden, um diese
Änderungen zu erkennen.

Daher der zusätzliche Default-Target `all`, der die Extraktion antriggert,
aber in jedem Fall aus ausführen der Unit-Tests in einem eigenen
`make`-Prozess durchführt.


## Unit-Tests bauen

Ich habe einen virtuellen Target `tests`, der nur für das Ausführen der
Unit-Tests zuständig ist:

```make
// ....

tests: t_smath
	@echo running unit-tests
	@./t_smath
```

Dadurch, dass der Target virtuell ist, wird er bei jedem Aufruf von `make`
ausgeführt.

Das Executable der Unit-Tests benötigt neben der Objekt-Datei der Tests auch
die Library, die statisch gelinkt wird:

```make
// ....

t_smath: t_smath.o libsmath.a
	@echo building $@
	@$(CC) $^ -o $@

t_smath.o: t_smath.c smath.h
```

Hier ist der Rahmen für das Ausführen der Unit-Tests in der Datei `t_smath.c`:

```c
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "smath.h"

int test_count = 0;

#define ASSERT(...) do { ++test_count; assert(__VA_ARGS__); } while(false)

int main(void) {
	// TESTS
	printf("%d tests ok\n", test_count);
}
```

Ich habe das Makro `assert` etwas aufgebohrt und zähle die Aufrufe mit. So
habe ich ein schnelles Feedback, dass neue Tests auch wirklich ausgeführt
werden.

## Library erstellen

Zurück in der Datei `./Makefile` baue ich eine statische Library mit dem
Object-Code.

```make
// ....

libsmath.a: smath.o
	@echo building $@
	@$(AR) -cr $@ $^

smath.o: smath.c smath.h
```

Das Grundgerüst für den Header `smath.h` sieht so aus:

```c
#if !defined(smath_h)
#define smath_h

#endif
```

Und die eigentliche Library `smath.c` sieht so aus:

```c
#include <stdbool.h>
#include <stdlib.h>

#include "smath.h"
```

Die Dateien habe ich hier angegeben, damit der Build-Prozess ohne Fehler
läuft.

## Temporäre Dateien löschen

In diesem Schritt lösche ich temporäre Dateien. Die Source-Dateien werden
nur dann als temporär angesehen, wenn sie mit `md-patcher` neu gebaut werden
können. Dazu muss `./Makefile` angepasst werden:

```make
.PHONY: clean
// ...

CFLAGS += -std=c17 -Wall -pedantic

%.o:%.c
	@echo "  building" $@
	@$(CC) $(CFLAGS) -c $< -o $@

// ....

clean:
	@echo "remove temopraries"
	@rm -f libsmath.a t_smath *.o extracted-ts
	@[ -x "$$(command -v mdp)" ] && rm -f t_smath.c smath.h smath.c
```
