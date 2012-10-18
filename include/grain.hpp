// Auduino Grain oscillator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:
// 18 Oct 2012: Attempt at optimizing 8bit multiplications

#if __AVR_HAVE_MUL__ && __AVR_HAVE_MOVW__
inline uint16_t mul(const uint8_t a, const uint8_t b) {
	uint16_t product;
	asm(	"mul %1, %2\n\t"
		"movw %0, __tmp_reg__\n\t"
		"clr __zero_reg__\n\t"
		: "=&r" (product)    // Force different register with '&'
		: "r" (a), "r" (b)); // so that a and b stay safe
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
  return amp >> 8;
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
