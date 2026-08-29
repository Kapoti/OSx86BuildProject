/* OS x86 Build Project String Process Function Defines
 * kernel/string.c
 * WARNING: NOT A STANDARD GNU CODE
 */
#include <klibc/string.h>
#include <klibc/stddef.h>
#include <klibc/stdbool.h>

unsigned int strlen(const char *s){
	unsigned int ret = 0;
	while(s[ret]) ret ++;
	return ret;
}

int atoi(const char *s) {
    int ret = 0;
    int sign = 1;
    unsigned char in_number = 0;

    while (isspace(*s)) s++;

    if (*s == '-') {
        sign = -1;
        s++;
    } else if (*s == '+') {
        s++;
    }

    while (*s >= '0' && *s <= '9') {
        ret = ret * 10 + (*s - '0');
        s++;
        in_number = 1;
    }

    return in_number ? ret * sign : 0;
}

char *itoa(int val, char *targ, int base) {
    const char digits[] = "0123456789abcdef";
    unsigned int num;
    int bits = 0;
    int i;

    if (base != 10 && base != 16) {
        targ[0] = '\0';
        return targ;
    }

    if (val == 0) {
        targ[0] = '0';
        targ[1] = '\0';
        return targ;
    }

    if (val < 0 && base == 10) {
        num = (unsigned int)(-val);
    } else {
        num = (unsigned int)val;
    }

    unsigned int tmp = num;
    while (tmp > 0) {
        bits++;
        tmp /= base;
    }

    int index = bits;
    tmp = num;
    while (tmp > 0) {
        int digit = tmp % base;
        targ[--index] = digits[digit];
        tmp /= base;
    }

    if (val < 0 && base == 10) {
        for (i = bits; i >= 0; i--) {
            targ[i + 1] = targ[i];
        }
        targ[0] = '-';
        bits++;
    }

    targ[bits] = '\0';
    return targ;
}

char *itoa64(long long int val, char *targ, int base) {
    const char digits[] = "0123456789abcdef";
    unsigned long long int num;
    int bits = 0;
    int i;

    if (base != 10 && base != 16) {
        targ[0] = '\0';
        return targ;
    }

    if (val == 0) {
        targ[0] = '0';
        targ[1] = '\0';
        return targ;
    }

    if (val < 0 && base == 10) {
        num = (unsigned long long int)(-val);
    } else {
        num = (unsigned long long int)val;
    }

    unsigned long long int tmp = num;
    while (tmp > 0) {
        bits++;
        tmp /= base;
    }

    int index = bits;
    tmp = num;
    while (tmp > 0) {
        int digit = tmp % base;
        targ[--index] = digits[digit];
        tmp /= base;
    }

    if (val < 0 && base == 10) {
        for (i = bits; i >= 0; i--) {
            targ[i + 1] = targ[i];
        }
        targ[0] = '-';
        bits++;
    }

    targ[bits] = '\0';
    return targ;
}
		
void *memcpy(void *dest, const void *src, size_t n){
	unsigned char *dp = (unsigned char *)dest;
	const unsigned char *sp = (const unsigned char *)src;
	for(size_t i = 0; i < n; i ++){
		dp[i] = sp[i];
	}
	return dest;
}

bool strcmp(const char *s1, const char *s2) {
	for(char cmp = 0; ; cmp ++) {
		if(s1[cmp] != s2[cmp]) {
			return false;
		}
		if(s1[cmp] == '\0') {
			return true;
		}
	}
	return false;
}

bool memcmp(const void *s1, const void *s2, size_t n) {
	const uint8_t *p1 = (uint8_t *)s1;
	const uint8_t *p2 = (uint8_t *)s2;
	
	for(size_t i = 0; i < n; i ++) {
		if(p1[i] != p2[i]) return false;
	}
	
	return true;
}

bool strncmp(const char *s1, const char *s2, size_t n) {
	return memcmp(s1, s2, n);
}

