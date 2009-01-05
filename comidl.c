/*
 * COM IDL Compiler
 * @(#) $Id$
 */

/*
 * Copyright (c) 2008 Mo McRoberts.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the author(s) of this software may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_comidl.h"

const char *progname = "comidl";
idl_module_t *curmod;

/* Global options */

int nodefimports = 0;
int nodefinc = 0;
idl_mode_t defmode = MODE_UNSPEC;

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [OPTIONS] SOURCE\n"
		"OPTIONS is one or more of:\n"
		"  -H FILE                  Write inteface definition C/C++ header to FILE\n"
		"  -P FILE                  Write proxy to FILE\n"
		"  -S FILE                  Write stub to FILE\n"
		"  -I PATH                  Add PATH to the import search path\n"
		"  -F PATH                  Add PATH to the framework search path\n"
		"  -nostdinc                Reset the import and framework search paths\n"
		"  -nodefimports            Don't perform default imports\n"
		"  -nodefinc                Omit default includes from generated C/C++ headers\n"
		"  -Wp,OPTIONS              Pass OPTIONS to the C preprocessor\n"
		"  -v                       Display version information and exit\n"
		"  -h                       Display this message and exit\n"
		"  -X LANGUAGE              Output for LANGUAGE [default=C]\n"
		"  -M MODE                  Specify default output mode: [default=com]\n"
		"    com                      libCOM\n"
		"    rpc                      libDCE-RPC\n"
		"    mscom                    Microsoft COM/ReactOS/WINE\n"
		"    dcerpc                   DCE RPC/FreeDCE\n"
		"    sunrpc                   Sun ONC RPC\n"
		"    xpcom                    Mozilla XPCOM\n"
		"  Note: specified mode value may be overridden within IDL\n",
		progname);
}

static void
version(void)
{
	fprintf(stderr, "@(#) $Id$\n\n");
	fprintf(stderr, "Copyright (c) 2008 Mo McRoberts\n");
	fprintf(stderr, "Copyright (c) 2007 Novell, Inc. All rights reserved.\n");
	fprintf(stderr, "Copyright (c) 1989 Open Software Foundation, Inc.\n");
	fprintf(stderr, "Copyright (c) 1989 Hewlett-Packard Company\n");
	fprintf(stderr, "Copyright (c) 1989 Digital Equipment Corporation\n");
	#ifndef HAVE_GETOPT
		fprintf(stderr, "Copyright (c) 1987, 1993, 1994\nThe Regents of the University of California.  All rights reserved.\n");
	#endif
	fprintf(stderr, "\nhttp://libcom.googlecode.com/\n");
}

idl_mode_t
idl_mode_parse(const char *modestr)
{
	if(0 == strcmp(modestr, "com"))
	{
		return MODE_COM;
	}
	if(0 == strcmp(modestr, "rpc"))
	{
		return MODE_RPC;
	}
	if(0 == strcmp(modestr, "mscom"))
	{
		return MODE_MSCOM;
	}
	if(0 == strcmp(modestr, "dcerpc"))
	{
		return MODE_DCERPC;
	}
	if(0 == strcmp(modestr, "sunrpc") || 0 == strcmp(modestr, "oncrpc"))
	{
		return MODE_SUNRPC;
	}
	if(0 == strcmp(modestr, "xpcom"))
	{
		return MODE_XPCOM;
	}
	return MODE_UNSPEC;
}

int
idl_builtin_create(idl_module_t *mod, idl_interface_t *intf, const char *name, idl_builtintype_t type, idl_typemod_t mods)
{
	idl_typedecl_t *typedecl;
	idl_symdef_t *sym;
	
	typedecl = idl_module_typedecl_push(mod);
	typedecl->builtin_type = type;
	typedecl->modifiers = mods;
	sym = idl_module_symdef_create(mod, &(intf->symlist), typedecl);
	sym->type = SYM_TYPEDEF;
	strncpy(sym->ident, name, IDL_IDENT_MAX);
	sym->ident[IDL_IDENT_MAX] = 0;
	idl_module_symdef_add(mod, &(intf->symlist), sym);
	idl_module_typedecl_pop(mod);
	return 0;
}

int
idl_builtin(void)
{
	idl_module_t *mod;
	idl_interface_t *builtins;
	
	mod = idl_module_create("*Built-in*", NULL);
	builtins = idl_intf_create(mod);
	idl_builtin_create(mod, builtins, "int8_t", TYPE_INT8, TYPEMOD_SIGNED);
	idl_builtin_create(mod, builtins, "int16_t", TYPE_INT16, TYPEMOD_SIGNED);
	idl_builtin_create(mod, builtins, "int32_t", TYPE_INT32, TYPEMOD_SIGNED);
	idl_builtin_create(mod, builtins, "int64_t", TYPE_INT64, TYPEMOD_SIGNED);
	idl_builtin_create(mod, builtins, "uint8_t", TYPE_INT8, TYPEMOD_UNSIGNED);
	idl_builtin_create(mod, builtins, "uint16_t", TYPE_INT16, TYPEMOD_UNSIGNED);
	idl_builtin_create(mod, builtins, "uint32_t", TYPE_INT32, TYPEMOD_UNSIGNED);
	idl_builtin_create(mod, builtins, "uint64_t", TYPE_INT64, TYPEMOD_UNSIGNED);
	return 0;
}

int
idl_parse(const char *src, const char *hout, int defimp, int useinc)
{
	FILE *f;
	char fpath[PATH_MAX + 1];
	idl_module_t *lastmod;
	void *scanner;
	
	if(0 == useinc || src[0] == '/')
	{
		if(strlen(src) > PATH_MAX)
		{
			fprintf(stderr, "%s: %s: path too long\n", progname, src);
			return -1;
		}
		strcpy(fpath, src);
	}
	else
	{
		if(-1 == idl_incpath_locate(fpath, sizeof(fpath), src))
		{
			fprintf(stderr, "%s: %s: %s\n", progname, src, strerror(ENOENT));
			return -1;
		}
	}
	if(NULL == (f = fopen(fpath, "rb")))
	{
		fprintf(stderr, "%s: %s: %s\n", progname, fpath, strerror(errno));
		return -1;
	}
	if(1 == defimp && 0 == nodefimports)
	{
		if(-1 == idl_parse("nbase.idl", NULL, 0, 1))
		{
			fclose(f);
			return -1;
		}
	}
	if(NULL != idl_module_lookup(fpath))
	{
		fprintf(stderr, "%s has already been imported\n", fpath);
		return 0;
	}
	lastmod = curmod;
	curmod = idl_module_create(fpath, hout);
	curmod->scanner = scanner;
	yylex_init(&scanner);
	yyrestart(f, scanner);
	yyparse(scanner);
	fclose(f);
	idl_module_done(curmod);
	curmod->scanner = NULL;
	curmod = lastmod;
	return 0;
}

int
main(int argc, char **argv)
{
	const char *srcfile, *intfheader, *t;
	char *ih, *s;
	int defaults, c;
	idl_mode_t mode;
	
	if(NULL != argv[0])
	{
		progname = argv[0];
		if(NULL != (t = strrchr(progname, '/')))
		{
			t++;
			progname = t;
		}
	}
	argv[0] = (char *) progname;
	srcfile = NULL;
	intfheader = NULL;
	defaults = 1;
	while((c = getopt(argc, argv, "H:P:S:I:F:n:W:X:M:hv")) != -1)
	{
		switch(c)
		{
			case 'H':
				intfheader = optarg;
				defaults = 0;
				break;
			case 'I':
				idl_incpath_add_includedir(optarg);
				break;
			case 'F':
				idl_incpath_add_frameworkdir(optarg);
				break;
			case 'n':
				if(0 == strcmp(optarg, "ostdinc"))
				{
					idl_incpath_reset();
				}
				else if(0 == strcmp(optarg, "odefimports"))
				{
					nodefimports = 1;
				}
				else if(0 == strcmp(optarg, "odefinc"))
				{
					nodefinc = 1;
				}
				else
				{
					fprintf(stderr, "%s: illegal option -- n%s\n", progname, optarg);
					usage();
					exit(EXIT_FAILURE);
				}
				break;
			case 'M':
				if(MODE_UNSPEC == (mode = idl_mode_parse(optarg)))
				{
					fprintf(stderr, "%s: invalid mode -- %s\n", progname, optarg);
					usage();
					exit(EXIT_FAILURE);
				}
				if(defmode != MODE_UNSPEC && defmode != mode)
				{
					fprintf(stderr, "%s: output mode cannot be specified more than once\n", progname);
					exit(EXIT_FAILURE);
				}
				defmode = mode;
				break;
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			case 'v':
				version();
				exit(EXIT_SUCCESS);
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}
	argv += optind;
	argc -= optind;
	if(argc != 1)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	srcfile = argv[0];
	ih = NULL;
	if(defaults)
	{
		if(NULL == (t = strrchr(srcfile, '/')))
		{
			t = srcfile;
		}
		else
		{
			t++;
		}
		ih = (char *) malloc(strlen(t + 8));
		strcpy(ih, t);
		if(strlen(ih) > 4)
		{
			s = strchr(ih, 0);
			s -= 4;
			if(0 == strcasecmp(s, ".idl"))
			{
				s[0] = '.';
				s[1] = 'h';
				s[2] = 0;
			}
			else
			{
				strcat(ih, ".h");
			}
		}
		else
		{
			strcat(ih, ".h");
		}
		intfheader = ih;
	}
	if(MODE_UNSPEC == defmode)
	{
		defmode = MODE_COM;
	}
/*	idl_incpath_addincludedir("/usr/include");
	idl_incpath_addframeworkdir("/System/Library/Frameworks");
	idl_incpath_addframeworkdir("/Library/Frameworks"); */
	curmod = NULL;
	idl_builtin();
	if(-1 == idl_parse(srcfile, intfheader, 1, 0))
	{
		exit(EXIT_FAILURE);
	}
	free(ih);
	return 0;
}
