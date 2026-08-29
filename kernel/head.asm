; OS x86 Build Project ELF Program Assembly Header
; kernel/head.asm

bits 32

global _AsmHeadInit

extern ProgInit
extern ProgMain
extern ProgExit

section .text
_AsmHeadInit:
	mov esp, stack_s
	jmp init

init:
	push ebx    ; BootInfo
	push eax    ; Magic
	call ProgInit
	cmp eax, 0
	jnz ProgExit

main:
	call ProgMain

exit:
	call ProgExit

stop:
	cli
	hlt
	jmp stop


section .rodata
db "OS x86 Build Project"
db "(c) Project Novalight"
align 8

section .bss
stack_e:
resq 1024
stack_s:
