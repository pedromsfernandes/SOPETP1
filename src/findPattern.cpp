#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"
#include "log.h"


using namespace std;

vector<string> readFile(string filename, const char* logfile)
{
    ifstream read(filename);

    logRead(filename,logfile);

    string line;
    vector<string> lines;

    while (!read.eof())
    {
        getline(read, line);

        if (!read.eof())
            lines.push_back(line);
    }

    read.close();

    return lines;
}

vector<string> readCin(const char* logfile)
{
    string line;
    vector<string> lines;

    logRead("stdin", logfile);

    while (!cin.eof())
    {
        getline(cin, line);

        if (!cin.eof())
            lines.push_back(line);
    }

    return lines;
}

void printLines(const vector<string> &lines)
{
    for (auto x : lines)
        cout << x << endl;
}

string toLowerCase(const string str)
{
    string lowered = str;
    transform(lowered.begin(), lowered.end(), lowered.begin(), ::tolower);
    return lowered;
}

vector<string> decompString(string str)
{
    vector<string> decomp;
    const string delim = " ,.!-?:\n";
    size_t pos1 = 0;
    size_t pos2 = 0;

    while ((pos2 = str.find_first_of(delim, pos1)) != string::npos)
    {
        decomp.push_back(str.substr(pos1, pos2 - pos1));
        pos1 = pos2 + 1;
    }

    return decomp;
}

vector<string> findPatternLines(string pattern, const vector<string> &lines)
{
    vector<string> foundLines;
    int size = lines.size();
    string line;

    for (int i = 0; i < size; i++)
    {
        if (lines.at(i).find(pattern) != string::npos)
        {
            line = to_string(i + 1) + ":" + lines.at(i);
            foundLines.push_back(line);
        }
    }

    return foundLines;
}

vector<string> findPattern(string pattern, const vector<string> &lines, bool *hasPattern)
{
    vector<string> foundLines;

    for (auto &line : lines)
    {
        if (line.find(pattern) != string::npos)
        {
            if (hasPattern)
            {
                *hasPattern = true;
                return foundLines;
            }

            foundLines.push_back(line);
        }
    }

    return foundLines;
}

vector<string> findPatternIgnore(string pattern, const vector<string> &lines, bool *hasPattern)
{
    vector<string> foundLines;

    string lowered = toLowerCase(pattern);

    for (auto &line : lines)
    {

        string loweredLine = toLowerCase(line);
        if (loweredLine.find(lowered) != string::npos)
        {
            if (hasPattern != NULL)
            {
                *hasPattern = true;
                return foundLines;
            }

            foundLines.push_back(line);
        }
    }

    return foundLines;
}

int findPatternCount(string pattern, const vector<string> &lines)
{
    int counter = 0;

    for (auto &line : lines)
        if (line.find(pattern) != string::npos)
            counter++;

    return counter;
}

vector<string> findPatternWord(string pattern, const vector<string> &lines, bool *hasPattern)
{
    vector<string> foundLines;
    vector<string> splitLine;

    for (auto &line : lines)
    {
        splitLine = decompString(line);

        for (auto &str : splitLine)
        {
            if (str == pattern)
            {
                if (hasPattern != NULL)
                {
                    *hasPattern = true;
                    return foundLines;
                }

                foundLines.push_back(line);
                break;
            }
        }
    }

    return foundLines;
}

bool hasOption(string options, const char option)
{
    return options.find(option) != string::npos;
}

vector<string> prependFileName(const vector<string> &lines, string fileName)
{
    vector<string> prepend;
    int size = lines.size();

    for (int i = 0; i < size; i++)
        prepend.push_back(fileName + ":" + lines[i]);

    return prepend;
}

int findPatternInFile(string pattern, string filename, string options, bool dir, const char* logfile)
{
    bool hasL = hasOption(options, 'l');
    bool hasC = hasOption(options, 'c');
    bool hasN = hasOption(options, 'n');
    bool hasI = hasOption(options, 'i');
    bool hasW = hasOption(options, 'w');

    vector<string> lines = filename == "stdin" ? readCin(logfile) : readFile(filename, logfile);

    if (hasL)
    {
        if (hasI && hasW)
        {
            bool hasPatternIgnore = false;
            bool hasPatternWord = false;
            findPatternIgnore(pattern, lines, &hasPatternIgnore);
            findPatternWord(pattern, lines, &hasPatternWord);

            if (hasPatternIgnore && hasPatternWord)
                cout << filename << endl;
        }
        else if (hasI)
        {
            bool hasPatternIgnore = false;
            findPatternIgnore(pattern, lines, &hasPatternIgnore);
            if (hasPatternIgnore)
                cout << filename << endl;
        }

        else if (hasW)
        {
            bool hasPatternWord = false;
            findPatternWord(pattern, lines, &hasPatternWord);
            if (hasPatternWord)
                cout << filename << endl;
        }
        else
        {
            bool hasPattern = false;
            findPattern(pattern, lines, &hasPattern);
            if (hasPattern)
                cout << filename << endl;
        }
    }
    else if (hasC)
    {
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

        if (dir)
            cout << filename << ":";
        cout << findPatternCount(pattern, lines) << endl;
    }
    else if (hasN)
    {
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
        }
        else if (hasW)
        {
            lines = findPatternWord(pattern, lines, NULL);
        }
        if (dir)
            lines = prependFileName(lines, filename);
        printLines(lines);
    }
    else if (hasI && hasW)
    {
        pattern = toLowerCase(pattern);
        lines = findPatternIgnore(pattern, lines, NULL);
        lines = findPatternWord(pattern, lines, NULL);
        if (dir)
            lines = prependFileName(lines, filename);
        printLines(lines);
    }
    else if (hasI)
    {
        lines = findPatternIgnore(pattern, lines, NULL);
        if (dir)
            lines = prependFileName(lines, filename);
        printLines(lines);
    }
    else if (hasW)
    {
        lines = findPatternWord(pattern, lines, NULL);
        if (dir)
            lines = prependFileName(lines, filename);
        printLines(lines);
    }
    else
    {
        lines = findPattern(pattern, lines, NULL);
        if (dir)
            lines = prependFileName(lines, filename);
        printLines(lines);
    }

    logClose(filename, logfile);

    return 0;
}