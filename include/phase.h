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
  int16_t adj;

  Phase& operator++();
  bool hasOverflowed() const;
  void reset();
};

#include "phase.hpp"

#endif
