// Auduino Grain oscillator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:
// 18 Oct 2012: Attempt at optimizing 8bit multiplications

#include <avr/pgmspace.h>

// Sine!
//
//  _--_
// -    -    -
//       -__-
//
static const uint8_t sine_lookup[256] PROGMEM = {
	128, 131, 134, 137, 140, 143, 146, 149, 152, 155, 158, 162, 165, 167, 170, 173,
	176, 179, 182, 185, 188, 190, 193, 196, 198, 201, 203, 206, 208, 211, 213, 215,
	218, 220, 222, 224, 226, 228, 230, 232, 234, 235, 237, 238, 240, 241, 243, 244,
	245, 246, 248, 249, 250, 250, 251, 252, 253, 253, 254, 254, 254, 255, 255, 255,
	255, 255, 255, 255, 254, 254, 254, 253, 253, 252, 251, 250, 250, 249, 248, 246,
	245, 244, 243, 241, 240, 238, 237, 235, 234, 232, 230, 228, 226, 224, 222, 220,
	218, 215, 213, 211, 208, 206, 203, 201, 198, 196, 193, 190, 188, 185, 182, 179,
	176, 173, 170, 167, 165, 162, 158, 155, 152, 149, 146, 143, 140, 137, 134, 131,
	128, 124, 121, 118, 115, 112, 109, 106, 103, 100, 97,  93,  90,  88,  85,  82,
	79,  76,  73,  70,  67,  65,  62,  59,  57,  54,  52,  49,  47,  44,  42,  40,
	37,  35,  33,  31,  29,  27,  25,  23,  21,  20,  18,  17,  15,  14,  12,  11,
	10,  9,   7,   6,   5,   5,   4,   3,   2,   2,   1,   1,   1,   0,   0,   0,
	0,   0,   0,   0,   1,   1,   1,   2,   2,   3,   4,   5,   5,   6,   7,   9,
	10,  11,  12,  14,  15,  17,  18,  20,  21,  23,  25,  27,  29,  31,  33,  35,
	37,  40,  42,  44,  47,  49,  52,  54,  57,  59,  62,  65,  67,  70,  73,  76,
	79,  82,  85,  88,  90,  93,  97,  100, 103, 106, 109, 112, 115, 118, 121, 124
};

#if __AVR_HAVE_MUL__ && __AVR_HAVE_MOVW__
static inline uint16_t mul(const uint8_t a, const uint8_t b) {
	uint16_t product;
	asm(	"mul %1, %2\n\t"
		"movw %0, __tmp_reg__\n\t"
		"clr __zero_reg__\n\t"
		: "=&r" (product)    // Force different register with '&'
		: "r" (a), "r" (b)); // so that a and b stay safe
	return product;
}

static inline int16_t mulsu(const int8_t a, const uint8_t b) {
	int16_t product;
	asm(	"mulsu %1, %2\n\t"
		"movw %0, __tmp_reg__\n\t"
		"clr __zero_reg__\n\t"
		: "=&r" (product)
		: "a" (a), "a" (b));
	return product;
}
#else
constexpr uint16_t mul(const uint8_t a, const uint8_t b) {
	return a * b;
}
#endif

inline void Env::tick() {
  // Make the grain amplitude decay by a factor every sample (exponential decay)
  amp -= mul(value(), decay);
}

inline uint8_t Env::value() const {
  return static_cast<uint8_t>(amp >> 8);
}

inline void Env::reset() {
  amp = 0x7fff;
}

inline void Grain::reset() {
  phase.reset();
  env.reset();
}

inline uint16_t Grain::getSample() const {
  // Convert phase into a triangle wave
  uint8_t value = phase.acc >> 7;
  if (phase.acc & 0x8000) value = ~value;
  // Multiply by current grain amplitude to get sample
  return mul(value, env.value());
}
