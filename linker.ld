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
	
	.text : {
		. = ALIGN(4);
		*(.text)
	}
	
	.data : {
		*(.data)
	}
	
	/* Cannot sign 'rodata' to .rodata section*/
	.rodata : {
		*(.rodata)
	}
	
	.bss : {
		*(.bss)
		*(COMMON)
	}
	
	. = ALIGN(4096);
	
	kernend = .;
}
