/* OS x86 Build Project MAT32 Memory Manage Function Defines
 * kernel/memman.c
 */

#include <memman.h>
#include <klibc/stdint.h>
#include <klibc/stdbool.h>
#include <klibc/stdgcc.h>
#include <stduart.h>

EXTERN_LD(kernstart);
EXTERN_LD(kernend);

void memman_markarea(uint8_t *mat, uintptr_t base, uintptr_t end, bool inUse) {
	if(!mat) {
		uart_puts("[FUNC_PANIC] memman_markarea: mat is not a valid address\r\n");
		return;
	}
	
	if(base >= end) {
		uart_puts("[FUNC_PANIC] memman_markarea: invalid area\r\n");
		return;
	}
	
	uintptr_t page_start = ALIGN_DOWN(base, BYTES_PER_BIT) / BYTES_PER_BIT;
	uintptr_t page_end   = ALIGN_UP(end, BYTES_PER_BIT) / BYTES_PER_BIT;
	
	for(uintptr_t page = page_start; page < page_end; page ++) {
		uintptr_t byte_ind = page / 8;
		uint8_t   bit_mask = 1 << (page % 8);
		
		if(inUse) {
			mat[byte_ind] |= bit_mask;
		} else {
			mat[byte_ind] &= ~bit_mask;
		}
		
	}
	return;
}

void memman_init(uint8_t *mat, struct MTBT2_InfoHeader *bootinfo) {
	if(!mat) {
		uart_puts("[FUNC_PANIC] memman_init: mat is not a valid address\r\n");
		return;
	}
	if(!bootinfo) {
		uart_puts("[FUNC_PANIC] memman_init: bootinfo is not a valid address\r\n");
		return;
	}
	
	struct MTBT2_MemMapTag *bimm = (struct MTBT2_MemMapTag *)mtbt2_findtag(bootinfo, 6);
	if(!bimm) {
		uart_puts("[FUNC_PANIC] memman_init: bimm is not a valid address\r\n");
		return;
	}
	
	memman_markarea(mat, 0, PE_MAX_MEM, true);
	
	uintptr_t mmptr = (uintptr_t)bimm + sizeof(struct MTBT2_MemMapTag);
	uintptr_t mmend = (uintptr_t)bimm + bimm->size;
	while(mmptr < mmend) {
		struct MTBT2_MemEntryTag *tag = (struct MTBT2_MemEntryTag *)mmptr;
		if(tag->type == 1) {
			memman_markarea(mat, (uintptr_t)tag->base, (uintptr_t)tag->base + tag->leng, false);
		}
		mmptr += bimm->esiz;
	}
	
	memman_markarea(mat, GET_LD(kernstart), GET_LD(kernend), true);
	memman_markarea(mat, (uintptr_t)bootinfo, (uintptr_t)bootinfo + bootinfo->size, true);
	
	return;
}


uintptr_t mfind(uint8_t *mat, size_t size) {
	if(!mat) {
		uart_puts("[FUNC_PANIC] mfind: mat is not a valid address\r\n");
		return (uintptr_t)NULL;
	}
	if(size == 0) {
		uart_puts("[FUNC_PANIC] mfind: size must greater than 0\r\n");
		return (uintptr_t)NULL;
	}
	
	size_t real_need_page = ALIGN_UP(size, BYTES_PER_BIT) / BYTES_PER_BIT;
	uintptr_t got_page_no = 0, fit_page_no = 0;
	size_t    got_page_ct = 0, fit_page_ct = SIZE_MAX;
	
	for(uintptr_t page = 0; page < MAT_SIZE * 8; page ++) {
		uintptr_t byte_idx = page / 8;
		uint8_t   bit_mask = 1 << (page % 8);
		if((mat[byte_idx] & bit_mask) == 0) {
			if(got_page_no == 0) {
				got_page_no = page;
			}
			got_page_ct ++;
		} else {
			if(got_page_ct >= real_need_page && got_page_ct < fit_page_ct) {
				fit_page_no = got_page_no;
				fit_page_ct = got_page_ct;
			}
			got_page_no = 0;
			got_page_ct = 0;
		}
	}
	
	if(got_page_ct >= real_need_page && got_page_ct < fit_page_ct) {
		fit_page_no = got_page_no;
		fit_page_ct = got_page_ct;
	}
	
	if(fit_page_ct == SIZE_MAX) {
		uart_puts("[FUNC_PANIC] mfind: cannot find fit area\r\n");
		return (uintptr_t)NULL;
	}
	
	return fit_page_no * BYTES_PER_BIT;
}

uintptr_t memman_alloc(uint8_t *mat, size_t size) {
	if(!mat) {
		uart_puts("[FUNC_PANIC] memman_alloc: mat is not a valid address\r\n");
		return (uintptr_t)NULL;
	}
	if(size == 0) {
		uart_puts("[FUNC_PANIC] memman_alloc: size must greater than 0\r\n");
		return (uintptr_t)NULL;
	}
	
	size_t alloc_size = ALIGN_UP(size, BYTES_PER_BIT);
	uintptr_t alloc_addr = mfind(mat, alloc_size);
	
	if(!alloc_addr) {
		uart_puts("[FUNC_PANIC] memman_alloc: cannot fin fit area\r\n");
		return (uintptr_t)NULL;
	}

	memman_markarea(mat, alloc_addr, alloc_addr + (uintptr_t)alloc_size, true);
	*((size_t *)alloc_addr) = alloc_size;
	return alloc_addr + sizeof(size_t);
}

void memman_free(uint8_t *mat, uintptr_t ptr) {
	if(!mat) {
		uart_puts("[FUNC_PANIC] memman_free: mat is not a valid address\r\n");
		return;
	}
	if(!ptr) {
		uart_puts("[FUNC_PANIC] memman_free: ptr is not a valid address\r\n");
		return;
	}
	size_t free_size = *((size_t *)ptr - 1);
	/* Because the ptr is changed to size_t pointer
	 * So this will move ptr forward a unit
	 * and the unit is size_t, so it's ok */
	uintptr_t free_addr = ptr - sizeof(size_t);
	memman_markarea(mat, free_addr, free_addr + (uintptr_t)free_size, false);

	return;
}
