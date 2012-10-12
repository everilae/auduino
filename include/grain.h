// Auduino Grain oscillator
//
// by Peter Knight, Tinker.it http://tinker.it,
//    Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:

#ifndef __GRAIN_H__
#define __GRAIN_H__ 1

#include <stdint.h>
#include "phase.h"

struct Env {
  uint16_t amp;
  uint8_t decay;

  void tick();
  uint8_t value() const;
  void reset();
};

struct Grain {
  Phase phase;
  Env env;

  void reset();
  uint16_t getSample() const;
};

#include "grain.hpp"

#endif
