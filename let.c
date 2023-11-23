#include <stdio.h>
#include "symbols.h"
#include "parse.h"

#define MAX_DEPTH	20

extern void errout(char *fmt, ...);

/************************************************************************
 ********************           EXPRESSION           ********************
 ************************************************************************/
void
expression(symbol_t tokens[], int num_tokens, int max_tokens)
{
    symbol_t	*sym, operands[MAX_DEPTH], operations[MAX_DEPTH];
    int		num_operations, num_operands, paren_count;

    num_operations = num_operands = paren_count = 0;
    for (sym = getsym(); sym->symtype != EOL; sym = getsym())
    {
	fprintf(stderr, "EXPRESSION: sym=%d\n", sym->symtype);
	switch(sym->symtype)
	{
	    case INTEGER:
	    case FLOAT:
	    case VARIABLE:
	    case STRING:
		if (num_operands > 0 && num_operations == 0)
		    errout("Syntax error in expression: expected operator");
		if (num_operands >= MAX_DEPTH-1)
		    errout("Operand stack overflow in expression");
		if (num_operations == 0)
		    operands[num_operands++] = *sym;
		else
		{
		    if (num_tokens >= max_tokens - 3)
			errout("Syntax error: max tokens exceeded (%d)",
			    max_tokens);
		    tokens[num_tokens++] = operands[num_operands--];
		    tokens[num_tokens++] = *sym;
		    tokens[num_tokens++] = operations[num_operations--];
		}
		break;

	    case PLUS:
	    case MINUS:
	    case TIMES:
	    case DIVIDE:
		if (num_operations >= MAX_DEPTH-1)
		    errout("Operator stack overflow in expression");
		if (num_operands == 0)
		    errout("Syntax error: expected operand in expression");
		operations[num_operations++] = *sym;
		break;

	    case LPAREN:
		paren_count++;
		break;

	    case RPAREN:
		if (paren_count == 0)
		    errout("Syntax error: unmatched right parenthesis");
		paren_count--;
		break;

	    default:
		errout("syntax error: unexpected symbol in expression");
	}

	if (num_operands >= MAX_DEPTH-1)
	    errout("Stack overflow in expression");
    }
}

/************************************************************************
 ********************            LET_STMT            ********************
 ************************************************************************/
void
let_stmt(symbol_t *sym, symbol_t tokens[], int max_tokens)
{
    int		num_tokens;

    if (sym->symtype == LET)
	sym = getsym();
    if (sym->symtype != VARIABLE)
	errout("Syntax error: expected a variable name\n");

    num_tokens = 0;
    tokens[num_tokens++] = *sym;
    sym = getsym();
    if (sym->symtype != EQUAL)
	errout("Syntax error: expected equals sign");

    expression(tokens, num_tokens, max_tokens);
}
