// Auduino Grain oscillator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

inline void Env::tick() {
  // Make the grain amplitude decay by a factor every sample (exponential decay)
  amp -= (amp >> 8) * decay;
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
  return value * env.value();
}
