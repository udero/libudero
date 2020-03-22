/**
 * @filename type.h
 * @author Yuki Suga (ysuga.net)
 * @copyright Sugar Sweet Robotics Co., Ltd.
 */

#pragma once

/**
 * setting for stdint.h
 */
#if defined (_MSC_VER) && (_MSC_VER == 1500) // VC2008�p
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
#else
#include <stdint.h> // VC2010��������Linux�p
#endif

