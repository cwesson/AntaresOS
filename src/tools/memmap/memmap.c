/**
 * @file tools/memmap/memmap.c
 * Functions for handling the memory map.
 * @author Conlan Wesson
 */

#include "memmap.h"

#include <stdint.h>
#include <stdio.h>
#include "dev/ram.h"

/**
 * Prints the memory map.
 */
void memmap_print(){
	struct mmap_entry *mmap = ram_mmap();
	uint32_t length = ram_mmap_length();
	uint32_t addr = (uint32_t)mmap;
	while((uint32_t)mmap < addr + length){
		printf("0x%8X  %10dB Type %u\n", (uint32_t)mmap->addr, (uint32_t)mmap->len, mmap->type);
		mmap = (struct mmap_entry*)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
	}
}

