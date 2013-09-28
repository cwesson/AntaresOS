/**
 * @file dev/ram.h
 * Functions for handling RAM information.
 * @author Conlan Wesson
 */

#ifndef DEV_RAM_H
#define DEV_RAM_H

#include <stdint.h>

#define RAM_BLOCK_USABLE 1

/**
 * Structure for holding memory map information, based on the multiboot mmap.
 */
struct mmap_entry{
	uint32_t size;  //!< Size of the entry in bytes, should always be 24.
	uint64_t addr;  //!< Starting address of the memory block.
	uint64_t len;   //!< Length of the memory block in bytes.
	uint32_t type;  //!< Type of the memory block, 1 if usable.
} __attribute__((packed));

/**
 * Returns the amount of RAM.
 * @return The total amount of RAM in bytes.
 */
uint64_t ram_size();

/**
 * Returns the multiboot memory map.
 * @return The memory map structure.
 */
struct mmap_entry *ram_mmap();

/**
 * Returns the length of the multiboot memory map.
 * @return The length of the memory map in bytes.
 */
uint32_t ram_mmap_length();

/**
 * Dumps 256 bytes of RAM to the screen.
 * @param addr The starting address to dump.
 */
void ram_dump(uint8_t *addr);

/**
 * Loads initial RAM info.
 * @param mmap Pointer to the memory map structure.
 * @param length Length of the memory map.
 * @param begin Pointer to the lowest address to use.
 */
void ram_init(struct mmap_entry *, uint32_t, void *);

/**
 * Returns a pointer to the end of the kernel.
 * @return The end of kernel memory.
 */
void *ram_kernel_end();

#endif
