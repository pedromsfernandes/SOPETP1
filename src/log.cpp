#include <sys/types.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <math.h>
#include "log.h"
#include "macros.h"

using namespace std;

unsigned long long start_ms;
time_t start_s;

unsigned long long getTime()
{
    unsigned long long now_ms;
    time_t now_s;

    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    now_ms = round(spec.tv_nsec / 1.0e4);
    now_s = spec.tv_sec;

    if (now_ms > 99999)
    {
        now_s++;
        now_ms = 0;
    }

    return ((now_s - start_s) * 100000) + (now_ms - start_ms);
}

void startTime()
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    start_ms = round(spec.tv_nsec / 1.0e4);
    start_s = spec.tv_sec;

    if (start_ms > 99999)
    {
        start_s++;
        start_ms = 0;
    }
}

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
    unsigned long long tempo = getTime();

    proglog << fixed << setprecision(2);
    proglog << tempo / 100.0 << " - " << setw(8) << setfill('0');
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
    unsigned long long tempo = getTime();

    proglog << fixed << setprecision(2);
    proglog << tempo / 100.0 << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - OPENED " << filename << endl;
}

void logClose(string filename)
{
    unsigned long long tempo = getTime();

    proglog << fixed << setprecision(2);
    proglog << tempo / 100.0 << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - CLOSED " << filename << endl;
}

void logSignal(int dest, string signal)
{
    unsigned long long tempo = getTime();

    proglog << fixed << setprecision(2);
    proglog << tempo / 100.0 << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - SIGNAL " << signal << " to ";
    proglog << to_string(dest) << endl;
}

void logSignal(string signal)
{
    unsigned long long tempo = getTime();

    proglog << fixed << setprecision(2);
    proglog << tempo / 100.0 << " - " << setw(8) << setfill('0');
    proglog << to_string(getpid()) << " - SIGNAL " << signal << endl;
}