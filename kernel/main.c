/* OS x86 Build Project Kernel Main Process Function Defines
 * kernel/main.c
 */

#include <klibc/stdint.h>
#include <klibc/stddef.h>
#include <klibc/string.h>
#include <klibc/stdgcc.h>
#include <stduart.h>
#include <mtboot2.h>
#include <memman.h>
#include <desctbl.h>

EXTERN_LD(kernstart);
EXTERN_LD(kernend);

static uint8_t mat_bitmap[MAT_SIZE];
static struct MTBT2_AcpiV2 *acpi_table = (struct MTBT2_AcpiV2 *)NULL;

int  ProgInit(uint32_t magic, struct MTBT2_InfoHeader *bootinfo){
	uart_init();
	uart_puts("ProgInit\r\n");
	if(magic != MTBT2_MAGIC){
		uart_puts("[KERN_PANIC] invaild magic\r\n");
		return 1;
	}
	
	/* Init MemMan */
	memman_init(mat_bitmap, bootinfo);
	
	/* Init GDT */
	struct GDT_Entry gdt[5] = {0};
	gdt_buildstruct(&gdt[1], 0x00000000, 0xffffffff, GDT_SEGM_KERN_CODE, GDT_SIGN_PM);
	gdt_buildstruct(&gdt[2], 0x00000000, 0xffffffff, GDT_SEGM_KERN_DATA, GDT_SIGN_PM);
	gdt_buildstruct(&gdt[3], 0x00000000, 0xffffffff, GDT_SEGM_USER_CODE, GDT_SIGN_PM);
	gdt_buildstruct(&gdt[4], 0x00000000, 0xffffffff, GDT_SEGM_USER_DATA, GDT_SIGN_PM);
	gdt_loadreg(gdt, sizeof(gdt));
	
	/* Init IDT */
	struct IDT_Entry idt[5] = {0};
	
	return 0;
}

void ProgMain(void){
	uart_puts("ProgMain\r\n");
	
	uart_puts("[KERN_INFO] memory allocate test start\r\n");
	uint8_t *p1 = (uint8_t *)memman_alloc(mat_bitmap, 114);
	uint32_t *p2 = (uint32_t *)memman_alloc(mat_bitmap, 1025);
	if(!p1 || !p2 || (p1 == p2)) {
		uart_puts("[KERN_PANIC] memory allocate test falied\r\n");
		return;
	}
	uart_puts("[KERN_INFO] memory allocate test done\r\n");
	memman_free(mat_bitmap, (uintptr_t)p1);
	memman_free(mat_bitmap, (uintptr_t)p2);
	
	while(1){
		__asm__("hlt");
	}
	return;
}

void ProgExit(void){
	uart_puts("ProgExit\r\n");
	return;
}
