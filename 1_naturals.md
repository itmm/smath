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


### `NULL` repräsentiert 0

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
	{ // parse zero digits
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
// ....

void sm_int_from_cstr(sm_int_p num, const char* cstr) {
	if (cstr) {
		sm_int_init(num, cstr, cstr + strlen(cstr));
	} else {
		sm_int_init(num, NULL, NULL);
	}
}
```

Die eigentliche Initialisierung prüft, ob die übergebenen Argumente valide
sind. Ist das nicht der Fall, wird die Zahl auf 0 gesetzt.

```c
// ....

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
	{ // parse simple number
		const char* c = "123";
		struct sm_int i; sm_int_from_cstr(&i, c);
		ASSERT(i.begin == c);
		ASSERT(i.end == c + 3);
	}
	{ // parse NULL
		struct sm_int i; sm_int_from_cstr(&i, NULL);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // parse empty string
		struct sm_int i; sm_int_from_cstr(&i, "");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // parse string with non-digits
		struct sm_int i; sm_int_from_cstr(&i, "03x");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // init with end before begin
		const char* c = "123";
		struct sm_int i; sm_int_init(&i, c + 2, c);
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
	{ // init with non-NULL end
		struct sm_int i; sm_int_init(&i, NULL, "1");
		ASSERT(i.begin == NULL);
		ASSERT(i.end == NULL);
	}
// ...
```

In `smath.h` wird eine Funktion hinzugefügt, um auf 0 zu prüfen:

```c
// ...

// ...

// ...

// ...

	bool sm_int_is_0(sm_int_p num);

// ...
```

Die Implementierung in `smath.c`:

```c
// ....

bool sm_int_is_0(sm_int_p num) {
	return num && ! num->begin;
}
```

Test in `t_smath.c`:

```c
// ...
	// TESTS
	{ // check sm_int_is_0
		struct sm_int z; sm_int_from_cstr(&z, "000");
		struct sm_int v; sm_int_from_cstr(&v, "1");
		ASSERT(sm_int_is_0(&z));
		ASSERT(! sm_int_is_0(NULL));
		ASSERT(! sm_int_is_0(&v));
	}
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
	{ // add number to itself
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end - 4);
		ASSERT(memcmp(start, "1024", 4) == 0);
	}
// ...
```

Zuerst muss wieder der Prototyp in `smath.h` deklariert werden:

```c
// ...

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
// ....

char* sm_int_add(char* begin, char* end, const sm_int_p a, const sm_int_p b) {
	if (! begin || ! a || ! b || end < begin) { return NULL; }
	char *result = end;
	const char* cur_a = a->end;
	const char* cur_b = b->end;
	int value = 0;
	while (cur_a > a->begin || cur_b > b->begin || value) {
		if (cur_a > a->begin) { value += *--cur_a - '0'; }
		if (cur_b > b->begin) { value += *--cur_b - '0'; }
		if (result <= begin) { return NULL; }
		*--result = (value % 10) + '0';
		value /= 10;
	}
	return result;
}
```

Weitere Tests in `t_smath.c`:

```c
// ...
	// TESTS
	{ // add with NULL arguments
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_add(buffer, buffer_end, &a, NULL);
		ASSERT(start == NULL);
		start = sm_int_add(buffer, buffer_end, NULL, &a);
		ASSERT(start == NULL);
		start = sm_int_add(buffer, NULL, &a, &a);
		ASSERT(start == NULL);
		start = sm_int_add(NULL, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
	{ // adding zero
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "99");
		struct sm_int z; sm_int_from_cstr(&z, "0");
		char* start = sm_int_add(buffer, buffer_end, &a, &z);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_add(buffer, buffer_end, &z, &a);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_add(buffer, buffer_end, &z, &z);
		ASSERT(start == buffer_end);
		start = sm_int_add(buffer, buffer, &z, &z);
		ASSERT(start == buffer);
	}
	{ // not enough room in add
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "50");
		char* start = sm_int_add(buffer, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
// ...
```

In `smath.h`:

```c
// ...

// ...

// ...

// ...

// ...

// ...

	char* sm_int_sub(
		char* begin, char* end, const sm_int_p a, const sm_int_p b
	);

// ...
```

In `smath.c`:

```c
// ....

char* sm_int_sub(char* begin, char* end, const sm_int_p a, const sm_int_p b) {
	if (! begin || ! a || ! b || end < begin) { return NULL; }
	char *result = end;
	const char* cur_a = a->end;
	const char* cur_b = b->end;
	int borrow = 0;
	while (cur_a > a->begin || cur_b > b->begin) {
		int value = -borrow; borrow = 0;
		if (cur_a > a->begin) { value += *--cur_a - '0'; }
		if (cur_b > b->begin) { value -= *--cur_b - '0'; }
		if (value < 0) { value = 10 + value; borrow = 1; }
		if (result <= begin) { return NULL; }
		*--result = value + '0';
	}
	if (borrow) { return NULL; } // a < b;
	while (result < end && *result == '0') { ++result; }
	return result;
}
```

In `t_smath.c`:

```c
// ...
	// TESTS
	{ // simple sub
		char buffer[3];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "100");
		struct sm_int b; sm_int_from_cstr(&b, "56");
		char* start = sm_int_sub(buffer, buffer_end, &a, &b);
		ASSERT(start == buffer + 1);
		ASSERT(memcmp(start, "44", 2) == 0);
		start = sm_int_sub(buffer, buffer_end, &b, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, &a, &a);
		ASSERT(start == buffer_end);
	}
	{ // sub with NULL arguments
		char buffer[5];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "512");
		char* start = sm_int_sub(buffer, buffer_end, &a, NULL);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, NULL, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, NULL, &a, &a);
		ASSERT(start == NULL);
		start = sm_int_add(NULL, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
	{ // subtracting zero
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "99");
		struct sm_int z; sm_int_from_cstr(&z, "0");
		char* start = sm_int_sub(buffer, buffer_end, &a, &z);
		ASSERT(start == buffer);
		ASSERT(memcmp(start, "99", 2) == 0);
		start = sm_int_sub(buffer, buffer_end, &z, &a);
		ASSERT(start == NULL);
		start = sm_int_sub(buffer, buffer_end, &z, &z);
		ASSERT(start == buffer_end);
		start = sm_int_sub(buffer, buffer, &z, &z);
		ASSERT(start == buffer);
	}
	{ // not enough room in sub
		char buffer[2];
		char* buffer_end = buffer + sizeof(buffer);
		struct sm_int a; sm_int_from_cstr(&a, "200");
		char* start = sm_int_sub(buffer, buffer_end, &a, &a);
		ASSERT(start == NULL);
	}
// ...
```
