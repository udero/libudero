#pragma once

#ifdef WIN32
#ifdef aqua_EXPORTS
#define AQUA_API __declspec(dllexport)
#else
#ifdef aquaStatic_EXPORTS
#define AQUA_API
#else
#define AQUA_API __declspec(dllimport)
#endif
#endif
#else
#define AQUA_API
#endif


#include "type.h"
