#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctype.h>
#include "macros.h"
#include "findPattern.h"
#include "log.h"
#include "signals.h"
#include <dirent.h>
#include <sys/wait.h>

using namespace std;

ofstream proglog;

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

void sweepDir(string pattern, string dirName, string options, bool isRec)
{
    vector<string> filesInDir;
    DIR *d;
    struct dirent *dir;
    d = opendir(dirName.c_str());

    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            int file_dir;

            if (dir->d_name[0] == '.')
                continue;

            if ((file_dir = isFileorDir((dirName + dir->d_name).c_str())) == 1)
            {
                if (isRec)
                {
                    int pid = fork();
                    if (pid == 0)
                    {
                        sweepDir(pattern, dirName + dir->d_name + '/', options, isRec);
                        exit(0);
                    }
                }
            }

            if (file_dir == -1)
                continue;

            if (!file_dir && string(dir->d_name).at(0) != '.')
                filesInDir.push_back(dirName + dir->d_name);
        }

        closedir(d);
    }

    unsigned int size = filesInDir.size();
    for (unsigned int j = 0; j < size; j++)
    {
        findPatternInFile(pattern, filesInDir[j], options, true);
    }

    int status;

    while (wait(&status) > 0)
    {
    }
}

int main(int argc, char *argv[], char *envp[])
{
    startTime();
    string logfile = getLogFileName();
    setenv(LOGFILENAME, logfile.c_str(), 1);
    proglog.open(string(getenv(LOGFILENAME)), ios::app);
    logCommand(argv);

    if (argc < 2 || argv[argc - 1][0] == '-')
        return invalidArgs();

    bool recursive = false;
    bool hasdir = false;
    bool hasfile = false;

    string options;
    string pattern;
    string filedir;

    int i = 1;
    int j = -1;

    for (; i < argc; i++)
    {
        if (argv[i][0] == '-')
        {
            options.append(1, argv[i][1]);
            j++;
        }

        else
        {
            pattern = string(argv[i]);
            i++;
            break;
        }

        if (hasOption(options, 'r'))
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
        {
            hasdir = true;
            if (filedir.back() != '/')
                filedir.append(1, '/');
        }

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
        setProcessGroup();
        installParentHandlers();
        int pid = fork();
        int status = 0;

        if (pid == 0)
        {
            installChildrenHandlers();
            sweepDir(pattern, filedir, options, recursive);
            exit(0);
        }
        else
        {
            wait(&status);
            exit(status);
        }
    }
    else
        return findPatternInFile(pattern, "stdin", options, false);

    return 0;
}