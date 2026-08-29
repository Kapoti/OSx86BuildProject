; OS x86 Build Project IDT Interrupt Assembly Process Defines
; idtcall.asm

bits 32

global asm_int_on20
asm_int_on20:
	pusha
	push ds
	push es
	push fs
	push gs

	push esp
	call int_on20
	add esp, 4
	
	iret

