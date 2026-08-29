/* OS x86 Build Project MAT32 Memory Manage Defines Header
 * include/memman.h
 */

#ifndef _MEMMAN_MAT_H
#define _MEMMAN_MAT_H

#include <klibc/stdint.h>
#include <klibc/stddef.h>
#include <mtboot2.h>
#include <klibc/stdbool.h>

#define ALIGN_UP(x, n)   (((uintptr_t)(x) + (uintptr_t)(n) - 1) & ~((uintptr_t)(n) - 1))
#define ALIGN_DOWN(x, n) ((uintptr_t)(x) & ~((uintptr_t)(n) - 1))
#define IS_ALIGNED(x, n) (((uintptr_t)(x) & ((uintptr_t)(n) - 1)) == 0)

#define PTR_IN_AREA(as, ae, p) ((uintptr_t)p >= (uintptr_t)as && (uintptr_t)p <= (uintptr_t)ae)

#define PE_MAX_MEM (4ULL * 1024 * 1024 * 1024) // 4GB
#define BYTES_PER_BIT 4096
#define MAT_SIZE (PE_MAX_MEM / (BYTES_PER_BIT * 8)) // = 128KB

static inline bool PTR_IN_AREAS(uintptr_t p, uintptr_t areas[][2], uint32_t areac) {
	for(uint32_t i = 0; i < areac; i ++) {
		if(PTR_IN_AREA(areas[i][0], areas[i][1], p)) {
			return true;
		}
	}
	return false;
}

void      memman_init(uint8_t *mat, struct MTBT2_InfoHeader *bootinfo);
uintptr_t memman_alloc(uint8_t *mat, size_t size);
void      memman_free(uint8_t *mat, uintptr_t ptr);

#endif
