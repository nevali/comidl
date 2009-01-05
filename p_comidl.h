/*
 * COM IDL Compiler
 * @(#) $Id$
 */

/*
 * Copyright (c) 2008, 2009 Mo McRoberts.
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

#ifndef P_COMIDL_H_
# define P_COMIDL_H_                   1

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <errno.h>
# include <ctype.h>
# include <stdarg.h>
# ifdef HAVE_STDINT_H
#  include <stdint.h>
# endif
# ifdef HAVE_INTTYPES_H
#  include <inttypes.h>
# endif
# ifdef HAVE_UNISTD_H
#  include <unistd.h>
# endif
# ifdef HAVE_SYS_TYPES_H
#  include <sys/types.h>
# endif
# include <sys/stat.h>
# include <limits.h>

# ifndef HAVE_GETOPT
extern int opterr, optind, optopt, optreset;
extern char *optarg;
extern int getopt(int argc, char * const argv[], const char *options);
# endif

# ifndef YYSTYPE
#  define YYSTYPE char *
# endif

#include "idlparse.h"

typedef char idl_ident_t[64];

# define IDL_IDENT_MAX (sizeof(idl_ident_t) - 1)

typedef struct idl_module_struct idl_module_t;
typedef struct idl_interface_struct idl_interface_t;
typedef struct idl_symlist_struct idl_symlist_t;
typedef struct idl_symdef_struct idl_symdef_t;
typedef struct idl_typedecl_struct idl_typedecl_t;
typedef struct idl_expr_struct idl_expr_t;

typedef struct idl_guid_struct idl_guid_t;

typedef enum
{
	MODE_UNSPEC = 0,
	MODE_RPC,
	MODE_COM,
	MODE_MSCOM,
	MODE_DCERPC,
	MODE_SUNRPC,
	MODE_XPCOM
} idl_mode_t;

typedef enum
{
	BLOCK_UNSPEC = 0,
	BLOCK_INTERFACE,
	BLOCK_MODULE,
	BLOCK_COCLASS,
	BLOCK_LIBRARY,
	BLOCK_DISPINTERFACE
} idl_containertype_t;

typedef enum
{
	PTRCLASS_UNIQUE,
	PTRCLASS_REF,
	PTRCLASS_PTR
} idl_ptrclass_t;

typedef enum
{
	TYPE_NONE,
	TYPE_VOID,
	TYPE_CHAR,
	TYPE_SHORT,
	TYPE_INT,
	TYPE_LONG,
	TYPE_LONGLONG,
	TYPE_FLOAT,
	TYPE_DOUBLE,
	TYPE_BOOLEAN,
	TYPE_STRUCT,
	TYPE_UNION,
	TYPE_ENUM,
	TYPE_INTERFACE,
	TYPE_DEF,
	TYPE_INT8,
	TYPE_INT16,
	TYPE_INT32,
	TYPE_INT64
} idl_builtintype_t;

typedef enum
{
	TYPEMOD_NONE = 0,
	TYPEMOD_SIGNED = (1<<0),
	TYPEMOD_UNSIGNED = (1<<1),
	TYPEMOD_CONST = (1<<2)
} idl_typemod_t;

typedef enum
{
	DECL_POINTER,
	DECL_CONST,
	DECL_RESTRICT,
	DECL_LBRACKET,
	DECL_RBRACKET,
	DECL_IDENT
} idl_declarator_t;

typedef enum
{
	SYM_UNSPEC = 0,
	SYM_TYPEDEF,
	SYM_METHOD,
	SYM_PARAM,
	SYM_MEMBER,
	SYM_CONST,
	SYM_ENUM,
	SYM_STRUCT
} idl_symtype_t;

typedef enum
{
	EXPR_UNSPEC = 0,
	EXPR_CONST,
	EXPR_SYM,
	EXPR_ADD,
	EXPR_SUB,
	EXPR_DIV,
	EXPR_MUL,
	EXPR_MOD,
	EXPR_AND,
	EXPR_BITAND,
	EXPR_OR,
	EXPR_BITOR,
	EXPR_XOR,
	EXPR_EQUALS,
	EXPR_NOTEQUALS,
	EXPR_LESSEQUALS,
	EXPR_GTEQUALS,
	EXPR_LESS,
	EXPR_GT,
	EXPR_BRACKET,
	EXPR_BITNOT,
	EXPR_NOT,
	EXPR_IFELSE,
	EXPR_LSHIFT,
	EXPR_RSHIFT
} idl_exprtype_t;

struct idl_guid_struct
{
	char name[IDL_IDENT_MAX + 16];
	uint32_t data1;
	uint16_t data2;
	uint16_t data3;
	uint8_t data4[8];
};

struct idl_symlist_struct
{
	idl_symtype_t symtype;
	idl_symlist_t *parent;
	idl_symdef_t **defs;
	size_t ndefs;
};

struct idl_module_struct
{
	void *scanner;
	char *filename; /* final filename, e.g., /Library/Frameworks/COM/Headers/wtypes.idl */
	char *shortname; /* short filename, e.g., COM/wtypes.idl */
	char *houtname; /* header output filename, e.g., wtypes.h */
	FILE *hout;
	char *hmacro; /* e.g., COMIDL_WTYPES_H_ */
	size_t houtdepth; /* indenting level */
	idl_mode_t mode; /* output mode */
	idl_guid_t const ** guids; /* GUIDs defined by this module */
	size_t nguids;
	idl_interface_t **interfaces; /* interfaces defined by this module */
	size_t ninterfaces;
	idl_interface_t *curintf;
	idl_symlist_t *cursymlist;
	idl_typedecl_t *typestack[32];
	size_t typestackpos;
	idl_typedecl_t *curtype;
	idl_symdef_t *cursym;
	int nodefinc;
	int nodefimports;
	int headerwritten;
};

struct idl_interface_struct
{
	idl_module_t *module;
	idl_containertype_t type;
	idl_guid_t uuid;
	unsigned long version;
	int local;
	int object;
	idl_ptrclass_t pointer_default;
	idl_ident_t name;
	idl_interface_t *ancestor;
	idl_symlist_t symlist;
	idl_symlist_t *cursymlist;
	idl_symdef_t *firstsym;
};

struct idl_symdef_struct
{
	idl_symtype_t type;
	/* Attributes */
	int local;
	/* Type  */
	idl_builtintype_t const_type;
	idl_typedecl_t *decl;
	/* Declarator */
	size_t ndeclarator;
	idl_declarator_t declarator[16];
	/* Identifier */
	idl_ident_t ident;
	/* Implied attributes */
	/* For methods and function pointers, the parameter lists */
	int is_fp;
	idl_symlist_t fp_params;
	/* Array attributes */
	int is_array;
	ssize_t array_len;
	/* For constants, the constant value. Constants may only be 'small' or 'long' */
	idl_expr_t *constval;
	int noval; /* Enums with no explicit value */
	/* For chained symbols, the next symbol in the chain */
	idl_symdef_t *nextsym;
};

struct idl_typedecl_struct
{
	/* Attributes */
	idl_builtintype_t builtin_type;
	idl_symdef_t *user_type;
	/* Modifiers */
	idl_typemod_t modifiers;
	/* Tag for enums, structs, unions */
	idl_ident_t tag;
	/* Symbol list for enums, structs and unions */
	int has_symlist;
	idl_symlist_t symlist;
};

struct idl_expr_struct
{
	idl_exprtype_t type;
	int isconst;
	long constval;
	const idl_expr_t *left, *right, *alt;
	idl_symdef_t *symdef;
};

extern idl_module_t *curmod;
extern const char *progname;
extern int nostdinc;
extern int nodefimports;
extern int nodefinc;
extern idl_mode_t defmode;

# define YY_DECL int yylex(YYSTYPE *yylval_param, void *yyscanner)
extern YY_DECL;

extern int yylex_init(void **scanner);
extern int yylex_destroy(void *scanner);
extern int yyparse(void *scanner);
extern void yyrestart(FILE *f, void *scanner);
extern char *yyget_text (void *yyscanner);
extern int yyget_lineno (void *yyscanner);

extern idl_mode_t idl_mode_parse(const char *modestr);
extern int idl_parse(const char *src, const char *hout, int defimp, int useinc);

extern int idl_keyword_lookup(const char *s);

extern void idl_module_vmsg(idl_module_t *module, int line, const char *prefix, const char *fmt, va_list ap);
extern void idl_module_error(idl_module_t *module, int line, const char *fmt, ...);
extern void idl_module_errmsg(idl_module_t *module, int line, const char *fmt, ...);
extern void idl_module_warning(idl_module_t *module, int line, const char *fmt, ...);
extern void idl_module_terminate(void);

extern idl_module_t *idl_module_create(const char *filename, const char *hout);
extern int idl_module_done(idl_module_t *module);
extern idl_module_t *idl_module_lookup(const char *pathname);
extern idl_interface_t *idl_module_lookupintf(const char *name);
extern int idl_module_addintf(idl_module_t *module, idl_interface_t *intf);
extern int idl_module_doneintf(idl_module_t *module, idl_interface_t *intf);
extern int idl_module_addguid(idl_module_t *module, const idl_guid_t *guid);
extern idl_typedecl_t *idl_module_typedecl_push(idl_module_t *module);
extern idl_typedecl_t *idl_module_typedecl_pop(idl_module_t *module);
extern idl_symdef_t *idl_module_symdef_create(idl_module_t *module, idl_symlist_t *symlist, idl_typedecl_t *typedecl);
extern int idl_module_symdef_add(idl_module_t *module, idl_symlist_t *symlist, idl_symdef_t *sym);
extern int idl_module_symdef_done(idl_module_t *module, idl_symlist_t *symlist, idl_symdef_t *sym);
extern int idl_module_symdef_link(idl_module_t *module, idl_symlist_t *symlist, idl_symdef_t *symdef);
extern idl_symdef_t *idl_module_symdef_lookup(idl_module_t *module, idl_symlist_t *start, const char *name, int recurse);
extern int idl_module_symlist_push(idl_module_t *module, idl_symlist_t *symlist);
extern int idl_module_symlist_pop(idl_module_t *module, idl_symlist_t *symlist);
extern int idl_module_set_mode(idl_module_t *module, int line, const char *mode);

extern idl_interface_t *idl_intf_create(idl_module_t *module);
extern int idl_intf_done(idl_interface_t *intf);
extern int idl_intf_uuid(idl_interface_t *intf, const char *uuid);
extern int idl_intf_name(idl_interface_t *intf, const char *name);
extern int idl_intf_started(idl_interface_t *intf);
extern int idl_intf_finished(idl_interface_t *intf);
extern idl_interface_t *idl_intf_lookup(const char *name);
extern idl_symdef_t *idl_intf_symdef_create(idl_interface_t *intf, idl_typedecl_t *typedecl);
extern int idl_intf_symdef_done(idl_interface_t *intf, idl_symdef_t *symdef);
extern int idl_intf_symdef_link(idl_interface_t *intf, idl_symdef_t *symdef);
extern idl_symdef_t *idl_intf_symdef_lookup(idl_interface_t *intf, const char *name);
extern int idl_intf_symlist_push(idl_interface_t *intf, idl_symlist_t *symlist);
extern int idl_intf_symlist_pop(idl_interface_t *intf, idl_symlist_t *symlist);
extern int idl_intf_method_exists(idl_interface_t *intf, const char *methodname);
extern int idl_intf_method_exists_recurse(idl_interface_t *intf, const char *methodname);
extern int idl_intf_method_inherited(idl_interface_t *intf, const char *methodname);

extern int idl_intf_write_typedef(idl_interface_t *intf, idl_symdef_t *symdef);
extern int idl_intf_write_method(idl_interface_t *intf, idl_symdef_t *symdef);
extern int idl_intf_write_cppquote(idl_interface_t *intf, const char *s);
extern int idl_intf_write_type(idl_interface_t *intf, idl_typedecl_t *decl);

extern int idl_emit_init(idl_module_t *module);
extern int idl_emit_done(idl_module_t *module);
extern int idl_emit_cppquote(idl_module_t *module, const char *quote);
extern int idl_emit_typedef(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef);
extern int idl_emit_type(idl_module_t *module, idl_interface_t *intf, idl_typedecl_t *decl);
extern int idl_emit_local_method(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef);
extern int idl_emit_const(idl_module_t *module, idl_symdef_t *symdef);
extern int idl_emit_intf_prologue(idl_module_t *module, idl_interface_t *intf);
extern int idl_emit_intf_epilogue(idl_module_t *module, idl_interface_t *intf);

extern int idl_incpath_reset(void);
extern int idl_incpath_add_includedir(const char *path);
extern int idl_incpath_add_frameworkdir(const char *path);
extern int idl_incpath_locate(char *buf, size_t buflen, const char *path);

extern idl_expr_t *idl_expr_create(void);
extern idl_expr_t *idl_expr_create_intconst(long value);
extern idl_expr_t *idl_expr_create_pair(const idl_expr_t *left, idl_exprtype_t op, const idl_expr_t *right);
extern idl_expr_t *idl_expr_create_bracket(const idl_expr_t *expr);
extern idl_expr_t *idl_expr_create_prefix(idl_exprtype_t op, const idl_expr_t *expr);
extern idl_expr_t *idl_expr_create_sym(idl_interface_t *curintf, const char *sym);

#endif /* !P_COMIDL_H_ */
