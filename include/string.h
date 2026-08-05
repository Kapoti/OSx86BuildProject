/* OS x86 Build Project Standard String Process Header
 * include/string.h
 * WARNING: NOT A STANDARD GNU HEADER
 */
 
#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

unsigned int strlen(const char *s);
int atoi(const char *s);
static inline unsigned char isspace(char c) {
    return (c == ' '  || c == '\t' || c == '\n' ||
            c == '\r' || c == '\v' || c == '\f');
}
char *itoa(int val, char *targ, int base);
char *itoa64(long long int val, char *targ, int base);
void *memcpy(void *dest, const void *src, size_t n);
#endif
