%{
/* COM IDL Compiler
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

/*
 * Copyright (c) 2007, Novell, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Novell, Inc. nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * 
 * (c) Copyright 1989 OPEN SOFTWARE FOUNDATION, INC.
 * (c) Copyright 1989 HEWLETT-PACKARD COMPANY
 * (c) Copyright 1989 DIGITAL EQUIPMENT CORPORATION
 * To anyone who acknowledges that this file is provided "AS IS"
 * without any express or implied warranty:
 *                 permission to use, copy, modify, and distribute this
 * file for any purpose is hereby granted without fee, provided that
 * the above copyright notices and this notice appears in all source
 * code copies, and that none of the names of Open Software
 * Foundation, Inc., Hewlett-Packard Company, or Digital Equipment
 * Corporation be used in advertising or publicity pertaining to
 * distribution of the software without specific, written prior
 * permission.  Neither Open Software Foundation, Inc., Hewlett-
 * Packard Company, nor Digital Equipment Corporation makes any
 * representations about the suitability of this software for any
 * purpose.
 * 
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_comidl.h"

static char vertmp[64];

void
yyerror(char *s)
{
	idl_module_error(curmod, yylineno, "%s", s);
}

%}

/********************************************************************/
/*                                                                  */
/*          Tokens used by the IDL parser.                          */
/*                                                                  */
/********************************************************************/


/* Keywords                 */
%token ALIGN_KW
%token BYTE_KW
%token CHAR_KW
%token CONST_KW
%token DEFAULT_KW
%token ENUM_KW
%token EXCEPTIONS_KW
%token FLOAT_KW
%token HYPER_KW
%token INT_KW
%token INTERFACE_KW
%token IMPORT_KW
%token LIBRARY_KW
%token LONG_KW
%token PIPE_KW
%token REF_KW
%token SMALL_KW
%token STRUCT_KW
%token TYPEDEF_KW
%token UNION_KW
%token UNSIGNED_KW
%token SHORT_KW
%token VOID_KW
%token DOUBLE_KW
%token BOOLEAN_KW
%token CASE_KW
%token SWITCH_KW
%token HANDLE_T_KW
%token TRUE_KW
%token FALSE_KW
%token NULL_KW
%token BROADCAST_KW
%token COMM_STATUS_KW
%token CONTEXT_HANDLE_KW
%token FIRST_IS_KW
%token HANDLE_KW
%token IDEMPOTENT_KW
%token IGNORE_KW
%token CALL_AS_KW
%token IID_IS_KW
%token IMPLICIT_HANDLE_KW
%token IN_KW
%token LAST_IS_KW
%token LENGTH_IS_KW
%token LOCAL_KW
%token MAX_IS_KW
%token MAYBE_KW
%token MIN_IS_KW
%token MUTABLE_KW
%token OUT_KW
%token OBJECT_KW
%token POINTER_DEFAULT_KW
%token ENDPOINT_KW
%token PTR_KW
%token RANGE_KW
%token REFLECT_DELETIONS_KW
%token REMOTE_KW
%token SECURE_KW
%token SHAPE_KW
%token SIGNED_KW
%token SIZE_IS_KW
%token STRING_KW
%token SWITCH_IS_KW
%token SWITCH_TYPE_KW
%token TRANSMIT_AS_KW
%token UNIQUE_KW
%token UUID_KW
%token VERSION_KW
%token V1_ARRAY_KW
%token V1_STRING_KW
%token V1_ENUM_KW
%token V1_STRUCT_KW
%token CPP_QUOTE_KW
%token WIRE_MARSHAL_KW
%token RESTRICT_KW

/*  Non-keyword tokens      */

%token UUID


/*  Punctuation             */

%token COLON
%token COMMA
%token DOTDOT
%token EQUAL
%token LBRACE
%token LBRACKET
%token LPAREN
%token RBRACE
%token RBRACKET
%token RPAREN
%token SEMI
%token STAR
%token QUESTION
%token BAR
%token BARBAR
%token LANGLE
%token LANGLEANGLE
%token RANGLE
%token RANGLEANGLE
%token AMP
%token AMPAMP
%token LESSEQUAL
%token GREATEREQUAL
%token EQUALEQUAL
%token CARET
%token PLUS
%token MINUS
%token NOT
%token NOTEQUAL
%token SLASH
%token PERCENT
%token TILDE
%token POUND
%token UNKNOWN  /* Something that doesn't fit in any other token class */

%token IDENTIFIER
%token STRING
%token INTEGER_NUMERIC
%token FLOAT_NUMERIC

%start module

%%

module:
		interfaces
	|	optional_imports
	|	cpp_quote
	;

interfaces:
		interfaces interface
	|	interface
	;

interface:
		interface_init interface_start interface_ancestor interface_tail
		{
			idl_intf_done(curmod->curintf);
		}
	;

interface_start:
		interface_attributes INTERFACE_KW IDENTIFIER
		{
			curmod->curintf->type = BLOCK_INTERFACE;
			idl_intf_name(curmod->curintf, $3);
			idl_intf_started(curmod->curintf);
		}
	;

interface_ancestor:
		/* Nothing */
		{
			idl_intf_write_prolog(curmod->curintf);
		}
		| COLON IDENTIFIER
		{
			fprintf(stderr, "interface inherits from %s\n", $2);
			/* XXX look up ancestor */
			idl_intf_write_prolog(curmod->curintf);
		}
	;

interface_init:
		{
			idl_intf_create(curmod);
		}
	;

interface_tail:
		LBRACE interface_body RBRACE
		{ idl_intf_write_epilog(curmod->curintf); }
	|	error
		{
			$$ = NULL;
			fprintf(stderr, "Parse error (2)\n");
		}
	|	error RBRACE
		{
			$$ = NULL;
			fprintf(stderr, "Parse error (3)\n");
		}
	;


interface_body:
		optional_imports exports extraneous_semi:
	;

/*
 * Interface Attributes
 *
 * Interface attributes are special--there is no cross between interface
 * attributes and other attributes (for instance on fields or types.
 */
interface_attributes:
        attribute_opener interface_attr_list extraneous_comma attribute_closer
		{
		}
    |   attribute_opener error attribute_closer
        {
			fprintf(stderr, "Some error occurred while reading attributes (%s)\n", $2);
        }

    |   /* Nothing */
    ;

interface_attr_list:
        interface_attr
    |   interface_attr_list COMMA interface_attr
    |   /* nothing */
    ;

interface_attr:
        UUID_KW error
        {
			fprintf(stderr, "Parse error (1)\n");
        }
    |   UUID_KW uuid
        {
			idl_intf_uuid(curmod->curintf, $2);
        }
    |   ENDPOINT_KW LPAREN port_list extraneous_comma RPAREN
        {
        }
    |   EXCEPTIONS_KW LPAREN excep_list extraneous_comma RPAREN
        {
        }
    |   VERSION_KW LPAREN version_number RPAREN
        {
			char *dummy;
			
			curmod->curintf->version = strtoul($3, &dummy, 0);
			$$ = $3;
        }
    |   LOCAL_KW
        {
			curmod->curintf->local = 1;
			$$ = $1;
        }
	|	OBJECT_KW
		{
			curmod->curintf->object = 1;
		}
    |   POINTER_DEFAULT_KW LPAREN pointer_class RPAREN
        {
			$$ = $3;
        }
    ;

pointer_class:
	        REF_KW {  }
	    |   PTR_KW {  }
	    |   UNIQUE_KW { }
	    ;

version_number:
        INTEGER_NUMERIC
        {
			$$ = $1;
        }
   |    FLOAT_NUMERIC
        {
			unsigned long major_version, minor_version;
			
			sscanf($1,"%lu.%lu",&major_version,&minor_version);
			major_version &= 0xFFFF;
			minor_version &= 0xFFFF;
            sprintf(vertmp, "%lu", (major_version << 16) | minor_version);
			$$ = vertmp;
        }
    ;

port_list:
        port_spec
    |   port_list COMMA port_spec
    ;

excep_list:
        excep_spec
        {
        }
    |   excep_list COMMA excep_spec
        {
        }
    ;

port_spec:
        STRING
        {
        }
    ;

excep_spec:
        IDENTIFIER
        {
        }
    ;

optional_imports:
        imports
    |   /* Nothing */
        {
        }
    ;

imports:
        import
    |   imports import
        {
			fprintf(stderr, "Importing %s\n", $2);
        }
    ;

import:
        IMPORT_KW error
        {
        }
    |   IMPORT_KW error SEMI
        {
        }
    |   IMPORT_KW import_files SEMI
        {
			$$ = $2;
        }
    ;

import_files:
        import_file
    |   import_files COMMA import_file
        {
        }
    ;



import_file:
        STRING
        {
			fprintf(stderr, "Performing import of %s\n", $1);
        }
    ;


exports:
		/* Nothing */
	|	export
	|	exports extraneous_semi export
	;

export:
		cpp_quote
	|	typedef_decl
	|	method_decl
	|	error
		{
			fprintf(stderr, "Parse error during exports\n");
		}
	;

attribute_opener:
        LBRACKET
        {
        }
    ;

attribute_closer:
        RBRACKET
        {
        }
    ;


extraneous_comma:
        /* Nothing */
    |   COMMA
        {
		}
    ;

extraneous_semi:
        /* Nothing */
    |   SEMI
        {
		}
    ;

typedef_decl:
		TYPEDEF_KW typedef_init type_decl pointer_ident_decl_list SEMI
		{
			idl_module_typedecl_pop(curmod);
			idl_intf_write_typedef(curmod->curintf, curmod->curintf->firstsym);
			curmod->curintf->firstsym = NULL;
		}
	;

method_decl:
		method_attributes type_decl method_init LPAREN fp_args_init possible_arg_list RPAREN SEMI
		{
			idl_symdef_t *fn;
			
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
			fn = curmod->cursymlist->defs[curmod->cursymlist->ndefs - 1];
			idl_intf_write_method(curmod->curintf, fn);
		}
	;
	
method_attributes:
		attribute_opener method_attr_list extraneous_comma attribute_closer
	|	/* Nothing */
	;

method_attr_list:
        method_attr
    |   method_attr_list COMMA method_attr
    |   /* nothing */
    ;

method_attr:
	|	CALL_AS_KW
	|	LOCAL_KW
	;
	
	
method_init: 
		IDENTIFIER
		{
			curmod->cursymlist->symtype = SYM_METHOD;
			idl_intf_symdef_create(curmod->curintf, curmod->curtype);
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_IDENT;
			curmod->cursym->ndeclarator++;
			strncpy(curmod->cursym->ident, $1, IDL_IDENT_MAX);
			curmod->cursym->ident[IDL_IDENT_MAX] = 0;
			/* Inherit attributes from the interface */
			curmod->cursym->local = curmod->curintf->local;
		}
	;
	
typedef_init:
		{
			curmod->cursymlist->symtype = SYM_TYPEDEF;
		}
	;
	
/* A single declaration (i.e., a function parameter) */
pointer_ident_decl:
		symdef_init declarator LPAREN fp_args_init possible_arg_list RPAREN
		{
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
		}
	|	symdef_init declarator
		{
			if(0 != curmod->cursym->is_fp)
			{
				idl_module_error(curmod, yylineno, "Symbol '%s' was declared as a function pointer but no arguments have been specified", curmod->cursym->ident);
			}
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
		}
	;
	
/* One or more declarations in a comma-separated list */
pointer_ident_decl_list:
		symdef_init declarator LPAREN fp_args_init_first possible_arg_list RPAREN
		{
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
		}
	|	symdef_init declarator
		{
			if(0 != curmod->cursym->is_fp)
			{
				idl_module_error(curmod, yylineno, "[symdef_init declarator] Symbol '%s' was declared as a function pointer but no arguments have been specified", curmod->cursym->ident);
			}
			curmod->curintf->firstsym = curmod->cursym;
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
		}
	|	pointer_ident_decl_list COMMA symdef_init declarator LPAREN fp_args_init_link possible_arg_list RPAREN
		{
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
		}
	|	pointer_ident_decl_list COMMA symdef_init declarator
		{
			if(0 != curmod->cursym->is_fp)
			{
				idl_module_error(curmod, yylineno, "Symbol '%s' was declared as a function pointer but no arguments have been specified", curmod->cursym->ident);
			}
			idl_intf_symdef_link(curmod->curintf, curmod->cursym);
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
		}
	|	error
		{
			fprintf(stderr, "Parse error on line %d while parsing pointer_ident_decl_list\n", yylineno);
		}
	;
	
declarator:
		pointer_decl direct_declarator
	|	direct_declarator
	|	error
		{
			fprintf(stderr, "Error in declarator\n");
		}
	;

direct_declarator:
		IDENTIFIER
		{
			strncpy(curmod->cursym->ident, $1, IDL_IDENT_MAX);
			curmod->cursym->ident[IDL_IDENT_MAX] = 0;
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_IDENT;
			curmod->cursym->ndeclarator++;
		}
	|	LPAREN fnpointer_init declarator RPAREN
		{
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_RBRACKET;
			curmod->cursym->ndeclarator++;
		}
	|	error
		{
			fprintf(stderr, "Error in direct_declarator\n");
		}
	;

fnpointer_init:
		{
			curmod->cursym->is_fp = 1;
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_LBRACKET;
			curmod->cursym->ndeclarator++;
		}
	;

fp_args_init:
		{
			idl_symdef_t *fp;
			
			fp = curmod->cursym;
			if(0 == curmod->cursym->is_fp && SYM_METHOD != curmod->cursym->type)
			{
				idl_module_error(curmod, yylineno, "Cannot declare arguments for non-function-pointer symbol '%s'", curmod->cursym->ident);
			}
			fp->fp_params.symtype = SYM_PARAM;
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
			idl_intf_symlist_push(curmod->curintf, &(fp->fp_params));
		}
	;

fp_args_init_first:
		{
			idl_symdef_t *fp;
			
			fp = curmod->cursym;
			if(0 == curmod->cursym->is_fp)
			{
				idl_module_error(curmod, yylineno, "Cannot declare arguments for non-function-pointer symbol '%s'", curmod->cursym->ident);
			}
			curmod->curintf->firstsym = curmod->cursym;
			fp->fp_params.symtype = SYM_PARAM;
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
			idl_intf_symlist_push(curmod->curintf, &(fp->fp_params));
		}
	;
	
fp_args_init_link:
		{
			idl_symdef_t *fp;
			
			fp = curmod->cursym;
			if(0 == curmod->cursym->is_fp)
			{
				idl_module_error(curmod, yylineno, "Cannot declare arguments for non-function-pointer symbol '%s'", curmod->cursym->ident);
			}
			fp->fp_params.symtype = SYM_PARAM;
			idl_intf_symdef_link(curmod->curintf, curmod->cursym);
			idl_intf_symdef_done(curmod->curintf, curmod->cursym);
			idl_intf_symlist_push(curmod->curintf, &(fp->fp_params));
		}
	;

pointer_decl:
		pointer_decl_token
	|	pointer_decl_token pointer_decl
	;

pointer_decl_token:
		STAR
		{
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_POINTER;
			curmod->cursym->ndeclarator++;
			$$ = $1;
		}
	|	CONST_KW
		{
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_CONST;
			curmod->cursym->ndeclarator++;
			$$ = $1;
		}
	|	RESTRICT_KW
		{
			curmod->cursym->declarator[curmod->cursym->ndeclarator] = DECL_RESTRICT;
			curmod->cursym->ndeclarator++;
			$$ = $1;
		}
	;

type_decl: typedecl_init type_attributes type_modifiers type
	;

typedecl_init:
		{
			idl_module_typedecl_push(curmod);
		}

symdef_init:
		{
			idl_intf_symdef_create(curmod->curintf, curmod->curtype);
		}
	;

type_attributes:
		attribute_opener type_attr_list extraneous_comma attribute_closer
	|	/* Nothing */
	;
	
type_attr_list:
        type_attr
    |   type_attr_list COMMA type_attr
    |   /* nothing */
    ;

type_attr:
		STRING_KW
	|	WIRE_MARSHAL_KW LPAREN IDENTIFIER RPAREN
	|	SIZE_IS_KW LPAREN IDENTIFIER RPAREN
	|	PTR_KW
	|	UNIQUE_KW
	|	REF_KW
	|	IID_IS_KW LPAREN IDENTIFIER RPAREN
	|	IN_KW
	|	OUT_KW
	;

type_modifiers:
		type_modifier
	|	type_modifiers type_modifier
	|	/* Nothing */
	;

type_modifier:
		CONST_KW
		{
			curmod->curtype->modifiers |= TYPEMOD_CONST;
		}
	|	UNSIGNED_KW
		{
			curmod->curtype->modifiers |= TYPEMOD_UNSIGNED;
		}
	|	SIGNED_KW
		{
			curmod->curtype->modifiers |= TYPEMOD_SIGNED;
		}
	;

type:
		CHAR_KW
		{
			curmod->curtype->builtin_type = TYPE_CHAR;
		}
	|	INT_KW
		{
			curmod->curtype->builtin_type = TYPE_INT;
		}
	|	VOID_KW
		{
			curmod->curtype->builtin_type = TYPE_VOID;
		}
	|	LONG_KW INT_KW
		{
			curmod->curtype->builtin_type = TYPE_LONG;
		}
	|	LONG_KW LONG_KW INT_KW
		{
			curmod->curtype->builtin_type = TYPE_LONGLONG;
		}
	|	SHORT_KW INT_KW
		{
			curmod->curtype->builtin_type = TYPE_SHORT;
		}
	|	SHORT_KW
		{
			curmod->curtype->builtin_type = TYPE_SHORT;
		}
	|	LONG_KW LONG_KW
		{
			curmod->curtype->builtin_type = TYPE_LONGLONG;
		}
	|	LONG_KW
		{
			curmod->curtype->builtin_type = TYPE_LONG;
		}
	|	FLOAT_KW
		{
			curmod->curtype->builtin_type = TYPE_FLOAT;
		}
	|	DOUBLE_KW
		{
			curmod->curtype->builtin_type = TYPE_DOUBLE;
		}
	|	struct LBRACE struct_body RBRACE
		{
			curmod->curtype->has_symlist = 1;
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
		}
	|	struct
		{
			idl_intf_symlist_pop(curmod->curintf, curmod->cursymlist);
		}
	|	UNION_KW IDENTIFIER
	|	ENUM_KW IDENTIFIER
	|	IDENTIFIER
		{
			curmod->curtype->builtin_type = TYPE_DEF;
			if(NULL == (curmod->curtype->user_type = idl_intf_symdef_lookup(curmod->curintf, $1)))
			{
				idl_module_error(curmod, yylineno, "Undeclared typedef '%s'", $1);
			}
		}
	|	error
		{
			idl_module_error(curmod, yylineno, "Expected: identifier, found '%s'", $1);
		}
	;
	
struct:
		STRUCT_KW
		{
			curmod->curtype->builtin_type = TYPE_STRUCT;
			curmod->curtype->symlist.symtype = SYM_MEMBER;
			idl_intf_symlist_push(curmod->curintf, &(curmod->curtype->symlist));
		}
	|	STRUCT_KW IDENTIFIER
		{
			strncpy(curmod->curtype->tag, $2, IDL_IDENT_MAX);
			curmod->curtype->tag[IDL_IDENT_MAX] = 0;
			curmod->curtype->builtin_type = TYPE_STRUCT;
			curmod->curtype->symlist.symtype = SYM_MEMBER;
			idl_intf_symlist_push(curmod->curintf, &(curmod->curtype->symlist));
		}
	;

possible_arg_list:
		arg_list
	|	VOID_KW
	|	/* nothing */
	;

arg_list:
		arg_list COMMA arg_decl
	|	arg_decl
	;
	

arg_decl:
		type_decl pointer_ident_decl
		{
			idl_module_typedecl_pop(curmod);
			curmod->curintf->firstsym = NULL;
		}
	;
	
struct_body:
		struct_body struct_union_member extraneous_semi
	|	struct_union_member extraneous_semi
	;

struct_union_member:
		type_decl pointer_ident_decl_list SEMI
		{
			idl_module_typedecl_pop(curmod);
			curmod->curintf->firstsym = NULL;
		}
	;

cpp_quote:
		CPP_QUOTE_KW LPAREN STRING RPAREN extraneous_semi
		{
			idl_emit_cppquote(curmod, $3);
		}
	;

uuid:
		UUID
		{
			const char *s;
			
			s = yyval;
			if('(' == *s)
			{
				s++;
			}
			while(*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
			{
				s++;
			}
			memmove(yylval, s, 36);
			yylval[36] = 0;
		}
	;
