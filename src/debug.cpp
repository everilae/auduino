// Arduino+simulavr debug library
//
// by Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:
// 19 Oct 2012: Use AVR stdio stream methods

#include <stdio.h>
#include <stdint.h>
#include "debug.h"

static volatile uint8_t *_port = reinterpret_cast<volatile uint8_t *>(DEBUGPORT);
static volatile uint8_t *_pin  = reinterpret_cast<volatile uint8_t *>(DEBUGPIN);

static int simulavr_fputc(const char c, FILE *stream) {
	*_port = c;
	return 0;
}

static int simulavr_fgetc(FILE *stream) {
	return *_pin;
}

static FILE _stderr;

void setup_debug() {
	fdev_setup_stream(&_stderr,
		simulavr_fputc,
		simulavr_fgetc,
		_FDEV_SETUP_WRITE | _FDEV_SETUP_READ);
	stderr = &_stderr;
}
