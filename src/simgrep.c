#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"
#include <dirent.h>

int invalidArgs()
{
    printf("simgrep: Invalid arguments!\n");
    printf("simgrep: Usage: simgrep [options] pattern [file/dir]\n");
    printf("simgrep: Valid options: -i -l -n -c -w -r\n");

    return 1;
}

int validOption(char option)
{
    return option == 'i' || option == 'l' || option == 'n' ||
           option == 'c' || option == 'w' || option == 'r';
}

void printArray(char **arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        if (i > 0)
            printf("\n");
        printf("%s", arr[i]);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

    int recursive = 0;
    int hasdir = 0;
    int hasfile = 0;

    char options[MAX_OPTIONS][3];
    char pattern[MAX_PATTERN_SIZE];
    char filedir[MAX_FILEDIRNAME_SIZE];

    int i = 1;
    int j = 0;

    for (; i < argc; i++)
    {
        if (argv[i][0] == '-')
            strcpy(options[j++], argv[i]);

        else
        {
            strcpy(pattern, argv[i]);
            i++;
            break;
        }

        if (options[j - 1][1] == 'r')
            recursive = 1;

        else if (!validOption(options[j - 1][1]))
        {
            printf("%s is not a valid option\n", options[j - 1]);
            return 2;
        }
    }

    if (i < argc - 1)
        return invalidArgs();

    if (i == argc - 1)
    {
        strcpy(filedir, argv[i]);

        struct stat path_stat;
        stat(filedir, &path_stat);
        if (S_ISDIR(path_stat.st_mode))
            hasdir = 1;

        if (S_ISREG(path_stat.st_mode))
            hasfile = 1;
    }

    if (hasdir && !recursive)
    {
        printf("simgrep: %s: Is a directory\n", filedir);
        return 2;
    }

    int size = 0;
    char **s = findPatternLines(pattern, filedir, &size);
    // int n = findPatternCount("chair", "/home/zephyrminas/Documentos/SOPETP1/files/pg174.txt");
    //printf("%d\n", n);
    printArray(s, size);

    if (hasdir)
    {
        char filesInDir[MAX_FILES_IN_DIR][MAX_FILE_NAME];
        int i = 0;
        DIR *d;
        struct dirent *dir;
        d = opendir(filedir);

        if (d)
        {
            while ((dir = readdir(d) )!= NULL)
            {
                strncpy(filesInDir[i++], dir->d_name, MAX_FILE_NAME -1);
            }

            closedir(d);
        }
    }

    return 0;
}
