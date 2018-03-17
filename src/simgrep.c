#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
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
    return option == 'i' || option == 'l' || option == 'n' ||
           option == 'c' || option == 'w' || option == 'r';
}

char *toLowerCase(const char *string)
{
    int length = strlen(string);
    char *lowered = (char *)malloc(length);

    for (int i = 0; i < length; i++)
        lowered[i] = tolower(string[i]);

    return lowered;
}

long getFileSize(FILE *file)
{
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    return file_size;
}

char **findPattern(const char *pattern, const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    long filesize = getFileSize(file);
    char **foundLines = (char **)malloc(filesize * sizeof(char *));
    int i = 0;

    while (1)
    {
        if (fgets(line, MAX_LINE_SIZE - 1, file) == NULL)
            break;

        if (strstr(line, pattern) != NULL)
        {
            foundLines[i] = (char *)malloc(strlen(line));
            strcpy(foundLines[i++], line);
        }
    }

    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));
    *size = i;

    return foundLines;
}

void printArray(char **arr, int size)
{
    for (int i = 0; i < size; i++)
        printf("%s", arr[i]);
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

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
        strcpy(filedir, argv[i]);

    // printf("%s\n", toLowerCase("olACARAlHo"));

    /*
    int size = 0;
    char **s = findPattern("chair", "/home/zephyrminas/Documentos/SOPETP1/files/pg174.txt", &size);
    printArray(s, size);
    */

    return 0;
}
