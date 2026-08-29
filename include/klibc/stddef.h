/* OS x86 Build Project Standard Defines
 * include/stddef.h
 * WARNING: NOT A STANDARD GNU HEADER
 */
#ifndef _STDDEF_H
#define _STDDEF_H

#include <klibc/stdint.h>

#define NULL ((void*)0)
#define offsetof(type, member) ((size_t)&(((type*)0)->member))

typedef uint32_t size_t;
typedef int32_t ssize_t;

#define U8_MAX   0xFF
#define U16_MAX  0xFFFFU
#define U32_MAX  0xFFFFFFFFUL
#define U64_MAX  0xFFFFFFFFFFFFFFFFULL
#define SIZE_MAX U32_MAX    // The size_t is the typedef of uint32_t (unsigned int)

#define range(var, start, stop, step) for(typeof(start) var = (start); var < (stop); var += (step))
#define lengthof(arr) (sizeof(arr) / sizeof((arr)[0]))

#define pass void(0)

#endif
