#include <stdio.h>
#include "symbols.h"
#include "parse.h"

extern void errout(char *fmt, ...);
extern void let_stmt(symbol_t *sym, symbol_t tokens[]);

/************************************************************************
 ********************          STORE_TOKEN           ********************
 ************************************************************************/
void
store_token(symbol_t *tokens, int num_tokens, symbol_t *sym)
{
    if (num_tokens >= MAX_TOKENS-1)
	errout("too many symbols in line");
    tokens[num_tokens] = *sym;
}

/************************************************************************
 ********************           PARSE_LINE           ********************
 ************************************************************************/
void
parse_line()
{
    symbol_t	*sym, tokens[MAX_TOKENS];

    sym = getsym();
    if (sym->symtype != INTEGER)
    {
	fprintf(stderr, "Syntax error: expected line number");
	return;
    }
    sym = getsym();
    switch(sym->symtype)
    {
	case VARIABLE:
	case LET:
	    let_stmt(sym, tokens);
	    break;
	default:
	    printf("Statement type # %d not implemented yet\n", sym->symtype);
    }
}
