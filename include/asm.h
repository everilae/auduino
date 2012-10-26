// Some ASM helpers
//
// by Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

#ifndef __ASM_H__
#define __ASM_H__ 1

#include <stdint.h>

#if __AVR_HAVE_MUL__ && __AVR_HAVE_MOVW__

/**
 * uint16_t acc;
 * CLR_ZERO_REG_BLOCK() {
 * 	mac(acc, 1, 2);
 *	mac(acc, 3, 4);
 * }
 */
#define CLR_ZERO_REG_BLOCK() \
	for (bool tmp = true; tmp; asm volatile ("clr __zero_reg__\n\t"), tmp = false)

static inline void mac(register uint16_t acc, const uint8_t x, const uint8_t y) {
	asm (	"mul %1, %2\n\t"
		"add %A0, r0\n\t"
		"adc %B0, r1\n\t"
		: "=r" (acc)
		: "r" (x), "r" (y));
}

static inline uint16_t mul(const uint8_t a, const uint8_t b) {
	uint16_t product;
	asm (	"mul %1, %2\n\t"
		"movw %0, __tmp_reg__\n\t"
		"clr __zero_reg__\n\t"
		: "=&r" (product)    // Force different register with '&'
		: "r" (a), "r" (b)); // so that a and b stay safe
	return product;
}

static inline int16_t mulsu(const int8_t a, const uint8_t b) {
	int16_t product;
	asm (	"mulsu %1, %2\n\t"
		"movw %0, __tmp_reg__\n\t"
		"clr __zero_reg__\n\t"
		: "=&r" (product)
		: "a" (a), "a" (b));
	return product;
}

#else

#define CLR_ZERO_REG_BLOCK() \
	for (bool tmp = true; tmp; tmp = false)

inline void mac(uint16_t &acc, const uint8_t x, const uint8_t y) {
	acc += x * y;
}

constexpr uint16_t mul(const uint8_t a, const uint8_t b) {
	return a * b;
}

constexpr int16_t mulsu(const int8_t a, const uint8_t b) {
	return a * b;
}

#endif

#endif
