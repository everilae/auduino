// Auduino Phase accumulator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

inline Phase& Phase::operator++() {
  // Increment the phase
  acc += inc;
  return *this;
}

// Using safe bool idiom introduces around 100 bytes more
inline bool Phase::hasOverflowed() const {
  return acc < inc;
}

inline void Phase::reset() {
  acc = 0;
}
