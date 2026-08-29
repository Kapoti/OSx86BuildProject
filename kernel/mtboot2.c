#include <mtboot2.h>
#include <klibc/stdint.h>
#include <klibc/stddef.h>
#include <stduart.h>
#include <memman.h>

struct MTBT2_TagHeader *mtbt2_findtag(struct MTBT2_InfoHeader *bootinfo, uint32_t type) {
	if(bootinfo == (struct MTBT2_InfoHeader *)NULL) {
		uart_puts("[FUNC_PANIC] find_mtbt2tag: bootinfo is not a valid address\r\n");
		return (struct MTBT2_TagHeader *)NULL;
	} else if(bootinfo->rsvd) {
		uart_puts("[FUNC_WARN] find_mtbt2tag: bootinfo->rsvd not zero\r\n");
	}
	
	struct MTBT2_TagHeader *tag = (struct MTBT2_TagHeader *)NULL;
	uintptr_t biptr = (uintptr_t)bootinfo + sizeof(struct MTBT2_InfoHeader);
	uintptr_t biend = (uintptr_t)bootinfo + bootinfo->size;

	while(biptr < biend) {
		tag = (struct MTBT2_TagHeader *)biptr;
		if(tag->type == type) {
			return tag;
		}
		biptr += tag->size;
		biptr = ALIGN_UP(biptr, 8);
	}
	uart_puts("[FUNC_PANIC] find_mtbt2tag: cannot find needed tag\r\n");
	return (struct MTBT2_TagHeader *)NULL;
}
