/**
 * @file include/kernel/ioport.h
 * Kernel operations for I/O ports.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_IOPORT_H_
#define __INCLUDE_KERNEL_IOPORT_H_

#include <stdint.h>

/**
 * Outputs a byte to the specified port.
 * @param port The output port.
 * @param val The byte to output.
 */
void outb(uint16_t port, uint8_t val);

/**
 * Reads a byte from the specified port.
 * @param port The input port.
 * @return The value read from input.
 */
uint8_t inb(uint16_t port);

/**
 * Outputs a long to the specified port.
 * @param port The output port.
 * @param val The long to output.
 */
void outl(uint16_t port, uint32_t val);

/**
 * Reads a long from the specified port.
 * @param port The input port.
 * @return The value long from input.
 */
uint32_t inl(uint16_t port);

#endif

