#include <stdlib.h>
#include "debug.h"

static volatile uint8_t *_port = reinterpret_cast<volatile uint8_t *>(DEBUGPORT);
static volatile uint8_t *_pin  = reinterpret_cast<volatile uint8_t *>(DEBUGPIN);

char _Adb::read() {
	return *_pin;
}

static void _write_cstring(const char *s) {
	char c;
	while ((c = *s++) != '\0') {
		*_port = c;
	}
}

void _Adb::write(const char *s) {
	_write_cstring(s);
}

static char buf[sizeof(unsigned long int) * 8 + 2];

inline static void _write_uint(unsigned int i, int radix) {
	utoa(i, buf, radix);
	_write_cstring(buf);
}

inline static void _write_int(int i, int radix) {
	itoa(i, buf, radix);
	_write_cstring(buf);
}

void _Adb::write(int i, int radix) {
	_write_int(i, radix);
}

void _Adb::write(uint8_t i, int radix) {
	_write_uint(i, radix);
}

void _Adb::write(uint16_t i, int radix) {
	_write_uint(i, radix);
}

void _Adb::write(int8_t i, int radix) {
	_write_int(i, radix);
}

void _Adb::write(const char &c) {
	*_port = c;
}

_Adb Adb;
