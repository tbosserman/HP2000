#include <stdio.h>
#include "symbols.h"
#include "parse.h"

extern void errout(char *fmt, ...);

void
parse_line()
{
    symbol_t	*sym;
    static char	*labels[] = {
	"Left Paren", "Right Paren", "Plus", "Minus", "Times", "Divide", "Equal"
    };

    sym = getsym();
    while (sym->symtype != EOL)
    {
	switch(sym->symtype)
	{
	    case INTEGER:
		printf("Integer = %d\n", sym->value.intval);
		break;
	    case FLOAT:
		printf("Float = %g\n", sym->value.floatval);
		break;
	    case STRING:
		printf("String = \"%s\"\n", sym->value.strval);
		break;
	    case LPAREN:
	    case RPAREN:
	    case PLUS:
	    case MINUS:
	    case TIMES:
	    case DIVIDE:
	    case EQUAL:
		printf("%s\n", labels[sym->symtype - LPAREN]);
		break;
	    case VARIABLE:
		printf("Variable = '%s'\n", sym->value.strval);
		break;
	    default:
		errout("Unrecognized symbol type");
	}
	sym = getsym();
    }
}
