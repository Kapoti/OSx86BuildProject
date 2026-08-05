/* OS x86 Build Project MultiBoot 2 Values Define Header
 * include/mtboot2.h
 */

#ifndef _MTBOOT2_H
#define _MTBOOT2_H

#include <stdint.h>

#define MTBT2_MAGIC 0x36d76289

struct MTBT2_InfoHeader{
	uint32_t size;
	uint32_t rsvd;
};

struct MTBT2_TagHeader {
	uint32_t type;
	uint32_t size;
};

struct MTBT2_MemMapTag {
	uint32_t type;
	uint32_t size;
	uint32_t esiz;
	uint32_t vers;
};

struct MTBT2_MemEntryTag {
	uint64_t base;
	uint64_t size;
	uint32_t type;
	uint32_t rsvd;
};

#endif
