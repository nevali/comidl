##
## COM IDL Compiler
## @(#) $Id$
##

AM_YFLAGS = -d
BUILT_SOURCES = idlparse.h

bin_PROGRAMS = comidl

comidl_CPPFLAGS = @IDL_CPPFLAGS@
comidl_SOURCES = p_comidl.h module.c interface.c idlparse.y lexidl.l comidl.c emit.c getopt.c

