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
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    proglog << fixed << setprecision(2);
    proglog << inst << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - " << parseCommand(argv) << endl;
}

string getLogFileName()
{
    string input;
    cout << "Please insert the logfile name: ";
    getline(cin, input);
    int size = input.size();

    if (size <= 4 || input.substr(size - 4, 4) != ".txt")
        input += ".txt";

    return input;
}

void logRead(string filename)
{
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    proglog << fixed << setprecision(2);
    proglog << inst << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - OPENNED " << filename << endl;
}

void logClose(string filename)
{
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    proglog << fixed << setprecision(2);
    proglog << inst << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - CLOSED " << filename << endl;
}

void logSignal(int dest, string signal)
{
    clock_t inst = ((double)clock() / CLOCKS_PER_SEC) * 1000;

    proglog << fixed << setprecision(2);
    proglog << inst << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - SIGNAL " << signal << " to ";
    proglog << to_string(dest) << endl;
}