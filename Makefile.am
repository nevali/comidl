##
## COM IDL Compiler
## @(#) $Id$
##

AM_YFLAGS = -d
BUILT_SOURCES = idlparse.h

bin_PROGRAMS = comidl

comidl_CPPFLAGS = @IDL_CPPFLAGS@
comidl_SOURCES = p_comidl.h \
	idlparse.y lexidl.l \
	keywords.c  module.c interface.c  comidl.c emit.c getopt.c incpath.c \
	expr.c

