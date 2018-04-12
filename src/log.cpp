#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include "log.h"
#include "macros.h"

using namespace std;

string parseCommand(char *argv[])
{
    string command = "COMMAND simgrep ";

    for (int i = 1; argv[i] != NULL; i++)
    {
        if (i > 1)
            command += " ";
        command += argv[i];
    }

    return command;
}

void logCommand(char *argv[])
{
    ofstream log(getenv(LOGFILENAME), ios::app);
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    log << fixed << setprecision(2);
    log << inst << " - " << setw(8) << setfill('0');
    log << to_string(getpid()) << " - " << parseCommand(argv) << endl;

    log.close();
}

string getLogFileName()
{
    string input;
    cout << "Please insert the logfile name: ";
    getline(cin, input);
    int size = input.size();

    if (size <= 4 || input.substr(size-4, 4) != ".txt")
        input += ".txt";

    return input;
}

void logRead(string filename, const char* logfile)
{
    ofstream log(logfile, ios::app);
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    log << fixed << setprecision(2);
    log << inst << " - " << setw(8) << setfill('0');
    log << to_string(getpid()) << " - OPENNED " << filename << endl;

    log.close();
}

void logClose(string filename, const char* logfile)
{
    ofstream log(logfile, ios::app);
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    log << fixed << setprecision(2);
    log << inst << " - " << setw(8) << setfill('0');
    log << to_string(getpid()) << " - CLOSED " << filename << endl;

    log.close();
}

void logUSR1()
{
    ofstream log(getenv(LOGFILENAME), ios::app);
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    log << fixed << setprecision(2);
    log << inst << " - " << setw(8) << setfill('0');
    log << to_string(getpid()) << "SIGNAL USR1" << endl;

    log.close();
}

void logUSR2(int dest)
{
    ofstream log(getenv(LOGFILENAME), ios::app);
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    log << fixed << setprecision(2);
    log << inst << " - " << setw(8) << setfill('0');
    log << to_string(getpid()) << "SIGNAL USR2 to ";
    log << setw(8) << setfill('0') << to_string(dest) << endl;

    log.close();
}