# Rechnen mit natürlichen Zahlen

## Repräsentation einer Zahl

Eine Zahl wird als String repräsentiert: eine Ziffer pro Byte. Das ist nicht
sonderlich speicher-effizient, aber es erleichtert das Wandeln in lesbare
Darstellungen und die Algorithmen kennen wir im Prinzip aus der Schule.

Eine Zahl wird durch zwei Zeiger repräsentiert: einen auf den Anfang und einen
hinter das letzte Byte. Ich bediene mich dabei den klassichen Iteratoren aus
C++.

Hier die Implementierung im Header `smath.h`:

```c
// ...

	struct sm_int {
		const char* begin;
		const char* end;
	};

	typedef struct sm_int* sm_int_p;
// ...
```

## Regeln für die Repräsentation einer Zahl

### Kein Speicher-Management

Der Speicher für die Zahlen muss ausserhalb verwaltet werden. Der `sm_int`-Typ
geht davon aus, dass der Speicher über die Laufzeit der Instanz konstant
bleibt.


### `NULL` repräsentiern 0

Wenn `begin` gleich `NULL` ist, dann muss auch `end` gleich `NULL` sein. Die
repräsentierte Zahl ist 0. So kann die Zahl 0 ohne zusätzlichen Speicher
abgebildet werden.


### Echter Bereich

Wenn `begin` nicht `NULL` ist, dann darf auch `end` nicht `NULL` sein und es
muss `begin < end` gelten.


### Nur Ziffern

Wenn `begin` nicht `NULL` ist, dann gilt für alle `begin <= x < end`, dass
`*x` eine dezimale Ziffer (`0` bis `9`) ist.


### Keine führenden Nullen

Wenn `begin` nicht `NULL` ist, dann ist `*begin` kein `0`-Zeichen. 


## Initialisierung einer Zahl

Bei der Initialisierung wird auf diese Einschränkungen Rücksicht genommen.
Dazu folgende Tests in `t_smath.c`:

```c
// ...
	// TESTS
	{
		struct sm_int i; sm_int_from_cstr(&i, "000");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
// ...
```

Dazu muss in `smath.h` erst einmal die Prototypen zum Initialisiern eines
Integers angegeben werden:

```c
// ...

// ...

// ...

	void sm_int_from_cstr(sm_int_p num, const char* cstr);
	void sm_int_init(sm_int_p num, const char* begin, const char* end);
// ...
```

Die Initialisierung von einem C-String in `smath.c` ruft einfach die andere
Initialisierung auf:

```c
// ...
#include <stdlib.h>
#include <string.h>
// ...

// ...

void sm_int_from_cstr(sm_int_p num, const char* cstr) {
	if (cstr) {
		sm_int_init(num, cstr, cstr + strlen(cstr));
	} else {
		sm_int_init(num, NULL, NULL);
	}
}
//...
```

Die eigentliche Initialisierung prüft, ob die übergebenen Argumente valide
sind. Ist das nicht der Fall, wird die Zahl auf 0 gesetzt.

```c
// ...

// ...

// ...

void sm_int_init(sm_int_p num, const char* begin, const char* end) {
	if (! num) { return; }
	num->begin = num->end = NULL;
	if (! begin) { return; }
	while (begin < end && *begin == '0') { ++begin; }
	if (begin >= end) { return; }
	for (const char* i = begin; i < end; ++i) {
		if (*i < '0' || *i > '9') { return; }
	}
	num->begin = begin;
	num->end = end;
}
```

Nur zur Sicherheit prüfen wir in `t_smath.c`, ob wir alles richtig gemacht
haben:

```c
// ...
	// TESTS
	{
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
		const char* c = "123";
		struct sm_int i; sm_int_init(&i, c + 2, c);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
		struct sm_int i; sm_int_init(&i, NULL, "1");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{
// ...
```


## Zahlen addieren

Formulieren wir zuerst wieder einen Test in `t_smath.c`, der Beschreibt, wie
die Addition von Statten gehen soll:

```c
// ...
#include <stdio.h>
#include <string.h>
// ...
	// TESTS
	{
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end - 4);
		ASSERT(memcmp(start, "1024", 4) == 0);
	}
	{
// ...
```

Zuerst muss wieder der Prototyp in `smath.h` deklariert werden:

```c
// ...

// ...

// ...

// ...

	char* sm_int_add(
		char* begin, char* end, const sm_int_p a, const sm_int_p b
	);
// ...
```

In `smath.c` erfolgt die Implementierung:

```c
// ...

// ...

// ...

// ...

char* sm_int_add(char* begin, char* end, const sm_int_p a, const sm_int_p b) {
	if (! begin || ! end || ! a || ! b || end < begin) { return NULL; }
	char *result = end;
	const char* cur_a = a->end;
	const char* cur_b = b->end;
	int value = 0;
	while (cur_a > a->begin || cur_b > b->begin || value) {
		if (cur_a > a->begin) { --cur_a; value += *cur_a - '0'; }
		if (cur_b > b->begin) { --cur_b; value += *cur_b - '0'; }
		if (result <= begin) { return NULL; }
		--result; *result = (value % 10) + '0';
		value /= 10;
	}
	return result;
}
```
