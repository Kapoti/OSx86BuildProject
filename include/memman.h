/* OS x86 Build Project MAT32 Memory Manage Defines Header
 * include/memman.h
 */

#ifndef _MEMMAN_MAT_H
#define _MEMMAN_MAT_H

#include <stdint.h>
#include <stddef.h>
#include <mtboot2.h>

#define BYTE_PER_BIT 4096
#define PE_MAX_MEM   (4ULL * 1024 * 1024 * 1024)
#define MAT_SIZE     (PE_MAX_MEM / (BYTE_PER_BIT * 8))

#define AREA_OVERLAP(as, ae, bs, be) ((uintptr_t)(ae) > (uintptr_t)(bs) && (uintptr_t)(as) < (uintptr_t)(be))

uint8_t  *minit(struct MTBT2_InfoHeader *bootinfo);
uintptr_t mfind(struct MTBT2_InfoHeader *bootinfo, size_t min_size);
void      mmark(uint8_t *mat, uintptr_t base, uintptr_t end, bool in_use);

#endif
