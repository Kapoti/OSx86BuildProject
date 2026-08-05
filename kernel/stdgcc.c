/* OS x86 Build Project GCC libgcc.a Feauture Function Defines
 * kernel/stdgcc.c
 * WARNING: NOT A STANDARD GNU CODE
 */

#include <stdint.h>

// Unsigned 64-bit integer (qword) division and modulo functions
uint64_t __udivdi3(uint64_t a, uint64_t b) {
	if (b == 0) return 0;    // div 0 protect
	uint64_t q = 0;
	
	while (a >= b) {
		// find max 2^k * b, not above of a
		uint64_t step = 1;
		uint64_t temp_b = b;
		while (a >= temp_b << 1) {
			temp_b <<= 1;
			step <<= 1;
		}
		a -= temp_b;
		q += step;
	}
	return q;
}

uint64_t __umoddi3(uint64_t a, uint64_t b) {
	if (b == 0) return 0;  // div 0 protect
	// calculate mod
	while (a >= b) {
		uint64_t temp_b = b;
		uint64_t multiple = 1;
		while (a >= temp_b << 1) {
			temp_b <<= 1;
			multiple <<= 1;
		}
		a -= temp_b;
		// update a only
	}
	return a;
}

// Signed 64-bit integer (qword) division and modulo functions
int64_t __divdi3(int64_t a, int64_t b) {
	if (b == 0) return 0;    // div 0 protect
	
	int sign = 1;    // get sign
	if (a < 0) { a = -a; sign = -sign; }
	if (b < 0) { b = -b; sign = -sign; }

	// calculate with unsigned version
	uint64_t result = __udivdi3((uint64_t)a, (uint64_t)b);

	return (int64_t)(sign * result);
}

int64_t __moddi3(int64_t a, int64_t b) {
	if (b == 0) return 0;    // div 0 protect
	
	int sign = (a < 0) ? -1 : 1;    // get sign
	if (a < 0) a = -a;
	if (b < 0) b = -b;

	// calculate with unsigned version
	uint64_t result = __umoddi3((uint64_t)a, (uint64_t)b);

	return (int64_t)(sign * result);
}
