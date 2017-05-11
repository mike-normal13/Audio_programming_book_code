#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([simple_oscil], [0.1], [bugs@drumwell.net])

AM_INIT_AUTOMAKE([simple_oscil], [0.1])

AC_CONFIG_SRCDIR([simple_oscil.c])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_join])
AC_CHECK_LIB([m], [sin])
AC_CHECK_LIB([portaudio], [Pa_Initialize], [],[
           echo "portaudio library is required for this program"
            exit -1])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_CHECK_FUNCS([pow])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

