/* OS x86 Build Project IDT Interrupt Defines Header
 * kernel/idtcall.c
 */

#ifndef _IDTCALL_H
#define _IDTCALL_H

#include <stdint.h>

#define PIC0_CMD      0x20
#define PIC0_DAT      0x21
#define PIC1_CMD      0xa0
#define PIC1_DAT      0xa1
#define INT_PROCDONE  0x20

#endif
