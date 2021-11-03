#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdarg.h>

char		*progname;

extern int hash(char *s);

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
 ********************              MAIN              ********************
 ************************************************************************/
int
main(int argc, char *argv[])
{
    int		i, bucket[51];
    char	line[128];

    progname = argv[0];
    for (i = strlen(progname) - 1; i >= 0 && progname[i] != '/'; --i);
    progname += (i + 1);

    for (i = 0; i < 51; ++i)
	bucket[i] = 0;

    while (fgets(line, sizeof(line), stdin) != NULL)
    {
	line[strlen(line)-1] = '\0';
	i = hash(line);
	/* printf("%2d - %s\n", i, line); */
	bucket[i]++;
    }
    if (ferror(stdin))
	errout("error reading stdin");

    for (i = 0; i < 51; ++i)
	printf("%2d - %6d\n", i, bucket[i]);

    exit(0);
}
