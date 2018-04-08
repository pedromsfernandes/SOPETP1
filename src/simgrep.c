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

//Returns -1 is not found, 0 is file, 1 if direcorty
int isFileorDir(char path[])
{
    struct stat path_stat;
    if (stat(path, &path_stat) < 0)
        return -1;

    if (S_ISDIR(path_stat.st_mode))
        return 1;

    if (S_ISREG(path_stat.st_mode))
        return 0;

    return -1;
}

void fileNotFound(char filedir[])
{
    printf("simgrep: File %s not found\n", filedir);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

    int recursive = 0;
    int hasdir = 0;
    int hasfile = 0;

    char options[MAX_OPTIONS];
    char pattern[MAX_PATTERN_SIZE];
    char filedir[MAX_FILEDIRNAME_SIZE];

    int i = 1;
    int j = 0;

    for (; i < argc; i++)
    {
        if (argv[i][0] == '-')
            options[j++] = argv[i][1];

        else
        {
            strcpy(pattern, argv[i]);
            i++;
            break;
        }

        if (options[j - 1] == 'r')
            recursive = 1;

        else if (!validOption(options[j - 1]))
        {
            printf("-%c is not a valid option\n", options[j - 1]);
            return 2;
        }
    }

    if (i < argc - 1)
        return invalidArgs();

    if (i == argc - 1)
    {
        strcpy(filedir, argv[i]);
        int fileordir = isFileorDir(filedir);
        if (fileordir == 1)
            hasdir = 1;

        else if (!fileordir)
            hasfile = 1;

        else
        {
            fileNotFound(filedir);
            return 2;
        }
    }

    if (hasfile)
        return findPatternInFile(pattern, filedir, options);

    //int size = 0;
    //char **s = findPatternLines(pattern, filedir, &size);
    // int n = findPatternCount("chair", "/home/zephyrminas/Documentos/SOPETP1/files/pg174.txt");
    //printf("%d\n", n);
    //printArray(s, size);

    else if (hasdir)
    {
        char **filesInDir = malloc(MAX_FILES_IN_DIR * sizeof(char *));
        int i = 0;
        DIR *d;
        struct dirent *dir;
        d = opendir(filedir);

        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                filesInDir[i] = malloc(strlen(dir->d_name) + 1);
                if (strcmp(dir->d_name, "..") != 0 && strcmp(dir->d_name, ".") != 0)
                    strncpy(filesInDir[i++], dir->d_name, strlen(dir->d_name) + 1);
            }

            closedir(d);
        }
        for (int j = 0; j < i; j++)
        {
            //printf("%s\n", filesInDir[j]);
            findPatternInFile(pattern, filesInDir[j], options);
        }

        free(filesInDir);
    }
    else
        return findPatternInFile(pattern, NULL, options);

    return 0;
}