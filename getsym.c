#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include "symbols.h"

extern void errout(char *fmt, ...);
extern int alltrim(char *string);

static line_t	line;
static symbol_t	symbol;

int
get_character()
{
    line.curch = line.nextch;
    if (line.nextch != EOL)
	line.nextch = *line.curp++;
    return(line.curch);
}

int
cur_character()
{
    return(line.curch);
}

int
next_character()
{
    return(line.nextch);
}

int
get_line(FILE *fp)
{
    if (fgets(line.line, MAXLINE, fp) == NULL)
    {
	if (ferror(fp))
	    return(errno);
	else
	    return(END_OF_FILE);
    }
    line.line[strlen(line.line)-1] = '\0';
    (void)alltrim(line.line);
    puts(line.line);
    line.curp = line.line;
    line.nextch = -1;
    get_character();
    return(0);
}

void
word()
{
    int		len;
    char	*charp;

    len = 0;
    charp = symbol.value.strval;
    symbol.symtype = VARIABLE;

    while (isalnum(line.nextch))
    {
	if (len >= MAXSTRING - 2)
	    errout("string too long");
	*charp++ = line.curch;
	++len;
	get_character();
    }
    if (len >= MAXSTRING - 2)
	errout("string too long");
    *charp++ = line.curch;
    *charp = '\0';
}

void
number()
{
    int		numtype, intval;
    double	floatval, divisor;

    numtype = INTEGER;
    intval = 0;
    while (isdigit(line.curch) || line.curch == '.')
    {
	if (line.curch == '.')
	{
	    if (numtype == FLOAT)
		errout("malformed floating point number");
	    numtype = FLOAT;
	    floatval = (double)intval;
	    divisor = 10.0;
	    get_character();
	    continue;
	}

	if (numtype == INTEGER)
	    intval = (intval * 10) + (line.curch - '0');
	else
	{
	    floatval += (line.curch - '0') / divisor;
	    divisor *= 10.0;
	}

	if (!isdigit(line.nextch) && line.nextch != '.')
	    break;
	get_character();
    }

    symbol.symtype = numtype;
    if (numtype == INTEGER)
	symbol.value.intval = intval;
    else
	symbol.value.floatval = floatval;
}

void
string()
{
    int		len;
    char	*charp, stringval[MAXSTRING];

    len = 0;
    charp = stringval;
    symbol.symtype = STRING;

    get_character();
    while (line.curch != '"' && line.curch != EOL)
    {
	if (len >= MAXSTRING - 2)
	    errout("string too long");
	*charp++ = line.curch;
	++len;
	get_character();
    }
    if (line.curch != '"')
	errout("unterminated string");
    *charp = '\0';
}

int
lookup_symbol(int symch)
{
    int		i, ch;
    static int symnames[] =
	{ LPAREN, RPAREN, PLUS, MINUS, TIMES, DIVIDE, EQUAL, NONE };
    static int symchars[] = { '(', ')', '+', '-', '*', '/', '=', 0 };

    for (i = 0; (ch = symchars[i]) != 0; ++i)
    {
	if (symch == ch)
	    return(symnames[i]);
    }
    return(NONE);
}

symbol_t *
getsym()
{
    int		ch;

    ch = get_character();
    while (isspace(ch))
	ch = get_character();

    if (isdigit(ch))
    {
	number();
	return(&symbol);
    }

    if (isalpha(ch))
    {
	word();
	return(&symbol);
    }

    switch(ch)
    {
	case '"':
	    string();
	    break;

	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '=':
	    symbol.symtype = lookup_symbol(ch);
	    break;

	case END_OF_FILE:
	case EOL:
	    symbol.symtype = ch;
	    break;

	default:
	    errout("syntax error");
    }

    return(&symbol);
}
