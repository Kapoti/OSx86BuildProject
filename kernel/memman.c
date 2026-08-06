/* OS x86 Build Project MAT32 Memory Manage Function Defines
 *kernel/memman.c
 */

#include <memman.h>
#include <stdint.h>
#include <stddef.h>
#include <stduart.h>
#include <string.h>
#include <stdgcc.h>

EXTERN_LD(kernstart);
EXTERN_LD(kernend);

uintptr_t mfind_bootinfo(struct MTBT2_InfoHeader *bootinfo, size_t min_size) {
	
	/* Parse Bootinfo */
	struct MTBT2_MemMapTag *bootinfo_mm = (struct MTBT2_MemMapTag *)NULL;
	struct MTBT2_InfoHeader *bibase = (struct MTBT2_InfoHeader *)bootinfo;
	
	if(bibase->rsvd) pputs("[FUNC_WARN] mfind_bootinfo: bibase->rsvd not 0\r\n");
	
	uintptr_t biptr = (uintptr_t)bibase + 8;
	uintptr_t biend = (uintptr_t)bibase + (uintptr_t)bibase->size;
	while(biptr < biend) {
		struct MTBT2_TagHeader *tag = (struct MTBT2_TagHeader *)biptr;
		if(tag->type == 6) {
			bootinfo_mm = (struct MTBT2_MemMapTag *)biptr;
			break;
		}
		biptr += tag->size;
		biptr = (uintptr_t)ALIGN_UP(biptr, 8);
	}
	
	/* Find memory area */
	uintptr_t mmend  = (uintptr_t)bootinfo_mm + bootinfo_mm->size;
	uintptr_t kstart = GET_LD(kernstart);
	uintptr_t kend   = GET_LD(kernend);
	
	pprintf("kstart = %x; kend = %x\r\n", kstart, kend);
	
	size_t minsize = ~0UL;
	uintptr_t minptr = (uintptr_t)NULL;
	
	pprintf("[FUNC_INFO] struct MTBT2_MemMapTag *bootinfo_mm {\r\n"
			"	->type=%d;\r\n"
			"	->size=%d;\r\n"
			"	->esiz=%d;\r\n"
			"	->vers=%x;\r\n"
			"}\r\n", 
			bootinfo_mm->type, bootinfo_mm->size, bootinfo_mm->esiz, bootinfo_mm->vers);
	for(uintptr_t i = (uintptr_t)bootinfo_mm + sizeof(struct MTBT2_MemMapTag); i < mmend; i += bootinfo_mm->esiz) {
		struct MTBT2_MemEntryTag *tag = (struct MTBT2_MemEntryTag *)i;
		pprintf("[FUNC_INFO] i = %x; mmend = %x;\r\n", i, mmend);
		pprintf("[FUNC_INFO] struct MTBT2_MemEntryTag *tag {\r\n"
				"	->type = %d;\r\n"
				"	->base = %llx;\r\n"
				"	->size = %lld;\r\n"
				"}\r\n", 
				tag->type, tag->base, tag->size);
		
		// The kernel and bootinfo's type is same as free rams
		// so we need to do something to protect the kernel and bootinfo
		
		// if there is an area larger than the size we need
		if(tag->size >= MAT_SIZE) {
			// Check if this block overlaps with the kernel image or bootinfo
			if(AREA_OVERLAP(tag->base, tag->base + tag->size, kstart, kend)
			|| AREA_OVERLAP(tag->base, tag->base + tag->size, bootinfo, bootinfo + (uintptr_t)bootinfo->size)) {
				// If overlap, try to cut the kernel and bootinfo
				size_t section_left_size = (size_t)kstart - (size_t)tag->base;
				size_t section_right_size = (size_t)(tag->base + tag->size) - (size_t)kend;
				// Compare which size is fit
				if(section_left_size >= MAT_SIZE && section_left_size < minsize) {
					minsize = section_left_size;
					minptr = tag->base;
				} else if(section_right_size >= MAT_SIZE && section_right_size < minsize) {
					minsize = section_right_size;
					minptr = kend;
				} else {
					continue;
				}
			} else {
				minsize = tag->size;
				minptr = tag->base;
			}
		}
	}
	
	// The minsize does not update
	if(minsize == ~0UL) {
		pputs("[FUNC_WARN] mfind_bootinfo: cannot find fit memory area\r\n");
		return (uintptr_t)NULL;
	}
	
	return (uintptr_t)minptr;
}

uint8_t *minit(struct MTBT2_InfoHeader *bootinfo) {
	if(bootinfo->rsvd) pputs("[FUNC_WARN] minit: bootinfo->rsvd not 0\r\n");

	/* Init MAT32 Bitmap */
	uint8_t *mat = (uint8_t *)mfind_bootinfo(bootinfo, MAT_SIZE);
	
	if(!mat){
		pputs("[FUNC_PANIC] minit: no enough memory to init MAT32\r\n");
		return (uint8_t *)NULL;
	}
	
	/* Get Memory Status And Record */
	// Write to "all in use"
	mmark(mat, (uintptr_t)mat, (uintptr_t)mat + MAT_SIZE, true);
	
	// Find MemMapTag in GRUB bootinfo
	struct MTBT2_MemMapTag *bootinfo_mm = (struct MTBT2_MemMapTag *)NULL;
	struct MTBT2_InfoHeader *bibase = (struct MTBT2_InfoHeader *)bootinfo;
	
	if(bibase->rsvd) pputs("[FUNC_WARN] minit: bibase->rsvd not 0\r\n");
	
	uintptr_t biptr = (uintptr_t)bibase + 8;
	uintptr_t biend = (uintptr_t)bibase + (uintptr_t)bibase->size;
	while(biptr < biend) {
		struct MTBT2_TagHeader *tag = (struct MTBT2_TagHeader *)biptr;
		if(tag->type == 6) {
			bootinfo_mm = (struct MTBT2_MemMapTag *)biptr;
			break;
		}
		biptr += tag->size;
		biptr = (uintptr_t)ALIGN_UP(biptr, 8);
	}
	
	// Record records in bootinfo
	uintptr_t mmptr = (uintptr_t)bootinfo_mm + sizeof(struct MTBT2_MemMapTag);
	uintptr_t mmend  = (uintptr_t)bootinfo_mm + bootinfo_mm->size;
	while(mmptr < mmend){
		struct MTBT2_MemEntryTag *tag = (struct MTBT2_MemEntryTag *)mmptr;
		if(tag->type == 1){
			mmark(mat, tag->base, tag->base + tag->size, false);
		}
		
		mmptr += bootinfo_mm->esiz;
	}
	
	// Record kernel and bootinfo itself
	uintptr_t kstart = GET_LD(kernstart);
	uintptr_t kend   = GET_LD(kernend);
	mmark(mat, kstart, kend, true);
	mmark(mat, (uintptr_t)bootinfo, (uintptr_t)bootinfo + bootinfo->size, true);
	
	return (uint8_t *)mat;
}

#if 0    // Problem Functions

void mmark(uint8_t *mat, uintptr_t base, uintptr_t end, bool inUse) {
	// Check input value
	if(!mat) {
		pputs("[FUNC_PANIC] mmark: mat is not a valid address\r\n");
		return;
	}
	uintptr_t page_base = ALIGN_DOWN(base, BYTE_PER_BIT) / BYTE_PER_BIT, page_end = ALIGN_UP(end, BYTE_PER_BIT) / BYTE_PER_BIT;
	
	if(page_end - page_base < 1) {
		pputs("[FUNC_PANIC] mmark: cannot mark area smaller than 1 page\r\n");
		return;
	}
	
	uint8_t write_val = inUse ? 0xFF : 0x00;
	
	/*
		we need to mark an rea fastly.
		so we can use five stage to write:
		uint64_t uint32_t uint16_t uint8_t bit
		
		It's worth noting that dividing page_count by
		64, 32, 16, or 8 might leave a remainder, 
		so we need a variable to store the remainder
		to calculate the number of times the next loop will run.
	 */
	
	uintptr_t page_now = page_base;
	
	while(page_now % 64 == 0 && page_now + 64 <= page_end) {
		uint64_t *ptr = (uint64_t *)(mat + page_now / 8);
		*ptr = write_val ? U64_MAX : 0;
		page_now += 64;
	}
	
	while(page_now % 32 == 0 && page_now + 32 <= page_end) {
		uint32_t *ptr = (uint32_t *)(mat + page_now / 8);
		*ptr = write_val ? U32_MAX : 0;
		page_now += 32;
	}
	
	while(page_now % 16 == 0 && page_now + 16 <= page_end) {
		uint16_t *ptr = (uint16_t *)(mat + page_now / 8);
		*ptr = write_val ? U16_MAX : 0;
		page_now += 16;
	}
	
	while(page_now % 8 == 0 && page_now + 8 <= page_end) {
		uint8_t *ptr = (uint8_t *)(mat + page_now / 8);
		*ptr = write_val ? U8_MAX : 0;
		page_now += 8;
	}
	
	while(page_now % 1 == 0 && page_now + 1 <= page_end) {
		uint8_t *bitptr = (uint8_t *)(mat + page_now / 8);
		uint8_t bitmask = 1 << (page_now % 8);
		if(inUse) {
			*bitptr |= bitmask;
		} else {
			*bitptr &= ~bitmask;
		}
		page_now ++;
	}
	return;
}

uintptr_t mfind(uint8_t *mat, size_t size) {
	// Check input value
	if(!mat) {
		pputs("[FUNC_PANIC] mfind: mat is not a valid address\r\n");
		return (uintptr_t)NULL;
	} else if(size == 0) {
		pputs("[FUNC_PANIC] mfind: size must greater than zero\r\n");
		return (uintptr_t)NULL;
	}
	
	// Init variables
	uintptr_t fit_addr = (uintptr_t)NULL, got_addr = (uintptr_t)NULL;
	size_t fit_size = U32_MAX, got_size = 0;
	
	// Jump branches
	if(size > U16_MAX) {
		goto find_u32;
	} else if(size > U8_MAX) {
		goto find_u16;
	} else {
		goto find_u8;
	}
	
	/* Find area stage by stage*/
	uint8_t *ptr = mat;
	
	// search MAT32 by uint32_t
	find_u32: {
		uint32_t *ptr32 = (uint32_t *)ptr;
		size_t blocks = (MAT_SIZE - (ptr - mat)) / 4;
		for(size_t i = 0; i < MAT_SIZE / 4; i ++) {
			if(ptr32[i] == 0) {
				if(got_size == 0) {
					got_addr = (uintptr_t)(i * 4 * BYTE_PER_BIT);
				}
				got_size += 32 * BYTE_PER_BIT;
			} else {
				ptr = (uint8_t *)(ptr32 + i);
				goto find_u16;
			}
		}
		ptr = (uint8_t *)(ptr32 + blocks);
	}
	
	find_u16: {
		uint16_t *ptr16 = (uint16_t *)ptr;
		size_t blocks = (MAT_SIZE - (ptr - mat)) / 2;
		for(size_t i = 0; i < MAT_SIZE / 2; i ++) {
			if(ptr16[i] == 0) {
				if(got_size == 0) {
					got_addr = (uintptr_t)(i * 2 * BYTE_PER_BIT);
				}
				got_size += 16 * BYTE_PER_BIT;
			} else {
				ptr = (uint8_t *)(ptr16 + i);
				goto find_u8;
			}
		}
		ptr = (uint8_t *)(ptr16 + blocks);
	}
	
	find_u8: {
		uint8_t *ptr8 = ptr;
		size_t blocks = (MAT_SIZE - (ptr - mat));
		for(size_t i = 0; i < MAT_SIZE; i ++) {
			if(ptr8[i] == 0) {
				if(got_size == 0) {
					got_addr = (uintptr_t)(i * BYTE_PER_BIT);
				}
				got_size += 8 * BYTE_PER_BIT;
			} else {
				goto find_bit;
			}
		}
	}
	
	find_bit: {
		uint8_t *ptrbit = ptr;
		size_t bytes = MAT_SIZE - (ptr - mat);
		for(size_t i = 0; i < bytes; i ++) {
			for(uint8_t bit = 0; bit < 8; bit ++) {
				if((ptrbit[i] & (1 << bit)) == 0) {
					if(got_size == 0){
						got_addr = (uintptr_t)((ptrbit - mat) * 8 + i) * BYTE_PER_BIT;
					}
					got_size += BYTE_PER_BIT;
				} else {
					if(got_size >= size && got_size < fit_size) {
						fit_addr = got_addr;
						fit_size = got_size;
					}
					got_size = 0;
				}
			}
		}
		if (got_size >= size && got_size < fit_size) {
			fit_addr = got_addr;
			fit_size = got_size;
		}
	}
	
	if(fit_size == U32_MAX){
		pputs("[FUNC_PANIC] mfind: cannot find fit memory area\r\n");
		return (uintptr_t)NULL;
	}
	
	return (uintptr_t)fit_addr;
	
}

#endif

uintptr_t mfind(uint8_t *mat, size_t size){
	if(!mat) {
		pputs("[FUNC_PANIC] mfind: mat is not a valid address\r\n");
		return (uintptr_t)NULL;
	} else if(size == 0) {
		pputs("[FUNC_PANIC] mfind: size must greater than zero\r\n");
		return (uintptr_t)NULL;
	}
	
	size_t need_pages_count = ALIGN_UP(size, BYTE_PER_BIT) / BYTE_PER_BIT;
	uintptr_t got_addr = (uintptr_t)NULL, fit_addr = (uintptr_t)NULL;
	size_t got_page = 0, fit_page = U32_MAX;
	
	for(size_t page = 0; page < MAT_SIZE * 8; page ++) {
		uint8_t bit = 1 << ( page % 8);
		if((mat[page / 8] & bit) == 0){
			if(got_page == 0){
				got_addr = (uintptr_t)page * BYTE_PER_BIT;
			}
			got_page ++;
		} else {
			if(got_page >= need_pages_count && got_page < fit_page) {
				fit_addr = got_addr;
				fit_page = got_page;
			}
			got_page = 0;
		}
	}
	
	// Check last free page
	if (got_page >= need_pages_count && got_page < fit_page) {
		fit_addr = got_addr;
		fit_page = got_page;
	}
	
	if(fit_page == U32_MAX){
		pputs("[FUNC_PANIC] mfind: cannot find enough memory area\r\n");
		return (uintptr_t)NULL;
	}
	
	return fit_addr;
}

void mmark(uint8_t *mat, uintptr_t base, uintptr_t end, bool inUse) {
	
	if(!mat) {
		pputs("[FUNC_PANIC] mmark: mat is not a vaild address\r\n");
		return;
	}
	
	uintptr_t page_start = ALIGN_DOWN(base, BYTE_PER_BIT), page_end = ALIGN_UP(base, BYTE_PER_BIT);
	
	if(page_end <= page_start) {
		pputs("[FUNC_WARN] mmark: page_end smaller than page_start, default to page_start + BYTE_PER_BIT\r\n");
		page_end = page_start + BYTE_PER_BIT;
	}
	
	for(uintptr_t page = page_start; page < page_end; page ++) {
		uintptr_t byte_ind = page / 8;
		uint8_t bitmask = 1 << (page % 8);
		if(inUse) {
			mat[byte_ind] |= bitmask;
		} else {
			mat[byte_ind] &= ~bitmask;
		}
	}
	return;
}

uintptr_t malloc(uint8_t *mat, size_t size) {
	// Check input value
	if(!mat) {
		pputs("[FUNC_PANIC] malloc: mat is not a valid address\r\n");
		return (uintptr_t)NULL;
	} else if(size == 0) {
		pputs("[FUNC_PANIC] malloc: size must greater than zero\r\n");
		return (uintptr_t)NULL;
	}
	
	// Align to 4kb
	size_t real_size = ALIGN_UP(size, BYTE_PER_BIT);
	
	// if it's not enough
	if(size + 2 < real_size) {
		// allocate more
		real_size += BYTE_PER_BIT;
	}
	
	// find memory area
	uintptr_t ret = mfind(mat, real_size);
	pprintf("[FUNC_INFO] malloc: marking 0x%x - 0x%x as used\r\n", ret, ret + (uintptr_t)real_size);
	mmark(mat, ret, ret + (uintptr_t)real_size, true);
	
	size_t *write_size = (size_t *)ret;
	*write_size = (size_t)real_size;
	
	return ret + sizeof(real_size);
}
