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

static int idl_emit_begin(idl_module_t *module);
static int idl_emit_cxxinc_open(idl_module_t *module);
static int idl_emit_cxxinc_begin(idl_module_t *module);
static void idl_emit_cxxinc_close(idl_module_t *module);
static void idl_emit_cxxinc_write_header(idl_module_t *module);
static void idl_emit_cxxinc_write_footer(idl_module_t *module);
static void idl_emit_cxxinc_write_indent(idl_module_t *module);
static void idl_emit_write_expr(idl_module_t *module, FILE *f, const idl_expr_t *expr);
static void idl_emit_write_type(idl_module_t *module, FILE *f, idl_typedecl_t *decl);
static void idl_emit_write_symdef(idl_module_t *module, FILE *f, idl_symdef_t *symdef, const char *fmt, const char *paramprefix, const char *voidstr);
static int idl_emit_write_sym(idl_module_t *module, FILE *f, idl_symdef_t *symdef, const char *fmt);

static int
idl_emit_begin(idl_module_t *module)
{
	if(MODE_UNSPEC == module->mode)
	{
		module->mode = defmode;
	}
	return 0;
}

static int
idl_emit_cxxinc_open(idl_module_t *module)
{
	if(NULL == module->hout)
	{
		if(NULL != module->houtname)
		{
			if(NULL == (module->hout = fopen(module->houtname, "wb")))
			{
				fprintf(stderr, "%s: %s: %s\n", progname, module->houtname, strerror(errno));
				exit(EXIT_FAILURE);
			}
			fprintf(module->hout, "/* Automatically generated from %s by %s */\n\n", module->shortname, progname);
			return 1;
		}
		return 0;
	}
	return 1;
}

static void
idl_emit_cxxinc_close(idl_module_t *module)
{
	if(NULL != module->hout)
	{
		if(0 != module->headerwritten)
		{
			idl_emit_cxxinc_write_footer(module);
		}
		fclose(module->hout);
		module->hout = NULL;
	}
}

static int
idl_emit_cxxinc_begin(idl_module_t *module)
{
	int r;
	
	if(-1 == idl_emit_begin(module))
	{
		return -1;
	}
	if(1 != (r = idl_emit_cxxinc_open(module)))
	{
		return r;
	}
	if(0 == module->headerwritten)
	{
		idl_emit_cxxinc_write_header(module);
		module->headerwritten = 1;
	}
	return 1;
}

static void
idl_emit_cxxinc_write_header(idl_module_t *module)
{
	fprintf(module->hout, "#ifndef %s\n", module->hmacro);
	fprintf(module->hout, "# define %s\n", module->hmacro);
	if(0 == nodefinc && 0 == module->nodefinc)
	{
		if(MODE_RPC == module->mode)
		{
			fprintf(module->hout, "\n# include \"DCE-RPC/idlbase.h\"\n");
			if(0 == nodefimports && 0 == module->nodefimports)
			{
				/* This corresponds to the default import of nbase.idl */
				fprintf(module->hout, "# include \"DCE-RPC/nbase.h\"\n");
			}
		}
		else if(MODE_COM == module->mode)
		{
			fprintf(module->hout, "\n# include \"COM/COM.h\"\n");
		}
		else if(MODE_MSCOM == module->mode)
		{
			fprintf(module->hout, "\n# include <rpc.h>\n");
			fprintf(module->hout, "\n# include <rpcndr.h>\n");
		}
		else if(MODE_DCERPC == module->mode)
		{
			fprintf(module->hout, "\n# include <nbase.h>\n");
		}
		else if(MODE_SUNRPC == module->mode)
		{
			fprintf(module->hout, "\n# include <rpc/rpc.h>\n");
		}
		else if(MODE_XPCOM == module->mode)
		{
			fprintf(module->hout, "\n# ifndef NS_NO_VTABLE\n");
			fprintf(module->hout, "#  define NS_NO_VTABLE\n");
			fprintf(module->hout, "# endif\n");
		}
	}
	fputc('\n', module->hout);
}

static void
idl_emit_cxxinc_write_footer(idl_module_t *module)
{
	size_t c;
	
	fprintf(module->hout, "\n#endif /*!%s*/\n", module->hmacro);
	if(MODE_RPC == module->mode || MODE_COM == module->mode || MODE_MSCOM == module->mode)
	{
		for(c = 0; c < module->nguids; c++)
		{
			fprintf(module->hout, "\n/* %s = {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x} */\n",
				module->guids[c]->name,
				module->guids[c]->data1, module->guids[c]->data2, module->guids[c]->data3,
				module->guids[c]->data4[0], module->guids[c]->data4[1],
				module->guids[c]->data4[2], module->guids[c]->data4[3], module->guids[c]->data4[4],
				module->guids[c]->data4[5], module->guids[c]->data4[6], module->guids[c]->data4[7]);
			fprintf(module->hout, "DEFINE_GUID(%s, 0x%08X, 0x%04X, 0x%04X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X, 0x%02X);\n",
				module->guids[c]->name,
				module->guids[c]->data1, module->guids[c]->data2, module->guids[c]->data3,
				module->guids[c]->data4[0], module->guids[c]->data4[1],
				module->guids[c]->data4[2], module->guids[c]->data4[3], module->guids[c]->data4[4],
				module->guids[c]->data4[5], module->guids[c]->data4[6], module->guids[c]->data4[7]);
		}
	}
}

static void
idl_emit_cxxinc_write_indent(idl_module_t *module)
{
	size_t c;
	
	for(c = 0; c < module->houtdepth; c++)
	{
		fputc('\t', module->hout);
	}
}

static void
idl_emit_cxxinc_write_methods(idl_module_t *module, FILE *f, idl_interface_t *intf)
{
	size_t c;
	int stdmethod;
	
	if(NULL != intf->ancestor)
	{
		idl_emit_cxxinc_write_methods(module, f, intf->ancestor);
		fputc('\n', f);
	}
	idl_emit_cxxinc_write_indent(module);
	fprintf(f, "/* %s */\n", intf->name);
	for(c = 0; c < intf->symlist.ndefs; c++)
	{
		if(1 == idl_intf_method_inherited(intf, intf->symlist.defs[c]->ident))
		{
			continue;
		}
		if(SYM_METHOD == intf->symlist.defs[c]->type)
		{
			stdmethod = 0;
			if(NULL != intf->symlist.defs[c]->decl &&
				NULL != intf->symlist.defs[c]->decl->user_type &&
				0 == strcmp(intf->symlist.defs[c]->decl->user_type->ident, "com_result_t"))
			{
				stdmethod = 1;
			}
			idl_emit_cxxinc_write_indent(module);
			if(stdmethod)
			{
				idl_emit_write_symdef(module, module->hout, intf->symlist.defs[c], "STDMETHOD(%s)", "THIS_ ", "THIS");
			}
			else
			{
				fprintf(f, "STDMETHOD_(");
				idl_emit_write_type(module, f, intf->symlist.defs[c]->decl);
				idl_emit_write_symdef(module, module->hout, intf->symlist.defs[c], ", %s)", "THIS_ ", "THIS");
			}
			fprintf(f, " PURE;\n");
		}
	}
}

static int
idl_emit_cxxinc_write_method_macros(idl_module_t *module, FILE *f, idl_interface_t *curintf, idl_interface_t *intf, int written)
{
	size_t c, d;

	if(MODE_XPCOM == module->mode)
	{
		return 0;
	}
	if(NULL != intf->ancestor)
	{
		written += idl_emit_cxxinc_write_method_macros(module, f, curintf, intf->ancestor, written);
	}
	for(c = 0; c < intf->symlist.ndefs; c++)
	{
		if(SYM_METHOD == intf->symlist.defs[c]->type)
		{
			if(1 == idl_intf_method_inherited(intf, intf->symlist.defs[c]->ident))
			{
				continue;
			}
			if(0 == written)
			{
				if(MODE_MSCOM == module->mode)
				{
					fprintf(f, "#  if defined(CINTERFACE) || !defined(__cplusplus)\n");
				}
				else
				{
					fprintf(f, "#  if defined(COM_CINTERFACE) || !defined(__cplusplus)\n");
				}
				written = 1;
			}
			fprintf(f, "#   define %s_%s(__this", curintf->name, intf->symlist.defs[c]->ident);
			for(d = 0; d < intf->symlist.defs[c]->fp_params.ndefs; d++)
			{
				fprintf(f, ", %s", intf->symlist.defs[c]->fp_params.defs[d]->ident);
			}
			fprintf(f, ") __this->lpVtbl->%s(__this", intf->symlist.defs[c]->ident);
			for(d = 0; d < intf->symlist.defs[c]->fp_params.ndefs; d++)
			{
				fprintf(f, ", %s", intf->symlist.defs[c]->fp_params.defs[d]->ident);
			}
			fputc(')', f);
			fputc('\n', f);
		}
	}
	if(0 != written && curintf == intf)
	{
		if(MODE_MSCOM == module->mode)
		{
			fprintf(f, "#  endif /*CINTERFACE || !__cplusplus*/\n");
		}
		else
		{
			fprintf(f, "#  endif /*COM_CINTERFACE || !__cplusplus*/\n");
		}
	}
	return written;
}

static void
idl_emit_write_expr(idl_module_t *module, FILE *f, const idl_expr_t *expr)
{
	if(EXPR_CONST == expr->type)
	{
		fprintf(f, "%ld", (long) expr->constval);
		return;
	}
	if(EXPR_SYM == expr->type)
	{
		fprintf(f, "%s", expr->symdef->ident);
	}
	if(EXPR_BRACKET == expr->type)
	{
		fputc('(', f);
	}
	if(NULL != expr->left)
	{
		idl_emit_write_expr(module, f, expr->left);
	}
	switch(expr->type)
	{
		case EXPR_ADD: fputc('+', f); break;
		case EXPR_SUB: fputc('-', f); break;
		case EXPR_MUL: fputc('*', f); break;
		case EXPR_DIV: fputc('/', f); break;
		case EXPR_MOD: fputc('%', f); break;
		case EXPR_AND: fputc('&', f); fputc('&', f); break;
		case EXPR_BITAND: fputc('&', f); break;
		case EXPR_OR: fputc('|', f); fputc('|', f); break;
		case EXPR_BITOR: fputc('|', f); break;
		case EXPR_XOR: fputc('^', f); break;
		case EXPR_EQUALS: fputc('=', f); fputc('=', f); break;
		case EXPR_NOTEQUALS: fputc('!', f); fputc('=', f); break;
		case EXPR_LESSEQUALS: fputc('<', f); fputc('=', f); break;
		case EXPR_GTEQUALS: fputc('>', f); break;
		case EXPR_LESS: fputc('<', f); break;
		case EXPR_GT: fputc('>', f); break;
		case EXPR_BITNOT: fputc('~', f); break;
		case EXPR_NOT: fputc('!', f); break;
		case EXPR_LSHIFT: fputc('<', f); fputc('<', f); break;
		case EXPR_RSHIFT: fputc('>', f); fputc('>', f); break;
		case EXPR_IFELSE: fputc('?', f); break;
		case EXPR_BRACKET:
		case EXPR_UNSPEC:
		case EXPR_CONST:
		case EXPR_SYM:
			/* nothing */
			break;
	}
	if(NULL != expr->right)
	{
		idl_emit_write_expr(module, f, expr->right);
	}
	
	if(EXPR_BRACKET == expr->type)
	{
		fputc(')', f);
	}
}


static void
idl_emit_write_type(idl_module_t *module, FILE *f, idl_typedecl_t *decl)
{
	size_t c;
	
	(void) module;
	
	if(NULL == decl || TYPE_NONE == decl->builtin_type)
	{
		return;
	}
	if(decl->modifiers & TYPEMOD_CONST)
	{
		fprintf(f, "const ");
	}
	if(decl->builtin_type == TYPE_CHAR ||
		decl->builtin_type == TYPE_INT ||
		decl->builtin_type == TYPE_LONG ||
		decl->builtin_type == TYPE_LONGLONG)
	{
		if(decl->modifiers & TYPEMOD_UNSIGNED)
		{
			fprintf(f, "unsigned ");
		}
		else if(decl->modifiers & TYPEMOD_SIGNED)
		{
			fprintf(f, "signed ");
		}
	}
	switch(decl->builtin_type)
	{
		case TYPE_VOID:
			fprintf(f, "void");
			break;
		case TYPE_CHAR:
			fprintf(f, "char");
			break;
		case TYPE_INT:
			fprintf(f, "int");
			break;
		case TYPE_LONG:
			fprintf(f, "long");
			break;
		case TYPE_LONGLONG:
			fprintf(f, "long long");
			break;
		case TYPE_SHORT:
			fprintf(f, "short");
			break;
		case TYPE_FLOAT:
			fprintf(f, "float");
			break;
		case TYPE_DOUBLE:
			fprintf(f, "double");
			break;
		case TYPE_BOOLEAN:
			fprintf(f, "uint8_t");
			break;
		case TYPE_STRUCT:
			fprintf(f, "struct %s", decl->tag);
			break;
		case TYPE_UNION:
			fprintf(f, "union %s", decl->tag);
			break;
		case TYPE_ENUM:
			fprintf(f, "enum %s", decl->tag);
			break;
		case TYPE_INTERFACE:
			fprintf(f, "cominterface %s", decl->tag);
			break;
		case TYPE_DEF:
			fprintf(f, "%s", decl->user_type->ident);
			break;
		case TYPE_INT8:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint8_t");
			}
			else
			{
				fprintf(f, "int8_t");
			}
			break;
		case TYPE_INT16:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint16_t");
			}
			else
			{
				fprintf(f, "int16_t");
			}
			break;
		case TYPE_INT32:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint32_t");
			}
			else
			{
				fprintf(f, "int32_t");
			}
			break;
		case TYPE_INT64:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint64_t");
			}
			else
			{
				fprintf(f, "int64_t");
			}
			break;
		default:
			break;
	}
	if(decl->has_symlist)
	{
		fputc('\n', f);
		idl_emit_cxxinc_write_indent(module);
		fputc('{', f);
		fputc('\n', f);
		module->houtdepth++;
		for(c = 0; c < decl->symlist.ndefs; )
		{
			idl_emit_cxxinc_write_indent(module);
			c += idl_emit_write_sym(module, f, decl->symlist.defs[c], NULL);
			if(TYPE_ENUM == decl->builtin_type)
			{
				if(c < decl->symlist.ndefs)
				{
					fputc(',', f);
				}
			}
			else
			{
				fputc(';', f);
			}
			fputc('\n', f);
		}
		module->houtdepth--;
		idl_emit_cxxinc_write_indent(module);
		fputc('}', f);
		fputc(' ', f);
	}
}

static void
idl_emit_write_symdef(idl_module_t *module, FILE *f, idl_symdef_t *symdef, const char *fmt, const char *paramprefix, const char *voidstr)
{
	size_t c;

	(void) module;
	
	if(NULL == paramprefix)
	{
		paramprefix = "";
	}
	if(NULL == voidstr)
	{
		voidstr = "void";
	}
	if(SYM_ENUM == symdef->type)
	{
		/* enum values are handled slightly differently */
		idl_emit_cxxinc_write_indent(module);
		if(symdef->noval)
		{
			fprintf(f, "%s", symdef->ident);
		}
		else
		{
			fprintf(f, "%s = ", symdef->ident);
			idl_emit_write_expr(module, f, symdef->constval);
		}
		return;
	}
	for(c = 0; c < symdef->ndeclarator; c++)
	{
		switch(symdef->declarator[c])
		{
			case DECL_POINTER:
				fputc('*', f);
				if(symdef->declarator[c + 1] == DECL_CONST ||
					symdef->declarator[c + 1] == DECL_RESTRICT)
				{
					fputc(' ', f);
				}
				break;
			case DECL_CONST:
				fprintf(f, "const ");
				break;
			case DECL_RESTRICT:
				fprintf(f, "restrict ");
				break;
			case DECL_LBRACKET:
				fputc('(', f);
				break;
			case DECL_RBRACKET:
				fputc(')', f);
				break;
			case DECL_IDENT:
				fprintf(f, fmt, symdef->ident);
				break;
		}
	}
	if(1 == symdef->is_fp || SYM_METHOD == symdef->type)
	{
		if(0 == symdef->fp_params.ndefs)
		{
			fprintf(f, "(%s)", voidstr);
		}
		else
		{
			fprintf(f, "(%s", paramprefix);
			for(c = 0; c < symdef->fp_params.ndefs; c++)
			{
				/* We never chain in a list of function parameters */
				symdef->fp_params.defs[c]->nextsym = NULL;
				idl_emit_write_sym(module, f, symdef->fp_params.defs[c], NULL);
				if(c < symdef->fp_params.ndefs - 1)
				{
					fputc(',', f);
					fputc(' ', f);
				}
			}
			fputc(')', f);
		}
	}
	if(symdef->is_array)
	{
		fputc('[', f);
		if(symdef->array_len > 0)
		{
			fprintf(f, "%d", (int) symdef->array_len);
		}
		else
		{
			fputc('1', f);
		}
		fputc(']', f);
	}
}

static int
idl_emit_write_sym(idl_module_t *module, FILE *f, idl_symdef_t *symdef, const char *fmt)
{
	idl_symdef_t *p;
	int c;
	
	if(NULL == fmt)
	{
		fmt = "%s";
	}
	c = 0;
	idl_emit_write_type(module, f, symdef->decl);
	fputc(' ', f);
	for(p = symdef; p; p = p->nextsym)
	{
		if(p != symdef)
		{
			fputc(',', module->hout);
			fputc(' ', module->hout);
		}
		idl_emit_write_symdef(module, module->hout, p, fmt, NULL, NULL);
		c++;
	}
	return c;
}

int
idl_emit_init(idl_module_t *module)
{
	(void) module;
	return 0;
}

int
idl_emit_done(idl_module_t *module)
{
	idl_emit_cxxinc_close(module);
	return 0;
}

/* Write a raw string to the generated C/C++ header */
int
idl_emit_cppquote(idl_module_t *module, const char *quote)
{
	if(1 == idl_emit_cxxinc_open(module))
	{
		fprintf(module->hout, "%s\n", quote);
	}
	return 0;
}

/* Write a typdef statement to the generated header */
int
idl_emit_typedef(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef)
{
	(void) intf;
	
	if(1 == idl_emit_cxxinc_begin(module))
	{
		fprintf(module->hout, "typedef ");
		idl_emit_write_sym(module, module->hout, symdef, NULL);
		fputc(';', module->hout);
		fputc('\n', module->hout);
	}
	return 0;
}

int
idl_emit_type(idl_module_t *module, idl_interface_t *intf, idl_typedecl_t *decl)
{
	(void) intf;
	
	if(1 == idl_emit_cxxinc_begin(module))
	{
		idl_emit_write_type(module, module->hout, decl);
		fputc(';', module->hout);
		fputc('\n', module->hout);
	}
	return 0;
}


/* Write a local method declaration to the generated header */
int
idl_emit_local_method(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef)
{
	if(1 == intf->local && 0 == intf->object)
	{
		if(1 == idl_emit_cxxinc_begin(module))
		{
			fprintf(module->hout, "RPC_CEXPORT ");
			idl_emit_write_sym(module, module->hout, symdef, "RPC_SYM(%s)");
			fprintf(module->hout, " RPC_ALIAS(%s)", symdef->ident);
			fputc(';', module->hout);
			fputc('\n', module->hout);
		}
	}
	return 0;
}

int
idl_emit_const(idl_module_t *module, idl_symdef_t *symdef)
{
	if(1 == idl_emit_cxxinc_begin(module))
	{
		fprintf(module->hout, "#  define %s ", symdef->ident);
		idl_emit_write_expr(module, module->hout, symdef->constval);
		fputc('\n', module->hout);
	}
	return 0;
}

/* Write an interface's prologue */

int
idl_emit_intf_prologue(idl_module_t *module, idl_interface_t *intf)
{
	FILE *f;
	unsigned major, minor;
	
	if(1 == idl_emit_cxxinc_begin(module))
	{
		f = module->hout;
		major = (unsigned int) (intf->version >> 16);
		minor = (unsigned int) (intf->version & 0xFFFF);
		fprintf(f, "/* %s version %u.%u */\n", intf->name, major, minor);
		if(MODE_RPC == module->mode || MODE_COM == module->mode)
		{
			fprintf(f, "# ifndef %s_v%u_%u_defined_\n", intf->name, major, minor);
			fprintf(f, "#  define %s_v%u_%u_defined_\n", intf->name, major, minor);
			fprintf(f, "#  undef RPC_EXPORTS\n");
			fprintf(f, "#  ifdef RPC_EXPORT_%s_v%d_%d\n", intf->name, major, minor);
			fprintf(f, "#   define RPC_EXPORTS\n");
			fprintf(f, "#  endif\n");
			if(0 == nodefinc && 0 == module->nodefinc)
			{
				fprintf(f, "#  include \"DCE-RPC/decl.h\"\n\n");
			}
			if(BLOCK_INTERFACE == intf->type && intf->object)
			{
				fprintf(f, "#  undef INTERFACE\n");
				fprintf(f, "#  define INTERFACE %s\n", intf->name);
			}
		}
		if(MODE_MSCOM == module->mode)
		{
			fprintf(f, "# ifndef __%s_INTERFACE_DEFINED__\n", intf->name);
			fprintf(f, "# define __%s_INTERFACE_DEFINED__\n", intf->name);
		}
	}
	return 0;
}

int
idl_emit_intf_epilogue(idl_module_t *module, idl_interface_t *intf)
{
	FILE *f;
	unsigned int major, minor;
	size_t c;
	int first;
	
	if(1 == idl_emit_cxxinc_begin(module))
	{
		f = module->hout;
		major = (unsigned int) (intf->version >> 16);
		minor = (unsigned int) (intf->version & 0xFFFF);
		fputc('\n', f);
		if(1 == intf->local && 0 == intf->object)
		{
			if(MODE_RPC == module->mode || MODE_COM == module->mode)
			{
				first = 1;
				for(c = 0; c < intf->symlist.ndefs; c++)
				{
					if(SYM_METHOD == intf->symlist.defs[c]->type)
					{
						if(first)
						{
							fprintf(f, "#  ifdef RPC_ALIAS_MACROS\n");
							first = 0;
						}
						fprintf(f, "#   define %s RPC_LOCAL_SYM(%s)\n", intf->symlist.defs[c]->ident, intf->symlist.defs[c]->ident);
					}
				}
				if(0 == first)
				{
					fprintf(f, "#  endif /*RPC_ALIAS_MACROS*/\n");
				}
			}
		}
		else if(1 == intf->object)
		{
			if(NULL == intf->ancestor)
			{
				fprintf(f, "DECLARE_INTERFACE(%s)\n", intf->name);
			}
			else
			{
				fprintf(f, "DECLARE_INTERFACE_(%s, %s)\n", intf->name, intf->ancestor->name);
			}
			fprintf(f, "{\n");
			module->houtdepth++;
			idl_emit_cxxinc_write_indent(module);
			fprintf(f, "BEGIN_INTERFACE\n\n");
			idl_emit_cxxinc_write_methods(module, f, intf);
			fputc('\n', f);
			idl_emit_cxxinc_write_indent(module);
			fprintf(f, "END_INTERFACE\n");
			module->houtdepth--;
			fprintf(f, "};\n\n");
			idl_emit_cxxinc_write_method_macros(module, f, intf, intf, 0);
		}
		if(MODE_RPC == module->mode || MODE_COM == module->mode)
		{
			fprintf(f, "\n#  undef RPC_EXPORTS\n");
		}
		if(MODE_RPC == module->mode || MODE_COM == module->mode || MODE_MSCOM == module->mode)
		{
			if(BLOCK_INTERFACE == intf->type && intf->object)
			{
				fprintf(f, "#  undef INTERFACE\n");
			}
		}
		if(MODE_RPC == module->mode || MODE_COM == module->mode)
		{
			fprintf(f, "# endif /*!%s_v%u_%u_defined_*/\n\n", intf->name, major, minor);
		}
		else if(MODE_MSCOM == module->mode)
		{
			fprintf(f, "# endif /*!__%s_INTERFACE_DEFINED__*/\n\n", intf->name);
		}
	}
	return 0;
}
