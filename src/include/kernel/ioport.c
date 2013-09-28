/**
 * @file include/kernel/ioport.c
 * Kernel operations for I/O ports.
 * @author Conlan Wesson
 */

#include "ioport.h"

#include <stdint.h>

/**
 * Outputs a byte to the specified port.
 * @param port The output port.
 * @param val The byte to output.
 */
inline void outb(uint16_t port, uint8_t val){
	asm volatile(
		"outb %0,%1"
		::"a"(val), "d" (port)
	);
}

/**
 * Reads a byte from the specified port.
 * @param port The input port.
 * @return The value read from input.
 */
inline uint8_t inb(uint16_t port){
	uint8_t ret;
	asm volatile(
		"inb %1,%0"
		:"=a"(ret)
		:"Nd"(port)
	);
	return ret;
}

/**
 * Outputs a long to the specified port.
 * @param port The output port.
 * @param val The long to output.
 */
inline void outl(uint16_t port, uint32_t val){
	asm volatile(
		"outl %0,%1"
		::"a"(val), "d" (port)
	);
}

/**
 * Reads a long from the specified port.
 * @param port The input port.
 * @return The value long from input.
 */
inline uint32_t inl(uint16_t port){
	uint32_t ret;
	asm volatile(
		"inl %1,%0"
		:"=a"(ret)
		:"d"(port)
	);
	return ret;
}

