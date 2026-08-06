#include <stdint.h>
#include <stddef.h>
#include <stduart.h>
#include <string.h>
#include <mtboot2.h>
#include <memman.h>

static uint8_t *mat;

int  ProgInit(uint32_t magic, struct MTBT2_InfoHeader *bootinfo){
	pinit();
	pputs("ProgInit\r\n");
	if(magic != MTBT2_MAGIC){
		pputs("[K_PANIC] invaild magic\r\n");
		return 1;
	}
	
	// Init MemoryManager
	mat = minit(bootinfo);
	if(mat == NULL){
		pputs("[KERN_PANIC] cannot init MAT32\r\n");
		return 1;
	}
	return 0;
}

void ProgMain(void){
	pputs("ProgMain\r\n");
	pputs("[KERN_INFO] Memory allocate test\r\n");
	uint8_t *p1 = (uint8_t *)malloc(mat, 15);
	if(!p1){
		pputs("[KERN_PANIC] Memory allocate test failed\r\n");
		return;
	}
	pprintf("[KERN_INFO] memory allocate test: p1 allocated to %x\r\n", (uint32_t)p1);
	pputs("[KERN_INFO] Memory allocate test done\r\n");
	
	while(1){
		__asm__("hlt");
	}
	return;
}

void ProgExit(void){
	pputs("ProgExit\r\n");
	return;
}
