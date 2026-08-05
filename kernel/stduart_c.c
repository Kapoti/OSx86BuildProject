#include <stdarg.h>
#include <stdint.h>
#include <stduart.h>
#include <string.h>

void pprintf(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	for (const char *p = fmt; *p; p++) {
		if (*p != '%') {
			pputchar(*p);
			continue;
		}
		p++;
		switch (*p) {
			case 's': {
				const char *s = va_arg(args, const char*);
				pputs(s);
				break;
			}
			case 'd': {
				int d = va_arg(args, int);
				char buf[12];
				itoa(d, buf, 10);
				pputs(buf);
				break;
			}
			case 'x': {
				uint32_t x = va_arg(args, uint32_t);
				char buf[12];
				itoa(x, buf, 16);
				pputs(buf);
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
						pputs(buf);
					} else if (*p == 'x') {
						uint64_t x = va_arg(args, uint64_t);
						char buf[24];
						itoa64(x, buf, 16);
						pputs(buf);
					}
				}
				break;
			}
			case 'c': {
				char c = (char)va_arg(args, int);
				pputchar(c);
				break;
			}
			case '%': {
				pputchar('%');
				break;
			}
			default: {
				pputchar('%');
				pputchar(*p);
				break;
			}
		}
	}
	va_end(args);
}
