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

#ifdef _MEMMAN_MAT_H

uintptr_t mfind_bootinfo(struct MTBT2_InfoHeader *bootinfo, size_t min_size){
	
	/* Parse Bootinfo */
	struct MTBT2_MemMapTag *bootinfo_mm = (struct MTBT2_MemMapTag *)NULL;
	struct MTBT2_InfoHeader *bibase = (struct MTBT2_InfoHeader *)bootinfo;
	
	if(bibase->rsvd) pputs("[FUNC_WARN] mfind_bootinfo: bibase->rsvd not 0\r\n");
	
	uintptr_t biptr = (uintptr_t)bibase + 8;
	uintptr_t biend = (uintptr_t)bibase + (uintptr_t)bibase->size;
	while(biptr < biend){
		struct MTBT2_TagHeader *tag = (struct MTBT2_TagHeader *)biptr;
		if(tag->type == 6){
			bootinfo_mm = (struct MTBT2_MemMapTag *)biptr;\
			break;
		}
		biptr += tag->size;
		biptr = (uintptr_t)ALIGN_UP(biptr, 8);
	}
	
	/* Find memory area */
	uintptr_t mmend = (uintptr_t)bootinfo_mm + bootinfo_mm->size;
	uintptr_t kstart = GET_LD(kernstart);
	uintptr_t kend   = GET_LD(kernend);
	
	pprintf("kstart = %x; kend = %x\r\n", kstart, kend);
	
	size_t minsize = ~0UL;
	struct MTBT2_MemEntryTag *mintag = (struct MTBT2_MemEntryTag *)NULL;
	
	pprintf("[FUNC_INFO] struct MTBT2_MemMapTag *bootinfo_mm {\r\n"
			"	->type=%d;\r\n"
			"	->size=%d;\r\n"
			"	->esiz=%d;\r\n"
			"	->vers=%x;\r\n"
			"}\r\n", 
			bootinfo_mm->type, bootinfo_mm->size, bootinfo_mm->esiz, bootinfo_mm->vers);
	for(uintptr_t i = (uintptr_t)bootinfo_mm + sizeof(struct MTBT2_MemMapTag); i < mmend; i += bootinfo_mm->esiz){
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
	}
	
	// The minsize does not update
	if(minsize == ~0UL){
		pputs("[FUNC_WARN] mfind_bootinfo: cannot find fit memory area\r\n");
		return (uintptr_t)NULL;
	}
	
	return (uintptr_t)mintag->base;
}

uint8_t *minit(struct MTBT2_InfoHeader *bootinfo){
	if(bootinfo->rsvd) pputs("[FUNC_WARN] minit: bootinfo->rsvd not 0\r\n");
	
	/* Parse Bootinfo */
	struct MTBT2_MemMapTag *bootinfo_mm = (struct MTBT2_MemMapTag *)NULL;
	struct MTBT2_InfoHeader *bibase = (struct MTBT2_InfoHeader *)bootinfo;
	
	if(bibase->rsvd) pputs("[FUNC_WARN] minit: bibase->rsvd not 0\r\n");
	
	uintptr_t biptr = (uintptr_t)bibase + 8;
	uintptr_t biend = (uintptr_t)bibase + (uintptr_t)bibase->size;
	while(biptr < biend){
		struct MTBT2_TagHeader *tag = (struct MTBT2_TagHeader *)biptr;
		if(tag->type == 6){
			bootinfo_mm = (struct MTBT2_MemMapTag *)biptr;
			break;
		}
		biptr += tag->size;
		biptr = (uintptr_t)ALIGN_UP(biptr, 8);
	}
	
	if(bootinfo_mm->type != 6){
		pputs("[FUNC_PANIC] minit: bootinfo_mm->type not 6\r\n");
		return (uint8_t *)NULL;
	}
	
	uint8_t *mat = (uint8_t *)mfind_bootinfo(bootinfo, MAT_SIZE);
	if(mat == NULL){
		pputs("[FUNC_PANIC] minit: cannot find enough memory to init MAT32\r\n");
		return NULL;
	}
	uintptr_t mmptr = (uintptr_t)bootinfo_mm + sizeof(struct MTBT2_MemMapTag);
	uintptr_t mmend = (uintptr_t)bootinfo_mm + bootinfo_mm->size;
	
	/* Init MAT32 */
	// init to "all in use"
	uint8_t *matptr = (uint8_t *)mat;
	while((uintptr_t)matptr < (uintptr_t)mat + MAT_SIZE){
		*matptr = 0xff;
		matptr ++;
	}
	
	// Record type=1's records to MAT32
	matptr = mat;
	while(mmptr < mmend){
		struct MTBT2_MemEntryTag *tag = (struct MTBT2_MemEntryTag *)mmptr;
		if(tag->type == 1) mmark(mat, (uintptr_t)tag->base, (uintptr_t)(tag->base + tag->size), false);
		mmptr += bootinfo_mm->esiz;
	}
	
	// Record kernel and bootinfo's records to MAT32
	mmark(mat, GET_LD(kernstart), GET_LD(kernend), true);
	mmark(mat, (uintptr_t)bibase, biend, true);
	
	return mat;
}

void mmark(uint8_t *mat, uintptr_t base, uintptr_t end, bool inUse) {
    // Align to page's size (4kb)
    uintptr_t page_start = ALIGN_DOWN(base, BYTE_PER_BIT);
    uintptr_t page_end   = ALIGN_UP(end, BYTE_PER_BIT);

    // Calculate start and end page number with base address is 0
    uintptr_t start_page = page_start / BYTE_PER_BIT;
    uintptr_t end_page   = page_end / BYTE_PER_BIT;

    // range each page and set pages
    for (uintptr_t i = start_page; i < end_page; i++) {
        uintptr_t byte_idx = i / 8;
        uint8_t bit_mask = 1 << (i % 8);

        if (inUse) {
            mat[byte_idx] |= bit_mask;   // set to 1 (in use)
        } else {
            mat[byte_idx] &= ~bit_mask;  // set to 0 (free)
        }
    }
    return;
}

#endif
