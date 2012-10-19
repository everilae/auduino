// Arduino+simulavr debug library
//
// by Ilja Everilä <saarni@gmail.com>
//
// ChangeLog:
// 19 Oct 2012: Add setup method

#ifndef __DEBUG_H__
#define __DEBUG_H__ 1

# ifndef DEBUGPORT
#  define DEBUGPORT 0x20
# endif
# ifndef DEBUGPIN
#  define DEBUGPIN 0x22
# endif

# ifdef DEBUG
extern void setup_debug();
#  define DEBUG_WRITE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#  define SETUP_DEBUG setup_debug()
# else
#  define DEBUG_WRITE(...) /* __VA_ARGS__ */
#  define SETUP_DEBUG /* setup_debug() */
# endif

#endif
