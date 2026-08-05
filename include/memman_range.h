/* OS x86 Build Project Range Memory Manage Defines Header
 * include/memman.h
 */

#ifndef _MEMMAN_RANGE_H
#define _MEMMAN_RANGE_H

#include <mtboot2.h>
#include <stdint.h>
#include <stddef.h>

#define AREA_OVERLAP(as, ae, bs, be) ((ae) > (bs) && (as) < (be))

struct MemMan {
	uint32_t type;
	uint32_t nxtw;
	uint32_t esiz;
	uint32_t count;
};

struct MemMan *minit(struct MTBT2_MemMapTag *bootinfo_mm);
uintptr_t mfind(struct MTBT2_MemMapTag *mm, size_t size, uint8_t exclc, uint64_t **exclv);
uintptr_t malloc(size_t size);
void mfree(uintptr_t addr);

#endif
