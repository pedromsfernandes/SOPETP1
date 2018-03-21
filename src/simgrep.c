#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macros.h"

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

char *toLowerCase(const char *string)
{
    int length = strlen(string);
    char *lowered = (char *)malloc(length + 1);

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

char **decompString(const char *string, int *size)
{
    char **decomp = (char **)malloc(MAX_WORDS_SIZE * sizeof(char *));
    const char delim[] = " ,.!?\n";
    char *token;
    char *temp = (char *)malloc(strlen(string) + 1);
    int i = 0;

    strncpy(temp, string, strlen(string) + 1);
    token = strtok(temp, delim);

    for (; token != NULL; i++)
    {
        decomp[i] = (char *)malloc((strlen(token) + 1));
        strncpy(decomp[i], token, strlen(token) + 1);
        token = strtok(NULL, delim);
    }

    decomp = realloc(decomp, i * sizeof(char *));
    *size = i;
    free(temp);

    return decomp;
}

char **findPatternLines(const char *pattern, const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    long filesize = getFileSize(file);
    char **foundLines = (char **)malloc(filesize * sizeof(char *));
    int i = 0;
    int lineNumber = 1;
    char str[MAX_LINE_SIZE + 5];

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
    {
        if (strstr(line, pattern) != NULL)
        {
            sprintf(str, "%d:", lineNumber);
            strcat(str, line);
            foundLines[i] = (char *)malloc(strlen(str) + 1);
            strcpy(foundLines[i++], str);
        }

        lineNumber++;
    }

    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));
    *size = i;

    return foundLines;
}

char **findPattern(const char *pattern, const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    long filesize = getFileSize(file);
    char **foundLines = (char **)malloc(filesize * sizeof(char *));
    int i = 0;
    int lineLength = 0;

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
    {
        if (strstr(line, pattern) != NULL)
        {
            lineLength = strlen(line) + 1;
            foundLines[i] = (char *)malloc(lineLength);
            strncpy(foundLines[i++], line, lineLength);
        }
    }

    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));
    *size = i;

    return foundLines;
}

char **findPatternIgnore(const char *pattern, const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    long filesize = getFileSize(file);
    char **foundLines = (char **)malloc(filesize * sizeof(char *));
    char *lowered = toLowerCase(pattern);
    int lineLength = 0;
    int i = 0;

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
    {
        char *loweredLine = toLowerCase(line);
        if (strstr(loweredLine, lowered) != NULL)
        {
            lineLength = strlen(line) + 1;
            foundLines[i] = (char *)malloc(lineLength);
            strncpy(foundLines[i++], line, lineLength);
        }

        free(loweredLine);
    }

    free(lowered);
    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));
    *size = i;

    return foundLines;
}

int findPatternCount(const char *pattern, const char *filename)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    int counter = 0;

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
        if (strstr(line, pattern) != NULL)
            counter++;

    fclose(file);

    return counter;
}

char **findPatternWord(const char *pattern, const char *filename, int *size)
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE];
    long filesize = getFileSize(file);
    char **foundLines = (char **)malloc(filesize * sizeof(char *));
    char **splitLine;
    int i = 0;
    int splitLineSize = 0;
    int lineLength = 0;

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
    {
        splitLine = decompString(line, &splitLineSize);

        for (int j = 0; j < splitLineSize; j++)
        {
            if (strcmp(splitLine[j], pattern) == 0)
            {
                lineLength = strlen(line) + 1;
                foundLines[i] = (char *)malloc(lineLength);
                strncpy(foundLines[i++], line, lineLength);
                break;
            }
        }

        free(splitLine);
    }

    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));
    *size = i;

    return foundLines;
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
        if(S_ISREG(path_stat.st_mode))
            hasfile = 1;

        else hasdir = 1;
    }

    if(hasdir && !recursive)
    {
        printf("simgrep: %s: Is a directory\n", filedir);
        return 2;
    }

    /*
    int size = 0;
    char **s = findPattern("chair", "/home/zephyrminas/Documentos/SOPETP1/files/pg174.txt", &size);
    int n = findPatternCount("chair", "/home/zephyrminas/Documentos/SOPETP1/files/pg174.txt");
    printf("%d\n", n);
    printArray(s, size);
    */

    return 0;
}
