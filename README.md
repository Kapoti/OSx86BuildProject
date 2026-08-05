# Operating System x86 Build Project README Document

## What is "Operating System x86"?
**Operating System x86** (Abbreviation: **OS x86**) is an 32-bit operating system build by **Project Novalight**.

## What can it do right now?
- Boot via GNU GRUB with MultiBoot 2 support
- Provides serial (UART) debug output via COM1
- Manage physical memory with a custom MAT32 bitmap allocator
- VBE framebuffer display (in progress)

## How can I build it on my device?

Before building, you need to install these packages and ensure they meet the **minimum required versions**:
- GNU GRUB Utils (`grub-mkrescue`) ≥ 2.12
- NASM ≥ 2.16
- GNU C Compiler ≥ 14
- ld.bfd ≥ 2.44
- GNU Make ≥ 4.4
- QEMU i386 Emulator ≥ 10.0

On Debian/Ubuntu-based systems, you can install them with:

```bash
sudo apt install nasm gcc binutils grub-pc qemu-system-x86
```

> **Note:** `grub-pc` provides the necessary `grub-mkrescue` tool for building the bootable ISO. It does *not* modify your system’s bootloader unless you explicitly run `grub-install`.

Clone or navigate to the project directory, then run:

```bash
make run
```

This will compile the kernel and launch it in the QEMU emulator, displaying the system window.

For more details, see [BUILD.md](BUILD.md).

## Who is behind this?
**Operating System x86** is developed by **Project Novalight**, an independent development group.

## What license does it use?
**Apache License 2.0**. See LICENSE.md for details. Third-party components are listed in THIRD_PARTY_NOTICES.md.

## How can I contribute?
Not open for contributions yet. Project is in early stages.
