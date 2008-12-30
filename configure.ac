dnl comidl: The libcom IDL compiler
dnl @(#) $Id$

dnl Copyright (c) 2008 Mo McRoberts.
dnl
dnl Redistribution and use in source and binary forms, with or without
dnl modification, are permitted provided that the following conditions
dnl are met:
dnl 1. Redistributions of source code must retain the above copyright
dnl    notice, this list of conditions and the following disclaimer.
dnl 2. Redistributions in binary form must reproduce the above copyright
dnl    notice, this list of conditions and the following disclaimer in the
dnl    documentation and/or other materials provided with the distribution.
dnl 3. The names of the author(s) of this software may not be used to endorse
dnl    or promote products derived from this software without specific prior
dnl    written permission.
dnl
dnl THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
dnl INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
dnl AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
dnl AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
dnl SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
dnl TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
dnl PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
dnl LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
dnl NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
dnl SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

AC_INIT(comidl, 1.0)
AC_CONFIG_SRCDIR([comidl.c])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([foreign])
AM_MAINTAINER_MODE


CPPFLAGS="$CPPFLAGS -W -Wall -D_REENTRANT"
export CPPFLAGS

CFLAGS="$CFLAGS -g -O0"
export CFLAGS

AC_PROG_CC
AM_PROG_LEX
AC_PROG_YACC
AC_PROG_SED

IDL_CPPFLAGS='-D_GNU_SOURCE -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64'
IDL_CC="$CC"
if test x"${ac_cv_prog_gcc}" = x"yes" ; then
	IDL_CPP="$CPP -x c-header"
else
	IDL_CPP=""
fi

AC_SUBST([IDL_CPPFLAGS])
AC_SUBST([IDL_CC])
AC_SUBST([IDL_CPP])

AC_CHECK_HEADERS([stdlib.h stdint.h unistd.h inttypes.h unistd.h stdarg.h])

AC_CONFIG_HEADER([config.h])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
