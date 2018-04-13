#include <signal.h>
#include <string>
#include <iostream>
#include <unistd.h>

using namespace std;

int processGroup;

void setProcessGroup()
{
    processGroup = getpgrp();
}

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

    action.sa_handler = nothing;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    if (sigaction(SIGTERM, &action, NULL) < 0)
    {
        cerr << "Couldn't install SIGTERM handler\n";
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

    action.sa_handler = sigusr1_handler;
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

    signal(SIGTERM, SIG_DFL);
}