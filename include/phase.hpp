// Auduino Phase accumulator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

inline void Phase::setInc(uint16_t value) {
  inc = modInc = value;
}

inline void Phase::modulate(uint16_t mod) {
  // 16bit * 14bit >> 13bit -> 17bit
  modInc = mod == 0x2000 ? inc : inc * mod >> 13;
}

inline Phase& Phase::operator++() {
  // Increment the phase
  acc += modInc;
  return *this;
}

// Using safe bool idiom introduces around 100 bytes more
inline bool Phase::hasOverflowed() const {
  return acc < modInc;
}

inline void Phase::reset() {
  acc = 0;
}
