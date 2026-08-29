/* OS x86 Build Project GCC libgcc.a Feauture Defines Header
 * kernel/stdgcc.h
 * WARNING: NOT A STANDARD GNU CODE
 */

#ifndef _STDGCC_H
#define _STDGCC_H

#include <klibc/stdint.h>

#define EXTERN_LD(symbol) extern char symbol[]
#define GET_LD(symbol)    (uintptr_t)symbol

uint64_t __udivdi3(uint64_t a, uint64_t b);
uint64_t __umoddi3(uint64_t a, uint64_t b);
int64_t __divdi3(int64_t a, int64_t b);
int64_t __moddi3(int64_t a, int64_t b);

static inline uint32_t GCC_CTZ(uint32_t n) {
	return (uint32_t)__builtin_ctz((unsigned int)n);
}

#endif
