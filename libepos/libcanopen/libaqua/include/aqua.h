#pragma once

#ifdef WIN32

#ifdef aqua_EXPORTS
#define AQUA_API __declspec(dllexport)
#elif defined _WINDLL
#define AQUA_API __declspec(dllexport)
#elif defined aquaStatic_EXPORTS
#define AQUA_API
#else
#define AQUA_API __declspec(dllimport)
#endif

#else // WIN32
#define AQUA_API
#endif // WIN32


#include "type.h"
