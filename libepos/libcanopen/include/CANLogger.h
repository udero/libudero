#pragma once
/**
 * Logging function
 *
 * prints local time and level simultaneously.
 */

#include <stdio.h>
#include <stdlib.h>
#include "CANopen_defines.h"


enum CAN_LOGLEVEL {
    CAN_SILENT = -1,
    CAN_ERROR,
    CAN_WARN,
    CAN_INFO,
    CAN_NORMAL,
    CAN_DEBUG,
    CAN_TRACE,
    CAN_VERBOSE,
    CAN_ALL
};

inline const char* LOGLEVEL_TO_STRING(CAN_LOGLEVEL level) {
    switch (level) {
    case CAN_SILENT: return "CAN_SILENT";
    case CAN_ERROR: return "CAN_ERROR";
    case CAN_WARN: return "CAN_WARN";
    case CAN_INFO: return "CAN_INFO";
    case CAN_NORMAL: return "CAN_NORMAL";
    case CAN_DEBUG: return "CAN_DEBUG";
    case CAN_TRACE: return "CAN_TRACE";
    case CAN_VERBOSE: return "CAN_VERBOSE";
    case CAN_ALL: return "CAN_ALL";
    default: return "CAN_UNKNOWN_DEBUG_LEVEL";
    }
}

CANOPEN_API bool ISLOG(CAN_LOGLEVEL level);
CANOPEN_API void CAN_LOG_PRINT(CAN_LOGLEVEL level, const char* format, ...);

#define CALL(...) CAN_LOG_PRINT(CAN_ALL, __VA_ARGS__)
#define CERROR(...) CAN_LOG_PRINT(CAN_ERROR, __VA_ARGS__)
#define CWARN(...) CAN_LOG_PRINT(CAN_WARN, __VA_ARGS__)
#define CINFO(...) CAN_LOG_PRINT(CAN_INFO, __VA_ARGS__)
#define CNORMAL(...) CAN_LOG_PRINT(CAN_NORMAL, __VA_ARGS__)
#define CDEBUG(...) CAN_LOG_PRINT(CAN_DEBUG, __VA_ARGS__)
#define CTRACE(...) CAN_LOG_PRINT(CAN_TRACE, __VA_ARGS__)
#define CVERBOSE(...) CAN_LOG_PRINT(CAN_VERBOSE, __VA_ARGS__)



namespace technotools {
    CANOPEN_API void initLogger(const int argc, const char* argv[]);
    CANOPEN_API void finiLogger();
    CANOPEN_API FILE* getLogFile();
    CANOPEN_API void setLogFile(FILE* fp);
};
