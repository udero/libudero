
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctime>

#include "udero/UderoLogger.h"

int g_loglevel = UDERO_VERBOSE;
FILE* g_logfile = NULL;

bool ISLOG(UDERO_LOGLEVEL level) {
	return g_loglevel >= level;
}

void UDERO_LOG_PRINT(UDERO_LOGLEVEL level, const char* format, ...) {
  if (g_logfile && (level <= g_loglevel || level==UDERO_ALL)) {
    time_t now = std::time(NULL);
    struct tm* localNow = std::localtime(&now);
    fprintf(g_logfile, "UDERO[log=%s],%02d/%02d,%02d:%02d.%02d, ", LOGLEVEL_TO_STRING(level),
	    localNow->tm_mon + 1, localNow->tm_mday, localNow->tm_hour, localNow->tm_min, localNow->tm_sec);
    va_list ap;
    va_start(ap, format);
    vfprintf(g_logfile, format, ap);
    va_end(ap);
    fputc('\n', g_logfile);
    fflush(g_logfile);
  }
}


void INIT_LOG(const char* filename, UDERO_LOGLEVEL level) {
  g_logfile = fopen(filename, "w");
  g_loglevel = level > UDERO_VERBOSE ? UDERO_VERBOSE : level;
  UINFO("INIT_LOG()");
  UINFO(" - libudero version: %s", UDERO_VERSION_STRING);
  UINFO(" - loglevel : %s", LOGLEVEL_TO_STRING(level));
}


void FINI_LOG() {
  if (g_logfile) {
    fclose(g_logfile);
  }
}
