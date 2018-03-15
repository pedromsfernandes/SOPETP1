#include <stdio.h>
#include <string.h>
#include "macros.h"

int main(int argc, char* argv[])
{
    if(argc < 2)
        return -1;

    char options[MAX_OPTIONS][3];
    char pattern[MAX_PATTERN_SIZE];

    int i = 1;
    int j = 0;

    for(; i < argc; i++)
    {
        if(argv[i][0] == '-')
            strcpy(options[j++],argv[i]);

        else
        {
            strcpy(pattern,argv[i]);
            break;
        }
    }

    printf("%s\n",pattern);

    return 0;
}
