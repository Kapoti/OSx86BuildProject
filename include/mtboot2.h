/* OS x86 Build Project MultiBoot 2 Values Define Header
 * include/mtboot2.h
 */

#ifndef _MTBOOT2_H
#define _MTBOOT2_H

#include <klibc/stdint.h>

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
	uint64_t leng;
	uint32_t type;
	uint32_t rsvd;
};

struct MTBT2_AcpiV1 {
    uint32_t type;
    uint32_t size;
    uint8_t  magic[8];
    uint8_t  chk;
    uint8_t  oem[6];
    uint8_t  vers;
    uint32_t rsdt;
} __attribute__((packed));

struct MTBT2_AcpiV2 {
    uint32_t type;
    uint32_t size;
    uint8_t  magic[8];
    uint8_t  chk;
    uint8_t  oem[6];
    uint8_t  vers;
    uint32_t rsdt;
    uint32_t leng;
    uint64_t xsdt;
    uint8_t  exck;
    uint8_t  rsvd[3];
} __attribute__((packed));

struct MTBT2_TagHeader *mtbt2_findtag(struct MTBT2_InfoHeader *bootinfo, uint32_t type);

#endif
