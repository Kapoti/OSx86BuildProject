/* OS x86 Build Project IDT Interrupt Process Function Defines
 * kernel/idtcall.c
 */

#include <klibc/stdint.h>
#include <klibc/stddef.h>
#include <stduart.h>
#include <idtcall.h>

void pic_init(void) {
	uint8_t save_pic0 = inb(PIC0_DAT);
	uint8_t save_pic1 = inb(PIC1_DAT);
	
	// Ready to init
	outb(PIC0_CMD, 0x11);
	outb(PIC1_CMD, 0x11);
	
	// Set offset
	outb(PIC0_DAT, 0x20);    // IRQ 0~7 -> int 0x20~0x27
	outb(PIC1_DAT, 0x28);    // IRQ 8~15 -> int 0x28~0x2f
	
	// Set Slave-PIC connect
	outb(PIC0_DAT, 0x04);
	outb(PIC1_DAT, 0x02);
	
	// Set mode
	outb(PIC0_DAT, 0x01);
	outb(PIC1_DAT, 0x01);
	
	outb(PIC0_DAT, save_pic0);
	outb(PIC1_DAT, save_pic1);
	
	return;
}

