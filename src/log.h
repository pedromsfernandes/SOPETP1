#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <fstream>

extern std::ofstream proglog;

void startTime();

void logRead(std::string filename);

void logClose(std::string filename);

std::string getLogFileName();

void logCommand(char *argv[]);

std::string parseCommand(char *argv[]);

void logSignal(int dest, std::string signal);

void logSignal(std::string signal);

#endif /* _LOG_H_ */
