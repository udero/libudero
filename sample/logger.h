#pragma once
#include <stdio.h>
//#include <time.h>
#include "param_loader.h"
#include "udero/UderoLogger.h"
enum LOGLEVEL {
  LOGLEVEL_VERBOSE,
  LOGLEVEL_TRACE,
  LOGLEVEL_INFO,
  LOGLEVEL_LOG,
  LOGLEVEL_WARN,
  LOGLEVEL_ERROR,
  LOGLEVEL_ALL,
};

extern FILE *logfile;
extern LOGLEVEL loglevel;


void logger_init(ParamLoader* loader);

const char* log2str(LOGLEVEL level);

void logprint(LOGLEVEL level, const char* format, ...);
#define LOGP logprint

void logger_init();
