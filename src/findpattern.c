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
    int lineLength = 0;
    char **lines = malloc(MAX_FILE_SIZE * sizeof(char *));
    int i = 0;

    file = filename != NULL ? fopen(filename, "r") : stdin;

    while (fgets(line, MAX_LINE_SIZE - 1, file) != NULL)
    {
        lineLength = strlen(line) + 1;
        lines[i] = (char *)malloc(lineLength);
        strncpy(lines[i++], line, lineLength);
    }

    lines = realloc(lines, (i + 1) * sizeof(char *));
    lines[i + 1] = NULL;
    fclose(file);
    return lines;
}

void printArray(char **arr)
{
    for (int i = 0; arr[i] != NULL; i++)
        printf("%s", arr[i]);
}

char *toLowerCase(const char *string)
{
    int length = strlen(string);
    char *lowered = (char *)malloc(length + 1);

    for (int i = 0; i < length; i++)
        lowered[i] = tolower(string[i]);

    lowered[length] = '\0';
    return lowered;
}

char **decompString(const char *string, int *size)
{
    char **decomp = (char **)malloc(MAX_WORDS_SIZE * sizeof(char *));
    const char delim[] = " ,.!-?\n";
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

    while (lines[lineNumber - 1] != NULL)
    {
        strcpy(line, lines[lineNumber - 1]);
        if (strstr(line, pattern) != NULL)
        {
            sprintf(str, "%d:", lineNumber);
            strcat(str, line);
            foundLines[i] = (char *)malloc(strlen(str) + 1);
            strcpy(foundLines[i++], str);
        }

        lineNumber++;
    }

    foundLines = realloc(foundLines, (i + 1) * sizeof(char *));
    foundLines[i + 1] = NULL;
    return foundLines;
}

char **findPattern(const char *pattern, char **lines, int *hasPattern)
{
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    int i = 0;
    int lineLength = 0;
    char line[MAX_LINE_SIZE];

    for (int j = 0; strcpy(line, lines[j]) != NULL; j++)
    {
        if (strstr(line, pattern) != NULL)
        {
            if (hasPattern)
            {
                *hasPattern = 1;
                free(foundLines);
                return NULL;
            }

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

    for (int j = 0; lines[j] != NULL; j++)
    {
        strncpy(line, lines[j], strlen(lines[j]));
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

    if (hasPattern != NULL)
    {
        free(foundLines);
        return NULL;
    }

    free(lowered);
    foundLines = realloc(foundLines, (i + 1) * sizeof(char *));
    foundLines[i + 1] = NULL;
    return foundLines;
}

int findPatternCount(const char *pattern, char **lines)
{
    char line[MAX_LINE_SIZE];
    int counter = 0;

    for (int j = 0; lines[j] != NULL; j++)
    {
        strncpy(line, lines[j], strlen(lines[j]) + 1);
        if (strstr(line, pattern) != NULL)
            counter++;
    }

    return counter;
}

char **findPatternWord(const char *pattern, char **lines, int *hasPattern)
{
    char line[MAX_LINE_SIZE];
    char **foundLines = (char **)malloc(MAX_FILE_SIZE * sizeof(char *));
    char **splitLine;
    int i = 0;
    int splitLineSize = 0;
    int lineLength = 0;

    for (int c = 0; lines[c] != NULL; c++)
    {
        strncpy(line, lines[c], strlen(lines[c]) + 1);
        splitLine = decompString(line, &splitLineSize);

        for (int j = 0; j < splitLineSize; j++)
        {
            if (strcmp(splitLine[j], pattern) == 0)
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
                break;
            }
        }

        free(splitLine);
    }

    if (hasPattern != NULL)
    {
        free(foundLines);
        return NULL;
    }

    foundLines = realloc(foundLines, (i + 1) * sizeof(char *));
    foundLines[i] = NULL;
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
    int hasC = hasOption(options, 'c');
    int hasN = hasOption(options, 'n');
    char **lines = readFile(filename);
    if (hasL)
    {
        int hasI = hasOption(options, 'i');
        int hasW = hasOption(options, 'w');

        if (hasI && hasW)
        {
            int hasPatternIgnore = 0;
            int hasPatternWord = 0;
            findPatternIgnore(pattern, lines, &hasPatternIgnore);
            findPatternWord(pattern, lines, &hasPatternWord);

            if (hasPatternIgnore && hasPatternWord)
                printf("%s\n", filename);

            return 0;
        }

        else if (hasI)
        {
            int hasPatternIgnore = 0;
            findPatternIgnore(pattern, lines, &hasPatternIgnore);
            if (hasPatternIgnore)
                printf("%s\n", filename);

            return 0;
        }

        else if (hasW)
        {
            int hasPatternWord = 0;
            findPatternWord(pattern, lines, &hasPatternWord);
            if (hasPatternWord)
                printf("%s\n", filename);

            return 0;
        }
        else
        {
            int hasPattern = 0;
            findPattern(pattern, lines, &hasPattern);
            if (hasPattern)
                printf("%s\n", filename);
        }

        return 0;
    }
    else if (hasC)
    {
        int hasI = hasOption(options, 'i');
        int hasW = hasOption(options, 'w');

        if (hasI && hasW)
        {
            lines = findPatternIgnore(pattern, lines, NULL);
            lines = findPatternWord(pattern, lines, NULL);
            pattern = toLowerCase(pattern);
        }
        else if (hasI)
        {
            lines = findPatternIgnore(pattern, lines, NULL);
            pattern = toLowerCase(pattern);
        }
        else if (hasW)
        {
            lines = findPatternWord(pattern, lines, NULL);
        }

        printf("%d\n", findPatternCount(pattern, lines));
    }
    else if (hasN)
    {

        int hasI = hasOption(options, 'i');
        int hasW = hasOption(options, 'w');

        if (hasI)
            pattern = toLowerCase(pattern);

        lines = findPatternLines(pattern, lines);

        if (hasI && hasW)
        {
            lines = findPatternIgnore(pattern, lines, NULL);
            lines = findPatternWord(pattern, lines, NULL);
        }
        else if (hasI)
        {
            lines = findPatternIgnore(pattern, lines, NULL);
            pattern = toLowerCase(pattern);
        }
        else if (hasW)
        {
            lines = findPatternWord(pattern, lines, NULL);
        }

        printArray(lines);
    }

    free(lines);

    return 0;
}