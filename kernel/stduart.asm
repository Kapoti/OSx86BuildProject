; OS x86 Build Project Standard UART I/O
; kernel/stduart.asm

bits 32

; void outb(uint16_t port, uint8_t data);
global outb
outb:
	push ebp
	mov ebp, esp
	push eax
	push edx
	
	mov dx, [ebp + 8]
	mov al, [ebp + 12]
	out dx, al
	
	pop edx
	pop eax
	pop ebp
	ret

; void outw(uint16_t port, uint16_t data);
global outw
outw:
	push ebp
	mov ebp, esp
	push eax
	push edx
	
	mov dx, [ebp + 8]
	mov ax, [ebp + 12]
	out dx, ax
	
	pop edx
	pop eax
	pop ebp
	ret

; uint8_t inb(uint16_t port);
global inb
inb:
	push ebp
	mov ebp, esp
	push edx
	
	mov dx, [ebp + 8]
	xor eax, eax
	in al, dx
	
	pop edx
	pop ebp
	ret

; uint16_t inw(uint16_t port);
global inw
inw:
	push ebp
	mov ebp, esp
	push edx
	
	mov dx, [ebp + 8]
	xor eax, eax
	in ax, dx
	
	pop edx
	pop ebp
	ret

%define COM1 0x3f8

; void pinit(void);
global pinit
pinit:
	push edx
	push eax
	
	mov dx, (COM1 + 1)
	mov al, 0
	out dx, al
	
	mov dx, (COM1 + 3)
	mov al, 0x80
	out dx, al
	
	mov dx, (COM1 + 0)
	mov al, 1
	out dx, al
	
	mov dx, (COM1 + 3)
	mov al, 3
	out dx, al
	
	mov dx, (COM1 + 2)
	mov al, 0xc7
	out dx, al
	
	mov dx, (COM1 + 4)
	mov al, 0x3
	out dx, al
	
	pop eax
	pop edx
	ret

; uint8_t pgetstat(void)
global pgetstat
pgetstat:
	push edx
	
	mov dx, (COM1 + 5)
	in al, dx
	pop edx
	
	ret

; void pputchar(const char c);
global pputchar
pputchar:
	push ebp
	mov ebp, esp
	push edx
	push eax
	
	.wait:
		mov dx, (COM1 + 5)
		in al, dx
		test al, 0x20
		jz .wait
	
	mov dx, COM1
	mov al, [ebp + 8]
	out dx, al
	
	pop eax
	pop edx
	pop ebp
	ret

; void pputs(const char *s);
global pputs
pputs:
	push ebp
	mov ebp, esp
	push edx
	push eax
	push edi
	
	.init:
		mov edi, [ebp + 8]
		test edi, edi
		jz .done
	
	.check:
		mov al, [edi]
		cmp al, 0
		jz .done
	
	.wait:
		mov dx, (COM1 + 5)
		in al, dx
		test al, 0x20
		jz .wait
	
	.put:
		mov dx, COM1
		mov al, [edi]
		out dx, al
		inc edi
		jmp .check
	
	.done:
		pop edi
		pop eax
		pop edx
		pop ebp
		ret
