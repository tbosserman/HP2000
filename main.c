#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include "symbols.h"
#include "parse.h"

char		*progname;

/************************************************************************
 ********************             ERROUT             ********************
 ************************************************************************/
void
errout(char *fmt, ...)
{
    va_list	ap;
    char	temp[256];

    sprintf(temp, "%s: ", progname);
    va_start(ap, fmt);
    vsprintf(temp+strlen(temp), fmt, ap);
    va_end(ap);
    if (errno)
	perror(temp);
    else
	fprintf(stderr, "%s\n", temp);
    exit(3);
}

/************************************************************************
 ********************             USAGE              ********************
 ************************************************************************/
void
usage()
{
    fprintf(stderr, "usage: %s -f source_file\n", progname);
    exit(1);
}

/************************************************************************
 ********************              MAIN              ********************
 ************************************************************************/
int
main(int argc, char *argv[])
{
    int		i, ch;
    char	*fname;
    FILE	*fp;

    progname = argv[0];
    for (i = strlen(progname) - 1; i >= 0 && progname[i] != '/'; --i);
    progname += (i + 1);

    fname = NULL;
    while ((ch = getopt(argc, argv, "f:h?")) != -1)
    {
	switch(ch)
	{
	    case 'f':
		fname = optarg;
		break;
	    case '?':
	    case 'h':
		usage();
	}
    }
    if (fname == NULL)
	usage();
    if (strcmp(fname, "-") == 0)
	fp = stdin;
    else
	if ((fp = fopen(fname, "r")) == NULL)
	    errout("error opening %s for reading", fname);

    while (get_line(fp) == 0)
    {
	parse_line();
	putchar('\n');
    }

    exit(0);
}
