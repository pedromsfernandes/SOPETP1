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
#include <dirent.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

int processGroup;

void sigint_handler(int sig)
{
    kill(-processGroup, SIGUSR1);
    string answer = "";

    while (!(answer == "Y" || answer == "y" || answer == "N" || answer == "n"))
    {
        cout << "\nAre you sure you want to terminate the program ? (Y / N) ";

        getline(cin, answer);
        if (answer == "Y" || answer == "y")
            kill(-processGroup, SIGTERM);
        else if (answer == "N" || answer == "n")
            kill(-processGroup, SIGUSR2);
    }
}

void sigusr1_handler(int sig)
{
    pause();
}

void nothing(int sig)
{
}

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

void sweepDir(string pattern, string dirName, string options, bool isRec, const char* logfile)
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
                        sweepDir(pattern, dirName + dir->d_name + '/', options, isRec, logfile);
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
        findPatternInFile(pattern, filesInDir[j], options, true, logfile);
    }

    int status;

    while (wait(&status) > 0)
    {
    }
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
    int j = -1;

    struct sigaction action;
    action.sa_handler = sigint_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGINT handler\n";
        exit(1);
    }

    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGUSR1, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGUSR1 handler\n";
        exit(1);
    }

    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGUSR2, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGUSR2 handler\n";
        exit(1);
    }

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
    
    string log = getLogFileName();
    setenv(LOGFILENAME, log.c_str(), 1);

    logCommand(argv);

    if (hasfile)
    {
        return findPatternInFile(pattern, filedir, options, false, log.c_str());
    }
    else if (hasdir)
    {
        processGroup = getpgrp();
        int pid = fork();
        int status = 0;

        if (pid == 0)
        {
            action.sa_handler = sigusr1_handler;
            sigemptyset(&action.sa_mask);
            action.sa_flags = 0;
            if (sigaction(SIGUSR1, &action, NULL) < 0)
            {
                cerr << "Couldn't install SIGUSR1 handler\n";
                exit(1);
            }

            sweepDir(pattern, filedir, options, recursive, log.c_str());
            exit(0);
        }
        else
        {
            wait(&status);
            exit(status);
        }
    }
    else
        return findPatternInFile(pattern, "stdin", options, false, log.c_str());

    return 0;
}