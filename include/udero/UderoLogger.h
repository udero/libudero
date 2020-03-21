#pragma once
/**
 * Logging function
 * 
 * prints local time and level simultaneously.
 */

#include "version.h"
#include "defines.h"



enum UDERO_LOGLEVEL {
  UDERO_SILENT = -1,
  UDERO_ERROR,
  UDERO_WARN,
  UDERO_INFO,
  UDERO_NORMAL,
  UDERO_DEBUG,
  UDERO_TRACE,
  UDERO_VERBOSE,
  UDERO_ALL
};

inline const char* LOGLEVEL_TO_STRING(UDERO_LOGLEVEL level) {
  switch(level){
  case UDERO_SILENT: return "UDERO_SILENT";
  case UDERO_ERROR: return "UDERO_ERROR";
  case UDERO_WARN: return "UDERO_WARN";
  case UDERO_INFO: return "UDERO_INFO";
  case UDERO_NORMAL: return "UDERO_NORMAL";
  case UDERO_DEBUG: return "UDERO_DEBUG";
  case UDERO_TRACE: return "UDERO_TRACE";
  case UDERO_VERBOSE: return "UDERO_VERBOSE";
  case UDERO_ALL: return "UDERO_ALL";
  default: return "UDERO_UNKNOWN_DEBUG_LEVEL";
  }
}

LIBUDERO_API bool ISLOG(UDERO_LOGLEVEL level);
LIBUDERO_API void UDERO_LOG_PRINT(UDERO_LOGLEVEL level, const char* format, ...);

#define UALL(...) UDERO_LOG_PRINT(UDERO_ALL, __VA_ARGS__)
#define UERROR(...) UDERO_LOG_PRINT(UDERO_ERROR, __VA_ARGS__)
#define UWARN(...) UDERO_LOG_PRINT(UDERO_WARN, __VA_ARGS__)
#define UINFO(...) UDERO_LOG_PRINT(UDERO_INFO, __VA_ARGS__)
#define UNORMAL(...) UDERO_LOG_PRINT(UDERO_NORMAL, __VA_ARGS__)
#define UDEBUG(...) UDERO_LOG_PRINT(UDERO_DEBUG, __VA_ARGS__)
#define UTRACE(...) UDERO_LOG_PRINT(UDERO_TRACE, __VA_ARGS__)
#define UVERBOSE(...) UDERO_LOG_PRINT(UDERO_VERBOSE, __VA_ARGS__)



namespace reharo {
  LIBUDERO_API void initLogger(const int argc, const char* argv[]);
  LIBUDERO_API void finiLogger();
};
