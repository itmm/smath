# `smath` – exakte Arithmetik mit Strings

## Vorbedingungen

Um die C-Bibliothek bauen zu können, muss ich ein passendes `Makefile`
erzeugen. Dies erweitere ich so, dass ich auch Unit-Tests damit ausführen
kann (immerhin will ich mich an die Regeln des Test Driven Designs (TDD)
halten).

Da dies mit dem eigentlichen Projekt wenig zu tun hat und eher mechanischen
Charakter hat, habe ich den Teil in einen extra Anhang ausgelagert:

[Anhang A: Build-System](./a_build-system.md)


## Rechnen mit natürlichen Zahlen

Im ersten Schritt betrachte ich nur natürliche Zahlen (inklusive 0). Das
Rechnen mit diesen bildet die Basis für das anschließende Rechnen mit Brüchen.
Auch die natürlichen Zahlen können schon beliebig groß werden.

[Kapitel 1: Rechnen mit natürlichen Zahlen](./1_naturals.md)

