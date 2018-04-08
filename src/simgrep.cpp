#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"
#include <dirent.h>

using namespace std;

int invalidArgs()
{
    cout << "simgrep: Invalid arguments!" << endl;
    cout << "simgrep: Usage: simgrep [options] pattern [file/dir]" << endl;
    cout << "simgrep: Valid options: -i -l -n -c -w -r" << endl;

    return 1;
}

int validOption(char option)
{
    return option == 'i' || option == 'l' || option == 'n' ||
           option == 'c' || option == 'w' || option == 'r';
}

//Returns -1 is not found, 0 is file, 1 if direcorty
int isFileorDir(const char path[])
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

void fileNotFound(string filedir)
{
    cout << "simgrep: File " << filedir << " not found!" << endl;
}

int main(int argc, char *argv[])
{
    if (argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

    bool recursive = false;
    bool hasdir = false;
    bool hasfile = false;

    string options;
    string pattern;
    string filedir;

    int i = 1;
    int j = 0;

    for (; i < argc; i++)
    {
        if (argv[i][0] == '-')
            options.append(1, argv[i][1]);

        else
        {
            pattern = string(argv[i]);
            i++;
            break;
        }

        if (options[j] == 'r')
            recursive = true;
        else if (!validOption(options[j]))
        {
            cout << options[j] << " is not a valid option" << endl;
            return 2;
        }
    }

    if (i < argc - 1)
        return invalidArgs();

    if (i == argc - 1)
    {
        filedir = string(argv[i]);
        bool fileordir = isFileorDir(filedir.c_str());
        if (fileordir)
            hasdir = true;

        else if (!fileordir)
            hasfile = true;

        else
        {
            fileNotFound(filedir);
            return 2;
        }
    }

    if (hasfile)
    {
        return findPatternInFile(pattern, filedir, options, false);
    }
    else if (hasdir)
    {

        vector<string> filesInDir;
        DIR *d;
        struct dirent *dir;
        d = opendir(filedir.c_str());

        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (string(dir->d_name) != "." && string(dir->d_name) != "..")
                    filesInDir.push_back(filedir + dir->d_name);
            }

            closedir(d);
        }

        for (unsigned int j = 0; j < filesInDir.size(); j++)
        {
            findPatternInFile(pattern, filesInDir[j], options, true);
        }
    }
    else
        return findPatternInFile(pattern, "", options, false);

    return 0;
}