#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <ctime>
#include <stdarg.h>
#include <iomanip>

FILE *logfile;
//LOGLEVEL loglevel = LOGLEVEL_LOG;
LOGLEVEL loglevel = LOGLEVEL_VERBOSE;


void logger_init(ParamLoader* loader) {

  std::string loglevelstr = loader->get("LOG_LEVEL", "TRACE");
  if (loglevelstr == "WARNING") {
    loglevel = LOGLEVEL_WARN;
  }
  else if (loglevelstr == "ERROR") {
    loglevel = LOGLEVEL_ERROR;
  }
  else if (loglevelstr == "INFO") {
    loglevel = LOGLEVEL_INFO;
  }
  else if (loglevelstr == "LOG") {
    loglevel = LOGLEVEL_LOG;
  }
  else if (loglevelstr == "VERBOSE") {
    loglevel = LOGLEVEL_VERBOSE;
  }
  else if (loglevelstr == "TRACE") {
    loglevel = LOGLEVEL_TRACE;
  }
  std::string logdir = loader->get("LOG_LOGDIR", ".");
  #ifdef WIN32
  std::string sep = "\\";
#else
  std::string sep = "/";
  #endif
  std::string filename = loader->get("LOG_FILENAME", "udero_log.log");
  time_t now = std::time(NULL);
  struct tm* localNow = std::localtime(&now);
  if (filename.find("%d") != std::string::npos) {
    std::ostringstream ss;
    ss << 1900 + localNow->tm_year
       << std::setw(2) << std::setfill('0') << localNow->tm_mon + 1
       << std::setw(2) << std::setfill('0') << localNow->tm_mday
       << std::setw(2) << std::setfill('0') << localNow->tm_hour
       << std::setw(2) << std::setfill('0') << localNow->tm_min
       << std::setw(2) << std::setfill('0') << localNow->tm_sec;

    filename.replace(filename.find("%d"), 2, ss.str());
  }
  
  std::cout << "Logfile: " << logdir + sep + filename << std::endl;
  //  int c;
  //  std::cin >> c;
  logfile = fopen((logdir + sep + filename).c_str(), "w");
  if (logfile == NULL) {
    std::cout << "CANNOT OPEN LOGFILE. " << std::endl;
    std::cout << "PRESS ENTER TO EXIT." << std::endl;
    getchar();
    exit(-1);
  }
  

  LOGP(LOGLEVEL_ALL,  "-----------------------------------------------------");
  LOGP(LOGLEVEL_ALL,  "  Log: udero application");
  std::ostringstream ss;  
  ss << " Date: "
     << 1900 + localNow->tm_year << "/"
     << std::setw(2) << std::setfill('0') << localNow->tm_mon + 1 << "/"
     << std::setw(2) << std::setfill('0') << localNow->tm_mday << "/"
     << std::setw(2) << std::setfill('0') << localNow->tm_hour << ":"
     << std::setw(2) << std::setfill('0') << localNow->tm_min << "."
     << std::setw(2) << std::setfill('0') << localNow->tm_sec;
  logprint(LOGLEVEL_ALL, "%s", ss.str().c_str());
  LOGP(LOGLEVEL_ALL, "---------------");
}

void logprint(LOGLEVEL level, const char* format, ...) {
  if (level >= loglevel) {
    time_t now = std::time(NULL);
    struct tm* localNow = std::localtime(&now);
    fprintf(logfile, "%s,%02d/%02d,%02d:%02d.%02d, ", log2str(level), localNow->tm_mon + 1, localNow->tm_mday, localNow->tm_hour, localNow->tm_min, localNow->tm_sec);
    va_list ap;
    va_start(ap, format);
    vfprintf(logfile, format, ap);
    va_end(ap);
    fputc('\n', logfile);
  }
}

const char* log2str(LOGLEVEL level) {
  if (level == LOGLEVEL_ALL) { return        "ALL    "; }
  else if (level == LOGLEVEL_ERROR) { return "ERROR  "; }
  else if (level == LOGLEVEL_WARN) { return "WARN   "; }
  else if (level == LOGLEVEL_LOG) { return "LOG    "; }
  else if (level == LOGLEVEL_INFO) { return "INFO   "; }
  else if (level == LOGLEVEL_TRACE) { return "TRACE  "; }
  else if (level == LOGLEVEL_VERBOSE) { return "VERBOSE"; }
}
