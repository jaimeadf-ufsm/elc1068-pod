#include <string.h>
#include <stdlib.h>

#include "common/string_utils.h"

char *strclone(const char *src)
{
    char *dest = (char *)malloc(strlen(src) + 1);

    if (dest == NULL)
    {
        return NULL;
    }

    strcpy(dest, src);
    
    return dest;
}