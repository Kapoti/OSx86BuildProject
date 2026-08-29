/* OS x86 Build Project Description Table Function Defines
 * kernel/desctbl.c
 */

#include <klibc/stdint.h>
#include <desctbl.h>
#include <stduart.h>

/* GDT Process Functions*/
void gdt_buildstruct(struct GDT_Entry *item, uintptr_t base, size_t limit, uint8_t ar, uint8_t sign) {
	if(!item) {
		uart_puts("[FUNC_PANIC] gdt_buildstruct: item is not a valid address\r\n");
		return;
	}
	
	item->base_low16  = (base >> 0)   & 0x0000ffff;
	item->limit_low16 = (limit >> 0)  & 0x0000ffff;
	item->base_mid8   = (base >> 16)  & 0x00ff;
	item->accessrght  = ar;
	item->limit_high4 = (limit >> 24) & 0x0f;
	item->limit_high4 |= sign & 0xf0;
	item->base_high8  = (base >> 24)  & 0xff;
	
	return;
}

void gdt_loadreg(struct GDT_Entry *table_ptr, size_t table_size) {
	if(!table_ptr) {
		uart_puts("[FUNC_PANIC] gdt_loadreg: table_ptr is not a valid address\r\n");
		return;
	}
	if(table_size < sizeof(struct GDT_Entry)) {
		uart_puts("[FUNC_PANIC] gdt_loadreg: cannot load a GDT table smaller than a item\r\n");
		return;
	}
	struct GDT_RegLoad gdtr = {
		.limit = table_size - 1,
		.base  = (uint32_t)table_ptr
	};
	
	__asm__ volatile(
		"lgdt %0"    // Assembly command
		:            // No output variables
		: "m"(gdtr)  // Input variables: memory address of gdtr
	);
	
	return;
}



/* IDT Process Functions */
void idt_buildstruct(struct IDT_Entry *item, uintptr_t base, uint16_t select, uint8_t attr) {
	if(!item) {
		uart_puts("[FUNC_PANIC] idt_buildstruct: item is not a valid address\r\n");
		return;
	}
	
	item->base_low16  = (base >> 0)  & 0x0000ffff;
	item->base_high16 = (base >> 16) & 0xffff;
	item->select      = select;
	item->attr        = attr;
	
	return;
}

void idt_loadreg(struct IDT_Entry *table_ptr, size_t table_size) {
	if(!table_ptr) {
		uart_puts("[FUNC_PANIC] idt_loadreg: table_ptr is not a valid address\r\n");
		return;
	}
	if(table_size < sizeof(struct IDT_Entry)) {
		uart_puts("[FUNC_PANIC] idt_loadreg: cannot load a IDT table smaller than a item\r\n");
		return;
	}
	
	struct IDT_RegLoad idtr = {
		.limit = table_size - 1,
		.base  = (uint32_t)table_ptr
	};
	
	__asm__ volatile(
		"lidt (%0)"       // Assembly command
		:                 // No output variables
		: "r"(table_ptr)  // move the value of table_ptr to a register
		: "memory"        // Warn: Will write memory
	);
	
	return;
}
