#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"

char **readFile(const char *filename)
{
    FILE *file;
    char line[MAX_LINE_SIZE];
    char **lines = malloc(MAX_FILE_SIZE * sizeof(char *));
    int i = 0;

    file = filename != NULL ? fopen(filename, "r") : stdin;

    for (; fgets(line, MAX_LINE_SIZE - 1, file) != NULL; i++)
    {
    }

    lines = realloc(lines, i * sizeof(char *));
    fclose(file);
    return lines;
}

char *toLowerCase(const char *string)
{
    int length = strlen(string);
    char *lowered = (char *)malloc(length + 1);

    for (int i = 0; i < length; i++)
        lowered[i] = tolower(string[i]);

    return lowered;
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

char **findPatternLines(const char *pattern, char **lines)
{
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    int i = 0;
    int lineNumber = 1;
    char str[MAX_LINE_SIZE + 5];
    char line[MAX_LINE_SIZE];

    while (strcpy(line, lines[lineNumber - 1]) != NULL)
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

    foundLines = realloc(foundLines, i * sizeof(char *));
    return foundLines;
}

char **findPattern(const char *pattern, char **lines)
{
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    int i = 0;
    int lineLength = 0;
    char line[MAX_LINE_SIZE];

    for (int j = 0; strcpy(line, lines[j]) != NULL; j++)
    {
        if (strstr(line, pattern) != NULL)
        {
            lineLength = strlen(line) + 1;
            foundLines[i] = (char *)malloc(lineLength);
            strncpy(foundLines[i++], line, lineLength);
        }
    }

    foundLines = realloc(foundLines, i * sizeof(char *));
    return foundLines;
}

char **findPatternIgnore(const char *pattern, char **lines, int *hasPattern)
{
    char line[MAX_LINE_SIZE];
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    char *lowered = toLowerCase(pattern);
    int lineLength = 0;
    int i = 0;

    for (int j = 0; strcpy(line, lines[j]) != NULL; j++)
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
    foundLines = realloc(foundLines, i * sizeof(char *));

    return foundLines;
}

int findPatternCount(const char *pattern, char **lines)
{
    char line[MAX_LINE_SIZE];
    int counter = 0;

    for (int j = 0; lines[j] != NULL; j++)
        if (strstr(line, pattern) != NULL)
            counter++;

    return counter;
}

char **findPatternWord(const char *pattern, char **lines)
{
    char line[MAX_LINE_SIZE];
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    char **splitLine;
    int i = 0;
    int splitLineSize = 0;
    int lineLength = 0;

    for (int c = 0; strcpy(line, lines[c]) != NULL; c++)
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
            char **lines = readFile(filename);
            findPatternIgnore(pattern, lines, &hasPatternIgnore);
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