#include <string.h>
#include <sys/types.h>
#include "strings.h"

extern void errout(char *fmt, ...);

char		string_data[MAX_STRING_DATA];
int		total_string_length = 0;

// Search through the string data and see if an identical string
// has already been stored. If so, return a pointer to the existing
// string. Otherwise store the new string and return that pointer.
u_int16_t
store_string(char *string)
{
    int		result, len;

    result = 0;
    len = strlen(string) + 1; /* The +1 is for the null terminator */
    while (result < total_string_length)
    {
	if (strcmp(string, string_data + result) == 0)
	    return(result);
	result += len;
    }
    if (result + len >= MAX_STRING_DATA)
	errout("max string data exceeded");
    strcpy(string_data+result, string);
    total_string_length += len;
}
