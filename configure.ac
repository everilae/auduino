AC_PREREQ([2.68])
AC_INIT([arduino], [1.0], [saarni@gmail.com])
AC_CONFIG_SRCDIR([src/auduino.cpp])

# Checks for programs.
AC_PROG_CC([avr-gcc])
AC_PROG_CXX([avr-g++])

# Checks for libraries.
AC_CHECK_LIB([core], [main])
AC_CHECK_LIB([m], [pow])

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_CHECK_FUNCS([itoa])
AC_CHECK_FUNCS([utoa])

# Checks for system services.

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
