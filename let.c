#include <stdio.h>
#include "symbols.h"
#include "parse.h"

#define MAX_DEPTH	20

extern void errout(char *fmt, ...);

/************************************************************************
 ********************            IN_SET              ********************
 ************************************************************************/
int
in_set(int symtype, int set[])
{
    int		i;

    for (i = 0; set[i] >= 0; ++i)
    {
	if (symtype == set[i])
	    return(TRUE);
    }
    return(FALSE);
}

/************************************************************************
 ********************           EXPRESSION           ********************
 ************************************************************************/
int
expression(symbol_t tokens[], int num_tokens)
{
    symbol_t	*sym;
    int		prev_symtype;
    // Symbols that can preceed an operand.
    static int	set1[] = { LPAREN, PLUS, MINUS, TIMES, DIVIDE, EQUAL, -1 };
    // Symbols that can preceed an operator
    static int	set2[] = { RPAREN, VARIABLE, INTEGER, FLOAT, -1 };
    // Symbols that can preceed a string
    static int set3[] = { EQUAL, -1 };

    prev_symtype = tokens[num_tokens-1].symtype;
    for (sym = getsym(); sym->symtype != EOL; sym = getsym())
    {
	switch(sym->symtype)
	{
	    case LPAREN:
	    case INTEGER:
	    case FLOAT:
	    case VARIABLE:
		if (!in_set(prev_symtype, set1))
		{
		    fprintf(stderr, "Syntax error\n");
		    return(-1);
		}
		store_token(tokens, num_tokens++, sym);
		break;

	    case RPAREN:
	    case PLUS:
	    case MINUS:
	    case TIMES:
	    case DIVIDE:
		if (!in_set(prev_symtype, set2))
		{
		    fprintf(stderr, "Syntax error\n");
		    return;
		}
		store_token(tokens, num_tokens++, sym);
		break;

	    case STRING:
		if (!in_set(prev_symtype, set3))
		{
		    fprintf(stderr, "Syntax error\n");
		    return(-1);
		}
		store_token(tokens, num_tokens++, sym);
		break;

	    default:
		fprintf(stderr, "Unexpected symbol: %d\n", sym->symtype);

	}
	prev_symtype = sym->symtype;
    }

    return(num_tokens);
}

/************************************************************************
 ********************            LET_STMT            ********************
 ************************************************************************/
int
let_stmt(symbol_t *sym, symbol_t tokens[])
{
    int		num_tokens;

    num_tokens = 0;
    store_token(tokens, num_tokens++, sym);
    if (sym->symtype == LET)
	sym = getsym();
    if (sym->symtype != VARIABLE)
	errout("Syntax error: expected a variable name\n");
    store_token(tokens, num_tokens++, sym);

    sym = getsym();
    if (sym->symtype != EQUAL)
	errout("Syntax error: expected equals sign");
    store_token(tokens, num_tokens++, sym);

    num_tokens = expression(tokens, num_tokens);

    return(num_tokens);
}
