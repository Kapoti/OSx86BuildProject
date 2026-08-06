# OS x86 Build Project Makefile
# Makefile

# Tools
AS = nasm                  # ASsembler
CC = gcc                   # C Compiler
LD = ld.bfd                # Linker
BL = grub-mkrescue         # BootLoader
EM = qemu-system-i386      # EMulator
RM = rm -rf

# Flags
AFLAGS = -f elf32
CFLAGS = -m32 -ffreestanding -fno-pie -no-pie -fno-pic -nostdlib -nostdinc -I include/ -g -Wall -c
LFLAGS = -T linker.ld -m elf_i386
EFLAGS = -display sdl -vga std -cdrom


# Directories & Files
DIR_BOOTLD = boot
DIR_KERNEL = kernel
DIR_BUILD  = build
DIR_INCLUD = include
FLE_KRNOBJ = mtbt2hd.o head.o main.o stduart.o stduart_c.o string.o memman.o stdgcc.o
DIR_GRUBLB = /usr/lib/grub/i386-pc/


# Normal rules
%.o: $(DIR_BOOTLD)/%.asm
	$(AS) $(AFLAGS) -o $@ $<

%.o: $(DIR_KERNEL)/%.c
	$(CC) $(CFLAGS) -o $@ $<

%.o: $(DIR_KERNEL)/%.asm
	$(AS) $(AFLAGS) -o $@ $<

oskernel: $(FLE_KRNOBJ)
	$(LD) $(LFLAGS) $^ -o $@

OSBuild.iso: oskernel
	mkdir -p $(DIR_BUILD)/boot/
	cp -r $(DIR_BOOTLD)/grub/ $(DIR_BUILD)/boot/
	cp oskernel $(DIR_BUILD)/boot/
	$(BL) --modules="vbe gfxterm" -d $(DIR_GRUBLB) -o $@ $(DIR_BUILD)
	

# Build commands

default: build

build: OSBuild.iso

run: OSBuild.iso
	$(EM) $(EFLAGS) OSBuild.iso

serial: OSBuild.iso
	$(EM) $(EFLAGS) OSBuild.iso -serial stdio

gdb: OSBuild.iso
	@echo "Please start GDB and use 'target remote localhost:1234' to connect QEMU"
	$(EM) $(EFLAGS) OSBuild.iso -serial stdio -s -S

clean:
	$(RM) oskernel OSBuild.iso *.o *.img build/

.PHONY: default build run serial clean
