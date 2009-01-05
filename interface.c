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

idl_interface_t *
idl_intf_create(idl_module_t *module)
{
	idl_interface_t *p;
	
	if(NULL == (p = (idl_interface_t *) calloc(1, sizeof(idl_interface_t))))
	{
		return NULL;
	}
	p->cursymlist = &(p->symlist);
	idl_module_addintf(module, p);
	return p;
}

idl_interface_t *
idl_intf_lookup(const char *name)
{
	return idl_module_lookupintf(name);
}

int
idl_intf_done(idl_interface_t *intf)
{
	return idl_module_doneintf(intf->module, intf);
}

int
idl_intf_started(idl_interface_t *intf)
{
	uint8_t nulldata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	int hasuuid, needuuid;
	idl_typedecl_t *typedecl;
	idl_symdef_t *sym;
	
	if(intf->type == BLOCK_INTERFACE)
	{
		if(intf->object)
		{
			needuuid = 1;
		}
		else if(0 == intf->local)
		{
			needuuid = -1;
		}
	}
	else if(intf->type == BLOCK_COCLASS)
	{
		needuuid = 1;
	}
	if(!intf->uuid.data1 && !intf->uuid.data2 && !intf->uuid.data3 &&
		0 == memcmp(nulldata, intf->uuid.data4, 8))
	{
		hasuuid = 0;
	}
	else
	{
		hasuuid = 1;
	}
	if(1 == needuuid)
	{
		if(0 == hasuuid)
		{
			idl_module_error(intf->module, yyget_lineno(intf->module->scanner), "The uuid() attribute is required for the definition of %s", intf->name);
		}
	}
	else if(0 == needuuid)
	{
		if(1 == hasuuid)
		{
			idl_module_warning(intf->module, yyget_lineno(intf->module->scanner), "The uuid() attribute should not be specified for the definition of %s", intf->name);
		}
	}
	if(0 != hasuuid)
	{
		idl_module_addguid(intf->module, &(intf->uuid));
	}
	typedecl = idl_module_typedecl_push(intf->module);
	typedecl->builtin_type = TYPE_INTERFACE;
	sym = idl_module_symdef_create(intf->module, &(intf->symlist), typedecl);
	sym->type = SYM_TYPEDEF;
	strcpy(sym->ident, intf->name);
	idl_module_symdef_add(intf->module, &(intf->symlist), sym);
	idl_module_typedecl_pop(intf->module);
	idl_emit_intf_prologue(intf->module, intf);
	return 0;
}

int
idl_intf_finished(idl_interface_t *intf)
{
	idl_emit_intf_epilogue(intf->module, intf);
	return 0;
}

int
idl_intf_uuid(idl_interface_t *intf, const char *uuid)
{
	unsigned int tmp1, tmp2, tmp3, tmp4[8];
	size_t c;
	
	sscanf(uuid, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		&(tmp1), &(tmp2), &(tmp3),
		&(tmp4[0]), &(tmp4[1]),
		&(tmp4[2]), &(tmp4[3]), &(tmp4[4]), 
		&(tmp4[5]), &(tmp4[6]), &(tmp4[7]));
	intf->uuid.data1 = tmp1;
	intf->uuid.data2 = tmp2;
	intf->uuid.data3 = tmp3;
	for(c = 0; c < 8; c++)
	{
		intf->uuid.data4[c] = tmp4[c];
	}
	return 0;
}

int
idl_intf_name(idl_interface_t *intf, const char *name)
{
	strncpy(intf->name, name, IDL_IDENT_MAX);
	intf->name[IDL_IDENT_MAX] = 0;
	switch(intf->type)
	{
		case BLOCK_INTERFACE:
			strcpy(intf->uuid.name, "IID_");
			break;
		case BLOCK_COCLASS:
			strcpy(intf->uuid.name, "CLSID_");
			break;
		default:
			strcpy(intf->uuid.name, "GUID_");
			break;
	}
	strcat(intf->uuid.name, intf->name);
	return 0;
}

idl_symdef_t *
idl_intf_symdef_create(idl_interface_t *intf, idl_typedecl_t *typedecl)
{
	return idl_module_symdef_create(intf->module, intf->cursymlist, typedecl);
}

int
idl_intf_symdef_done(idl_interface_t *intf, idl_symdef_t *symdef)
{
	return idl_module_symdef_done(intf->module, intf->cursymlist, symdef);
}

int
idl_intf_symdef_link(idl_interface_t *intf, idl_symdef_t *symdef)
{
	return idl_module_symdef_link(intf->module, intf->cursymlist, symdef);
}

idl_symdef_t *
idl_intf_symdef_lookup(idl_interface_t *intf, const char *name)
{
	return idl_module_symdef_lookup(intf->module, intf->cursymlist, name, 1);
}

int
idl_intf_symlist_push(idl_interface_t *intf, idl_symlist_t *symlist)
{
	idl_module_symlist_push(intf->module, symlist);
	intf->cursymlist = intf->module->cursymlist;
	return 0;
}

int
idl_intf_symlist_pop(idl_interface_t *intf, idl_symlist_t *symlist)
{
	idl_module_symlist_pop(intf->module, symlist);
	intf->cursymlist = intf->module->cursymlist;
	return 0;
}

int
idl_intf_method_exists(idl_interface_t *intf, const char *methodname)
{
	size_t c;
	
	for(c = 0; c < intf->symlist.ndefs; c++)
	{
		if(SYM_METHOD == intf->symlist.defs[c]->type)
		{
			if(0 == strcmp(intf->symlist.defs[c]->ident, methodname))
			{
				return 1;
			}
		}
	}
	return 0;
}

int
idl_intf_method_exists_recurse(idl_interface_t *intf, const char *methodname)
{
	if(1 == idl_intf_method_exists(intf, methodname))
	{
		return 1;
	}
	if(NULL == intf->ancestor)
	{
		return 0;
	}
	return idl_intf_method_exists_recurse(intf->ancestor, methodname);
}

int
idl_intf_method_inherited(idl_interface_t *intf, const char *methodname)
{
	if(NULL == intf->ancestor)
	{
		return 0;
	}
	if(1 == idl_intf_method_exists_recurse(intf->ancestor, methodname))
	{
		return 1;
	}
	return 0;
}

/* Write a local method declaration */
int
idl_intf_write_method(idl_interface_t *intf, idl_symdef_t *def)
{
	return idl_emit_local_method(intf->module, intf, def);
}

/* Write a typedef declaration */
int
idl_intf_write_typedef(idl_interface_t *intf, idl_symdef_t *symdef)
{
	return idl_emit_typedef(intf->module, intf, symdef);
}

/* Write a type definition (e.g., a struct) */
int
idl_intf_write_type(idl_interface_t *intf, idl_typedecl_t *decl)
{
	return idl_emit_type(intf->module, intf, decl);
}
