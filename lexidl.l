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

# ifndef YYSTYPE
#  define YYSTYPE char *
# endif

%}

%option noyywrap
%option yylineno

delim           [ \t\n\f]
opt_ws          {delim}*
ws              {delim}
letter          [A-Za-z_$]
digit           [0-9]
hex_digit       [0-9A-Fa-f]
id              {letter}({letter}|{digit})*
l_suffix        (l|L)
u_suffix        (u|U)
f_suffix        (u|U)
integer         -?{digit}+(({u_suffix}{l_suffix})|({l_suffix}{u_suffix})|{l_suffix}|{u_suffix})?
c_hex_integer   (0(x|X){hex_digit}*)(({l_suffix}{u_suffix}?)|({u_suffix}{l_suffix}?))?
float           {digit}+\.{digit}+({f_suffix}|{l_suffix})?
octet           {hex_digit}{hex_digit}
octet2          {octet}{octet}
octet_dot       {octet}\.
octet2_dash     {octet2}\-
olduuid         \({opt_ws}{octet2}{octet2}{octet}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet_dot}{octet}{opt_ws}\)
uuid            \({opt_ws}{octet2}{octet2_dash}{octet2_dash}{octet2_dash}{octet2_dash}{octet2}{octet2}{octet2}{opt_ws}\)
other           .

%%
{ws}            { /* No action, and no return */ }
'\n'            { }
":"             {return(COLON);}
","             {return(COMMA);}
".."            {return(DOTDOT);}
"="             {return(EQUAL);}
\[              {return(LBRACKET);}
"("             {return(LPAREN);}
\]              {return(RBRACKET);}
")"             {return(RPAREN);}
";"             {return(SEMI);}
"*"             {return(STAR);}
"{"             {return(LBRACE);}
"??<"           {return(LBRACE);}
"}"             {return(RBRACE);}
"??>"           {return(RBRACE);}
"?"             {return(QUESTION);}
"|"             {return(BAR);}
"||"            {return(BARBAR);}
"<"             {return(LANGLE);}
"<<"            {return(LANGLEANGLE);}
">"             {return(RANGLE);}
">>"            {return(RANGLEANGLE);}
"&"             {return(AMP);}
"&&"            {return(AMPAMP);}
"<="            {return(LESSEQUAL);}
">="            {return(GREATEREQUAL);}
"=="            {return(EQUALEQUAL);}
"^"             {return(CARET);}
"+"             {return(PLUS);}
"-"             {return(MINUS);}
"!"             {return(NOT);}
"!="            {return(NOTEQUAL);}
"/"             {return(SLASH);}
"%"             {return(PERCENT);}
"~"             {return(TILDE);}

\"[^\"\n]*  	{
	if (yytext[yyleng-1] == '\\')
	{
		yymore(); /* Allow backslashed " within strings, look for next " */
	}
	input();
	yytext++;
	yylval = yytext;
	return STRING;
}

{uuid}			{
	yylval = yytext;
	return(UUID);
}

{id}			{
	/* Attributes */
	if(!strcmp(yytext, "uuid")) return UUID_KW;
	if(!strcmp(yytext, "version")) return VERSION_KW;
	if(!strcmp(yytext, "interface")) return INTERFACE_KW;
	if(!strcmp(yytext, "local")) return LOCAL_KW;
	if(!strcmp(yytext, "pointer_default")) return POINTER_DEFAULT_KW;
	if(!strcmp(yytext, "object")) return OBJECT_KW;
	if(!strcmp(yytext, "string")) return STRING_KW;
	if(!strcmp(yytext, "wire_marshal")) return WIRE_MARSHAL_KW;
	if(!strcmp(yytext, "transmit_as")) return TRANSMIT_AS_KW;
	if(!strcmp(yytext, "size_is")) return SIZE_IS_KW;
	
	/* Pointer types */
	if(!strcmp(yytext, "ref")) return REF_KW;
	if(!strcmp(yytext, "ptr")) return PTR_KW;
	if(!strcmp(yytext, "unique")) return UNIQUE_KW;
	
	/* Direction */
	if(!strcmp(yytext, "in")) return IN_KW;
	if(!strcmp(yytext, "out")) return OUT_KW;
	
	if(!strcmp(yytext, "cpp_quote")) return CPP_QUOTE_KW;
	if(!strcmp(yytext, "typedef")) return TYPEDEF_KW;
	
	if(!strcmp(yytext, "void")) return VOID_KW;
	if(!strcmp(yytext, "int")) return INT_KW;
	if(!strcmp(yytext, "char")) return CHAR_KW;
	if(!strcmp(yytext, "float")) return FLOAT_KW;
	if(!strcmp(yytext, "double")) return DOUBLE_KW;
	if(!strcmp(yytext, "long")) return LONG_KW;
	if(!strcmp(yytext, "short")) return SHORT_KW;
	
	if(!strcmp(yytext, "const")) return CONST_KW;
	if(!strcmp(yytext, "restrict")) return RESTRICT_KW;
	
	if(!strcmp(yytext, "unsigned")) return UNSIGNED_KW;
	if(!strcmp(yytext, "signed")) return SIGNED_KW;

	if(!strcmp(yytext, "struct")) return STRUCT_KW;
	if(!strcmp(yytext, "union")) return UNION_KW;
	yylval = yytext;
	
	return IDENTIFIER;
}

{c_hex_integer}	{
	yylval = yytext;
	return INTEGER_NUMERIC;
}

{float}			{
	yylval = yytext;
	return FLOAT_NUMERIC;
}

{other}			{
	fprintf(stderr, "Parse error at line %d near '%s'\n", yylineno, yytext);
	exit(EXIT_FAILURE);
}
