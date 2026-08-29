/* OS x86 Build Project Linker Script */
/* linker.ld */

ENTRY(_AsmHeadInit)

SECTIONS
{
	. = 0x100000;
	
	kernstart = .;
	
	.multiboot2 : {
		*(.multiboot2)
	}
	
	kern_textstart = .;
	.text : {
		. = ALIGN(4);
		*(.text)
	}
	kern_textend = .;
	
	kern_datastart = .;
	.data : {
		*(.data)
	}
	
	.rodata : {
		*(.rodata)
	}
	
	.bss : {
		*(.bss)
		*(COMMON)
	}
	kern_dataend = .;
	
	. = ALIGN(4096);
	
	kernend = .;
}
