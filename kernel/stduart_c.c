/* OS x86 Build Project Standard UART Upper C Function Defines
 * kernel/stduart_c.c
 */


#include <klibc/stdarg.h>
#include <klibc/stdint.h>
#include <stduart.h>
#include <klibc/string.h>
#include <klibc/stddef.h>

// Upper C uart_printf function
void uart_printf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	for (const char *p = fmt; *p; p++) {
		if (*p != '%') {
			uart_putchar(*p);
			continue;
		}
		p++;
		switch (*p) {
			case 's': {
				const char *s = va_arg(args, const char*);
				uart_puts(s);
				break;
			}
			case 'd': {
				int d = va_arg(args, int);
				char buf[12];
				itoa(d, buf, 10);
				uart_puts(buf);
				break;
			}
			case 'x': {
				uint32_t x = va_arg(args, uint32_t);
				char buf[12];
				itoa(x, buf, 16);
				uart_puts(buf);
				break;
			}
			case 'l': {
				p++;
				if (*p == 'l') {
					p++;
					if (*p == 'd') {
						int64_t d = va_arg(args, int64_t);
						char buf[24];
						itoa64(d, buf, 10);
						uart_puts(buf);
					} else if (*p == 'x') {
						uint64_t x = va_arg(args, uint64_t);
						char buf[24];
						itoa64(x, buf, 16);
						uart_puts(buf);
					}
				}
				break;
			}
			
			case 'u': {
				p ++;
				switch(*p){
					case 'd': {
					unsigned int d = va_arg(args, unsigned int);
					char buf[12];
					itoa(d, buf, 10);
					uart_puts(buf);
					break;
					}
					case 'l': {
						p ++;
						if (*p == 'l') {
							p++;
							if (*p == 'd') {
								uint64_t d = va_arg(args, uint64_t);
								char buf[24];
								itoa64(d, buf, 10);
								uart_puts(buf);
							}
						}
						break;
					}
				}
			}
			
			case 'c': {
				unsigned char c = (unsigned char)va_arg(args, int);
				uart_putchar(c);
				break;
			}
			case '%': {
				uart_putchar('%');
				break;
			}
			default: {
				uart_putchar('%');
				uart_putchar(*p);
				break;
			}
		}
	}
	va_end(args);
}
