/* OS x86 Build Project Standard UART I/O Header
 * include/stduart.h
 */

#ifndef _STDUART_H
#define _STDUART_H

#include<stdint.h>

#define COM1 0x3f8

void outb(uint16_t port, uint8_t data);
void outw(uint16_t port, uint16_t data);
uint8_t inb(uint16_t port);
uint16_t inw(uint16_t port);

uint8_t pgetstat(void);

void pinit(void);
void pputchar(const char c);
void pputs(const char *s);
void pprintf(const char *fmt, ...);

#endif
