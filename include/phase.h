#ifndef __PHASE_H__
#define __PHASE_H__ 1

#include <stdint.h>

struct Phase {
  uint16_t acc;
  uint16_t inc;

  Phase& operator++();
  bool hasOverflowed() const;
  void reset();
};

#include "phase.hpp"

#endif
