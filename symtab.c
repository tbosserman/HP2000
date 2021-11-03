#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbols.h"
#include "symtab.h"

extern void errout(char *fmt, ...);

static node_t	*symtab[NUM_BUCKETS];

/************************************************************************
 ********************              HASH              ********************
 ************************************************************************/
int
hash(char *s)
{
    unsigned int	h, ch, highorder;

    h = 0;
    while ((ch = (unsigned int)*s++) != '\0')
    {
	highorder = h & 0xf8000000;
	h = h << 5;
	h = h ^ (highorder >> 27);
	h = h ^ ch;
    }
    return (h % NUM_BUCKETS);
}

/************************************************************************
 ********************           GET_SYMBOL           ********************
 ************************************************************************/
symbol_t *
get_symbol(char *name)
{
    int		bucket;
    node_t	*node;

    bucket = hash(name);
    for (node = symtab[bucket]; node; node = node->next)
    {
	if (strcmp(name, node->name) == 0)
	    break;
    }
    return(node->value);
}

/************************************************************************
 ********************          STORE_SYMBOL          ********************
 ************************************************************************/
void
store_symbol(char *name, symbol_t *symbol)
{
    int		bucket;
    node_t	*node;

    bucket = hash(name);
    node = symtab[bucket];
    while (node && strcmp(name, node->name) != 0)
	node = node->next;

    /* If symbol already exists just overwrite */
    if (node)
    {
	*(node->value) = *symbol;
	return;
    }

    if ((node = (node_t *)malloc(sizeof(node))) == NULL)
	errout("out of memory");
    node->name = strdup(name);
    *(node->value) = *symbol;
    node->next = symtab[bucket];
    symtab[bucket] = node;
}
