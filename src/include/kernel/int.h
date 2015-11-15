/**
 * @file include/kernel/int.h
 * Kernel operations for enabling and disabling interrupts.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_INT_H_
#define __INCLUDE_KERNEL_INT_H_

/**
 * Enable interrupts.
 */
static inline void sti(){
	asm volatile("sti");
}

/**
 * Disable interrupts.
 */
static inline void cli(){
	asm volatile("cli");
}

#endif

