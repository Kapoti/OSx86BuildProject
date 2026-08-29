/* OS x86 Build Project Description Table Defines Header
 * include/desctbl.h
 */

#ifndef _DESCTBL_H
#define _DESCTBL_H

#include <klibc/stdint.h>
#include <klibc/stddef.h>



/* GDT Defines */
#define GDT_SIGN_G 0x80
#define GDT_SIGN_D 0x40
#define GDT_SIGN_L 0x20
#define GDT_SIGN_A 0x10

#define GDT_SIGN_PM (GDT_SIGN_G | GDT_SIGN_D)

#define GDT_AC_PR   0x80
#define GDT_AC_DPL0 0x00
#define GDT_AC_DPL1 0x02
#define GDT_AC_DPL2 0x04
#define GDT_AC_DPL3 0x60
#define GDT_AC_S_SY 0x00
#define GDT_AC_S_CD 0x10
#define GDT_AC_EX   0x08
#define GDT_AC_DC   0x04
#define GDT_AC_RW   0x02
#define GDT_AC_AC   0x01

#define GDT_SEGM_KERN_CODE (GDT_AC_PR | GDT_AC_DPL0 | GDT_AC_S_CD | GDT_AC_EX | GDT_AC_RW)
#define GDT_SEGM_KERN_DATA (GDT_AC_PR | GDT_AC_DPL0 | GDT_AC_S_CD | GDT_AC_RW)
#define GDT_SEGM_USER_CODE (GDT_AC_PR | GDT_AC_DPL3 | GDT_AC_S_CD | GDT_AC_EX | GDT_AC_RW)
#define GDT_SEGM_USER_DATA (GDT_AC_PR | GDT_AC_DPL3 | GDT_AC_S_CD | GDT_AC_RW)

struct GDT_Entry {
	uint16_t limit_low16, base_low16;
	uint8_t  base_mid8;
	uint8_t  accessrght;
	uint8_t  limit_high4, base_high8;
} __attribute__((packed));

struct GDT_RegLoad {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void gdt_buildstruct(struct GDT_Entry *item, uintptr_t base, size_t limit, uint8_t ar, uint8_t sign);
void gdt_loadreg(struct GDT_Entry *table_ptr, size_t table_size);



/* IDT Defines */
struct IDT_Entry {
	uint16_t base_low16;
	uint16_t select;
	uint8_t  rsvd;
	uint8_t  attr;
	uint16_t base_high16;
} __attribute((packed));

struct IDT_RegLoad {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

void idt_buildstruct(struct IDT_Entry *item, uintptr_t base, uint16_t select, uint8_t attr);
void idt_loadreg(struct IDT_Entry *table_ptr, size_t table_size);

#endif
