#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"

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

char **findPatternLines(const char *pattern, const char *filename)
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

    return foundLines;
}

char **findPattern(const char *pattern, const char *filename)
{
    FILE *file;
    char **foundLines;
    long filesize;
    if (filename != NULL)
    {
        file = fopen(filename, "r");
        filesize = getFileSize(file);
        foundLines = (char **)malloc(filesize * sizeof(char *));
    }
    else
    {
        file = stdin;
        filesize = 100;
        foundLines = (char **)malloc(filesize * sizeof(char *));
    }
    char line[MAX_LINE_SIZE];
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

    return foundLines;
}

char **findPatternIgnore(const char *pattern, const char *filename, int *hasPattern)
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
            if (hasPattern != NULL)
            {
                *hasPattern = 1;
                free(foundLines);
                return NULL;
            }

            lineLength = strlen(line) + 1;
            foundLines[i] = (char *)malloc(lineLength);
            strncpy(foundLines[i++], line, lineLength);
        }

        free(loweredLine);
    }

    free(lowered);
    fclose(file);
    foundLines = realloc(foundLines, i * sizeof(char *));

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

char **findPatternWord(const char *pattern, const char *filename)
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

    return foundLines;
}

int hasOption(const char *options, const char option)
{
    int i = 0;
    for (; i < MAX_OPTIONS; i++)
        if (options[i] == option)
            return 1;

    return 0;
}

int findPatternInFile(const char *pattern, const char *filename, const char *options)
{
    int hasL = hasOption(options, 'l');
    //int hasC = hasOption(options, 'c');

    if (hasL)
    {
        int hasI = hasOption(options, 'i');
        int hasW = hasOption(options, 'w');

        if (hasI && hasW)
        {
            //TODO
        }

        if (hasI)
        {
            int hasPatternIgnore = 0;
            findPatternIgnore(pattern, filename, &hasPatternIgnore);
            if (hasPatternIgnore)
                printf("%s\n", filename);

            return 0;
        }

        /*if (hasW)
        {
            if (hasPatternWord(pattern, filename))
                printf("%s\n", filename);

            return 0;
        }

        if (hasPattern(pattern, filename))
            printf("%s\n", filename);

        return 0;*/
    }

    return 0;
}