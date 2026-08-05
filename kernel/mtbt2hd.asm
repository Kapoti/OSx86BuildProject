; OS x86 Build Project Multiboot 2 Header
; boot/mtboot2.asm

section .multiboot2

header_s:

    ; Header of Multiboot 2 Header
    .taghead_s:
    dd 0xe85250d6            ; Magic
    dd 0                     ; Arch(i386)
    dd (header_e - header_s) ; Size
    ; CheckSum
    dd 0x100000000 - (0xe85250d6 + 0 + (header_e - header_s))
    .taghead_e:

    align 8
    
    ; Get info from GRUB 2
    .infotag_s:
        dw 1                 ; Get Info
        dw 0                 ; Flag = 0
        ; Total Size
        dd (.infotag_e - .infotag_s)
        dd 6                 ; MemoryMapInfo
        dd 8                 ; FrameBufferInfo
    .infotag_e:
    
    align 8
    
    %define width  800
    %define height 600
    %define bpp    16
    .vbeset_s:
		dw 5                 ; VBE mode
		dw 0                 ; Flag = 0
		; Total Size
		dd (.vbeset_e - .vbeset_s)
		dd width
		dd height
		dd bpp
    .vbeset_e:
    
    align 8

    dw 0                     ; End Info Request
    dw 0                     ; Flag = 0
    dd 8                     ; Total Size

header_e:
