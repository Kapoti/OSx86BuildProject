/* OS x86 Build Project Standard Defines
 * include/stddef.h
 * WARNING: NOT A STANDARD GNU HEADER
 */
#ifndef _STDDEF_H
#define _STDDEF_H

#include <stdint.h>

#define NULL ((void*)0)
#define offsetof(type, member) ((size_t)&(((type*)0)->member))

typedef uint32_t size_t;
typedef int32_t ssize_t;

typedef unsigned char bool;
#define false 0
#define true  1

#define U8_MAX 0xFF
#define U16_MAX 0xFFFFU
#define U32_MAX 0xFFFFFFFFUL
#define U64_MAX 0xFFFFFFFFFFFFFFFFULL

#define ALIGN_UP(x, n)   (((x) + (n) - 1) & ~((n) - 1))
#define ALIGN_DOWN(x, n) ((x) & ~((n) - 1))
#define IS_ALIGNED(x, n) (((x) & ((n) - 1)) == 0)

#endif
