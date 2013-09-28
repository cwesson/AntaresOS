/**
 * @file dev/ram.c
 * Functions for handling RAM information.
 * @author Conlan Wesson
 */

#include "ram.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

static uint64_t ram_mem_size = 0;
static struct mmap_entry *ram_mem_map = 0;
static uint32_t ram_mmap_len = 0;
static void *ram_heap_start = 0;

/**
 * Returns the amount of RAM.
 * @return The total amount of RAM in bytes.
 */
uint64_t ram_size(){
	return ram_mem_size;
}

/**
 * Returns the multiboot memory map.
 * @return The memory map structure.
 */
struct mmap_entry *ram_mmap(){
	return ram_mem_map;
}

/**
 * Returns the length of the multiboot memory map.
 * @return The length of the memory map in bytes.
 */
uint32_t ram_mmap_length(){
	return ram_mmap_len;
}

/**
 * Dumps 256 bytes of RAM to the screen.
 * @param addr The starting address to dump.
 */
void ram_dump(uint8_t *addr){
	addr = (uint8_t*)((uint32_t)addr & 0xFFFFFFF0);
	uint8_t *end = addr + 256;
	for(; addr < end; addr+=16){
		printf("\e[1;37;44m 0x%8X  ", (uint32_t)addr);
		for(int i = 0; i < 16; i++){
			printf("%2X ", *(addr+i));
		}
		puts("  ");
		for(int i = 0; i < 16; i++){
			if(isprint(*(addr+i))){
				putchar((char)*(addr+i));
			}else{
				putchar('.');
			}
		}
		puts("\e[0m\n");
	}
}

/**
 * Loads initial RAM info.
 * @param mmap Pointer to the memory map structure.
 * @param length Length of the memory map.
 * @param begin Pointer to the lowest address to use.
 */
void ram_init(struct mmap_entry *mmap, uint32_t length, void * begin){
	ram_heap_start = begin;
	ram_mem_map = mmap;
	ram_mmap_len = length;
	ram_mem_size = 0;
	while((unsigned int)mmap < (unsigned int)ram_mem_map + length){
		ram_mem_size += mmap->len;
		mmap = (struct mmap_entry*)((unsigned int)mmap + mmap->size + sizeof(unsigned int));
	}
}

/**
 * Returns a pointer to the end of the kernel.
 * @return The end of kernel memory.
 */
void *ram_kernel_end(){
	return ram_heap_start;
}
