#include <string>

void logRead(std::string filename, const char* logfile);

void logClose(std::string filename, const char* logfile);

std::string getLogFileName();

void logCommand(char *argv[]);

std::string parseCommand(char *argv[]);

void logUSR1();

void logUSR2(int dest);
