; OS x86 Build Project ELF Program Assembly Header
; kernel/head.asm

bits 32

global _AsmHeadInit
; The _end symbol will set by linker

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

section .rodata
db "OS x86 Build Project"
db "(c) Project Novalight"
align 8

section .bss
stack_e:
resq 128
stack_s:
