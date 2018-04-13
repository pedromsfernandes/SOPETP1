#include <signal.h>
#include <string>
#include <iostream>
#include <unistd.h>
#include "log.h"

using namespace std;

int processGroup;

void setProcessGroup()
{
    processGroup = getpgrp();
}

void sigint_handler(int sig)
{
    logSignal(-processGroup, "SIGTSTP");
    kill(-processGroup, SIGTSTP);
    string answer = "";

    while (!(answer == "Y" || answer == "y" || answer == "N" || answer == "n"))
    {
        cout << "\nAre you sure you want to terminate the program ? (Y / N) ";

        getline(cin, answer);
        if (answer == "Y" || answer == "y")
        {
            logSignal(-processGroup, "SIGTERM");
            kill(-processGroup, SIGTERM);
        }
        else if (answer == "N" || answer == "n")
        {
            logSignal(-processGroup, "SIGCONT");
            kill(-processGroup, SIGCONT);
        }
    }
}

void nothing(int signo)
{
}

void installParentHandlers()
{
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
    if (sigaction(SIGTSTP, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGTSTP handler\n";
        exit(1);
    }

    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGTERM, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGTERM handler\n";
        exit(1);
    }

    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGCONT, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGCONT handler\n";
        exit(1);
    }
}

void installChildrenHandlers()
{
    struct sigaction action;
    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGINT, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGINT handler\n";
        exit(1);
    }

    signal(SIGTSTP, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
    signal(SIGCONT, SIG_DFL);
}
