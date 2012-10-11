#ifndef __DEBUG_H__
#define __DEBUG_H__ 1

# ifndef DEBUGPORT
#  define DEBUGPORT 0x20
# endif
# ifndef DEBUGPIN
#  define DEBUGPIN 0x22
# endif

#include <stdint.h>

struct _Adb {

	char read();
	void write(const char *s);
	void write(int i, int radix = 10);
	void write(uint8_t i, int radix = 10);
	void write(uint16_t i, int radix = 10);
	void write(int8_t i, int radix = 10);
	void write(const char &c);

};

# ifdef DEBUG
extern _Adb Adb;
#  define DEBUG_WRITE(...) Adb.write(__VA_ARGS__)
# else
#  define DEBUG_WRITE(...) /* __VA_ARGS__ */
# endif

#endif
