/* OS x86 Build Project Range Memory Manage Function Defines
 *kernel/memman.c
 */
#include <mtboot2.h>
#include <stdint.h>
#include <stduart.h>
#include <stddef.h>
#include <string.h>
#include <memman.h>

#define MAX_MEM_RECORD 64

uintptr_t mfind(struct MTBT2_MemMapTag *mm, size_t size, uint8_t exclc, uint64_t **exclv){
	uintptr_t mmbase = (uintptr_t)mm;
	uintptr_t mmptr = (uintptr_t)mm + sizeof(struct MTBT2_MemMapTag);
	uintptr_t mmend = (uintptr_t)mm + (uintptr_t)mm->size;
	
	struct MTBT2_MemEntryTag *mintag = NULL;
	uint64_t minsize = ~0UL;

	while(mmptr < mmend){
		bool isexcl = false;
		struct MTBT2_MemEntryTag *tag = (struct MTBT2_MemEntryTag *)mmptr;
		for(uint8_t i = 0; i < exclc; i ++){
			if(AREA_OVERLAP(tag->base, tag->base + tag->size, exclv[i][0], exclv[i][1])){
				isexcl = true;
				break;
			}
		}
		
		if(!isexcl && tag->type == 1 && tag->size >= size && tag->size < minsize){
			mintag = tag;
			minsize = tag->size;
		}
		mmptr += mm->esiz;
	}
	
	if(mintag == NULL){
		pputs("[FUNC_WARN] mfind: cannot find fit area\r\n");
		return (uintptr_t)NULL;
	}
	
	return (uintptr_t)mintag->base;
}

struct MemMan *minit(struct MTBT2_MemMapTag *bootinfo_mm){
	if(bootinfo_mm->type != 6){
		pputs("[FUNC_PANIC] minit: bootinfo_mm->type not 6\r\n");
		return NULL;
	}
	
	// Make the MemMan struct
	uint64_t *excludes[2] = {
		(uint64_t[]){(uint64_t)bootinfo_mm, (uint64_t)bootinfo_mm + (uint64_t)bootinfo_mm->size}, 
		(uint64_t[]){0x00000, 0xfffff}
	};
	struct MemMan *ret = (struct MemMan *)mfind(
		bootinfo_mm, 
		sizeof(struct MemMan)
		 + sizeof(struct MTBT2_MemEntryTag)
		 * MAX_MEM_RECORD, 
		 1, excludes);
	if(ret == NULL){
		pputs("[FUNC_PANIC] minit: no enough memory to init MemMan\r\n");
		return NULL;
	}
	
	// Set the sure values
	ret->type = 6;
	ret->esiz = bootinfo_mm->esiz;
	
	// Copy orginal records to new MemMan
	uintptr_t cpyd = (uintptr_t)ret + sizeof(struct MemMan);
	uintptr_t cpys = (uintptr_t)bootinfo_mm + sizeof(struct MTBT2_MemMapTag);
	size_t cpysize = bootinfo_mm->size - sizeof(struct MTBT2_MemMapTag);
	memcpy((void *)cpyd, (void *)cpys, cpysize);
	
	// Set the unsure values
	ret->count = cpysize / ret->esiz;
	ret->nxtw = ret->count + 1;
	
	return ret;
}

void mrmtag(struct MTBT2_MemMapTag *mm, uintptr_t tag);

uintptr_t malloc(size_t size);

void mfree(uintptr_t addr);
