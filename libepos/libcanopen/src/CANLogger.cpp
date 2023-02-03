
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctime>

#include "CANLogger.h"


int g_loglevel = CAN_VERBOSE;
FILE* g_logfile = NULL;

FILE* getLogFile() {
    return g_logfile;
}

void setLogFile(FILE* fp) {
    g_logfile = fp;
}

bool ISLOG(CAN_LOGLEVEL level) {
    return g_loglevel >= level;
}

void CAN_LOG_PRINT(CAN_LOGLEVEL level, const char* format, ...) {
    if (g_logfile && (level <= g_loglevel || level == CAN_ALL)) {
        time_t now = std::time(NULL);
        struct tm* localNow = std::localtime(&now);
        fprintf(g_logfile, "CAN[log=%s],%02d/%02d,%02d:%02d.%02d, ", LOGLEVEL_TO_STRING(level),
            localNow->tm_mon + 1, localNow->tm_mday, localNow->tm_hour, localNow->tm_min, localNow->tm_sec);
        va_list ap;
        va_start(ap, format);
        vfprintf(g_logfile, format, ap);
        va_end(ap);
        fputc('\n', g_logfile);
        fflush(g_logfile);
    }
}


void INIT_LOG(const char* filename, CAN_LOGLEVEL level) {
    g_logfile = fopen(filename, "w");
    g_loglevel = level > CAN_VERBOSE ? CAN_VERBOSE : level;
    CINFO("INIT_LOG()");
    CINFO(" - libcanopen version: %s", CAN_VERSION_STRING);
    CINFO(" - loglevel : %s", LOGLEVEL_TO_STRING(level));
}


void FINI_LOG() {
    if (g_logfile) {
        fclose(g_logfile);
    }
}
