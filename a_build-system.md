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
extracted-ts: $(wildcard *.md)
	@echo "extracting source code"
	@[ -x "$$(command -v mdp)" ] || echo "mdp not installed" 1>&2
	@[ -x "$$(command -v mdp)" ] && mdp README.md
	@date >$@
	@$(MAKE) tests
```

```make
.PHONY: tests

// ...

tests:
	@echo tests
```
