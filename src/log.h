#include <string>

void logRead(std::string filename);

void logClose(std::string filename);

std::string getLogFileName();

void logCommand(char *argv[]);

std::string parseCommand(char *argv[]);

void logUSR1();

void logUSR2(int dest);
