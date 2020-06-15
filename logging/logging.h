/*
A simple logging module
Written by Jared Epstein
*/

#define LOG_INFO 0
#define LOG_ERROR 1
#define LOG_DEBUG 2
#define LOG_TRACE 3

#define SND_STDOUT 0
#define SND_STDERR 1

const char* getTimestamp();

void logMessage(FILE* fp, char* message, int log_level, int log_mode);

void screenMessage(int target, char* message, int log_level, int log_mode);