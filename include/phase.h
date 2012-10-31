// Auduino Phase accumulator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

#ifndef __PHASE_H__
#define __PHASE_H__ 1

#include <stdint.h>

struct Phase {
  uint16_t acc;
  uint16_t inc;
  uint16_t modInc;

  Phase& operator++();
  bool hasOverflowed() const;
  void reset();
  void setInc(uint16_t value);
  void modulate(uint16_t mod);
};

#include "phase.hpp"

#endif
