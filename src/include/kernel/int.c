/**
 * @file include/kernel/int.c
 * Kernel operations for enabling and disabling interrupts.
 * @author Conlan Wesson
 */

#include "int.h"

/**
 * Enable interrupts.
 */
inline void sti(){
	asm volatile("sti");
}

/**
 * Disable interrupts.
 */
inline void cli(){
	asm volatile("cli");
}

