#include <stdio.h>
#include <string.h>
#include "macros.h"

int recursive = 0;

int invalidArgs()
{
    printf("Invalid arguments!\n");
    printf("Usage: simgrep [options] pattern [file/dir]\n");
    printf("Valid options: -i -l -n -c -w -r\n");

    return 1;
}

int validOption(char option)
{
    return option == 'i' || option == 'l' || option == 'n'
                                ||
           option == 'c' || option == 'w' || option == 'r';
}

int main(int argc, char* argv[])
{
    if(argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

    char options[MAX_OPTIONS][3];
    char pattern[MAX_PATTERN_SIZE];
    char filedir[MAX_FILEDIRNAME_SIZE];

    int i = 1;
    int j = 0;

    for(; i < argc; i++)
    {
        if(argv[i][0] == '-')
            strcpy(options[j++],argv[i]);

        else
        {
            strcpy(pattern,argv[i]);
            i++;
            break;
        }

        if(options[j-1][1] == 'r')
            recursive = 1;

        else if(!validOption(options[j-1][1]))
        {
            printf("%s is not a valid option\n",options[j - 1]);
            return 2;
        }
    }

    if(i < argc - 1)
        return invalidArgs();

    if(i == argc - 1)
        strcpy(filedir,argv[i]);
}
