#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_comidl.h"

idl_expr_t *
idl_expr_create(void)
{
	return (idl_expr_t *) calloc(1, sizeof(idl_expr_t));
}

idl_expr_t *
idl_expr_create_intconst(long value)
{
	idl_expr_t *p;
	
	if(NULL == (p = idl_expr_create()))
	{
		return NULL;
	}
	p->type = EXPR_CONST;
	p->isconst = 1;
	p->constval = value;
	return p;
}

idl_expr_t *
idl_expr_create_sym(idl_interface_t *intf, const char *sym)
{
	idl_symdef_t *symdef;
	idl_expr_t *p;
	
	if(NULL == (symdef = idl_intf_symdef_lookup(intf, sym)))
	{
		idl_module_error(curmod, yyget_lineno(curmod->scanner), "identifier '%s' referenced in expression ");
	}
	if(NULL == (p = idl_expr_create()))
	{
		return NULL;
	}
	p->type = EXPR_SYM;
	p->symdef = symdef;
	if(symdef->constval && 1 == symdef->constval->isconst)
	{
		p->isconst = 1;
		p->constval = symdef->constval->constval;
	}
	return p;
}

idl_expr_t *
idl_expr_create_pair(const idl_expr_t *left, idl_exprtype_t op, const idl_expr_t *right)
{
	idl_expr_t *p;
	
	if(NULL == (p = idl_expr_create()))
	{
		return NULL;
	}
	p->type = op;
	p->left = left;
	p->right = right;
	if(1 == left->isconst && 1 == right->isconst)
	{
		p->isconst = 1;
		switch(op)
		{
			case EXPR_ADD:
				p->constval = left->constval + right->constval;
				break;
			case EXPR_SUB:
				p->constval = left->constval - right->constval;
				break;
			case EXPR_DIV:
				p->constval = left->constval / right->constval;
				break;
			case EXPR_MUL:
				p->constval = left->constval * right->constval;
				break;
			case EXPR_MOD:
				p->constval = left->constval % right->constval;
				break;
			case EXPR_BITAND:
				p->constval = left->constval & right->constval;
				break;
			case EXPR_BITOR:
				p->constval = left->constval | right->constval;
				break;
			case EXPR_XOR:
				p->constval = left->constval ^ right->constval;
				break;
			case EXPR_EQUALS:
				p->constval = (left->constval == right->constval);
				break;
			case EXPR_NOTEQUALS:
				p->constval = (left->constval != right->constval);
				break;
			case EXPR_LESSEQUALS:
				p->constval = (left->constval <= right->constval);
				break;
			case EXPR_GTEQUALS:
				p->constval = (left->constval >= right->constval);
				break;
			case EXPR_LESS:
				p->constval = (left->constval < right->constval);
				break;
			case EXPR_GT:
				p->constval = (left->constval > right->constval);
				break;
			case EXPR_LSHIFT:
				p->constval = (left->constval << right->constval);
				break;
			case EXPR_RSHIFT:
				p->constval = (left->constval >> right->constval);
				break;
			default:
				fprintf(stderr, "[idl_expr_create_pair] unhandled paired operator %d\n", (int) op);
				break;
		}
	}
	return p;
}

idl_expr_t *
idl_expr_create_bracket(const idl_expr_t *expr)
{
	idl_expr_t *p;
	
	if(NULL == (p = idl_expr_create()))
	{
		return NULL;
	}
	p->type = EXPR_BRACKET;
	p->left = expr;
	p->isconst = expr->isconst;
	p->constval = expr->constval;
	return p;
}

idl_expr_t *
idl_expr_create_prefix(idl_exprtype_t op, const idl_expr_t *expr)
{
	idl_expr_t *p;
	
	if(NULL == (p = idl_expr_create()))
	{
		return NULL;
	}
	p->type = op;
	p->right = expr;
	if(1 == expr->isconst)
	{
		p->isconst = expr->isconst;
		switch(op)
		{
			case EXPR_BITNOT:
				p->constval = ~expr->constval;
				break;
			case EXPR_NOT:
				p->constval = !expr->constval;
				break;
			default:
				fprintf(stderr, "[idl_expr_create_prefix] unhandled prefix operator %d\n", (int) op);
		}
	}
	return p;
}
