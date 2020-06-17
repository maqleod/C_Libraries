#include <stdio.h>
#include "logging.h"

#define LOGFILE "log_test.log"

int main(int argc, char* argv[])
{
    FILE *fp = NULL;
    int log_mode = LOG_INFO;

    fp = fopen(LOGFILE, "a+");

    logMessage(fp, "Prints an INFO message", LOG_INFO, log_mode);
    screenMessage(SND_STDOUT, "Prints an INFO message to stdout", LOG_INFO, log_mode);
    screenMessage(SND_STDERR, "Prints an INFO message to stderr", LOG_INFO, log_mode);

    log_mode = LOG_ERROR;

    logMessage(fp, "Prints an ERROR message", LOG_ERROR, log_mode);
    screenMessage(SND_STDOUT, "Prints an ERROR message to stdout", LOG_ERROR, log_mode);
    screenMessage(SND_STDERR, "Prints an ERROR message to stderr", LOG_ERROR, log_mode);

    log_mode = LOG_DEBUG;

    logMessage(fp, "Prints an DEBUG message", LOG_DEBUG, log_mode);
    screenMessage(SND_STDOUT, "Prints an DEBUG message to stdout", LOG_DEBUG, log_mode);
    screenMessage(SND_STDERR, "Prints an DEBUG message to stderr", LOG_DEBUG, log_mode);

    log_mode = LOG_TRACE;

    logMessage(fp, "Prints an TRACE message", LOG_TRACE, log_mode);
    screenMessage(SND_STDOUT, "Prints an TRACE message to stdout", LOG_TRACE, log_mode);
    screenMessage(SND_STDERR, "Prints an TRACE message to stderr", LOG_TRACE, log_mode);

    fclose(fp);
    return 0;
}