#include <stdio.h>
#include "symbols.h"
#include "parse.h"

#define MAX_TOKENS	250

extern void errout(char *fmt, ...);
extern void let_stmt(symbol_t *sym, symbol_t tokens[], int max_tokens);

/************************************************************************
 ********************           PARSE_LINE           ********************
 ************************************************************************/
void
parse_line()
{
    symbol_t	*sym, tokens[MAX_TOKENS];

    sym = getsym();
    if (sym->symtype != INTEGER)
	errout("Syntax error: expected line number");
    sym = getsym();
    switch(sym->symtype)
    {
	case VARIABLE:
	case LET:
	    let_stmt(sym, tokens, MAX_TOKENS);
	    break;
	default:
	    printf("Statement type # %d not implemented yet\n", sym->symtype);
    }
}
