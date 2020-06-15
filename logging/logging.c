/*
A simple logging module
Written by Jared Epstein
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "logging.h"

/* collects the current timestamp and returns it as string with newline removed */
const char* getTimestamp()
{
    time_t ltime;
    ltime = time(NULL);
    char *t = ctime(&ltime);
    t[strlen(t)-1] = '\0';
    return t;
}

/* prints a message to the given log with current timestamp and flushes the buffer */
void logMessage(FILE* fp, char* message, int log_level, int log_mode)
{
    if (log_level <= log_mode) {
        fprintf(fp, "%s %s\n", getTimestamp(), message);
        fflush(fp);
    }
}

/* prints a message to given std stream with current timestamp*/
void screenMessage(int target, char* message, int log_level, int log_mode)
{
    if (log_level <= log_mode) {
        if (target == SND_STDOUT) {
            printf("%s %s\n", getTimestamp(), message);
        } else if (target == SND_STDERR) {
            fprintf(stderr, "%s %s\n", getTimestamp(), message);
            fflush(stderr);
        }
    }
}