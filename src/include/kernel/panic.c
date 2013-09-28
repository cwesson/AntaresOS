/**
 * @file include/kernel/panic.c
 * Kernel panic handler.
 * @author Conlan Wesson
 */

#include "panic.h"

#include <stdint.h>
#include <stdio.h>

/**
 * Actual kernel panic handler.
 * @param msg Explanation of the panic.
 * @param file File in which the panic occured.
 * @param line Line on which the panic occured.
 * @param func Function in which the panic occured.
 */
void __panic_actual(const char *msg, const char *file, uint32_t line, const char *func){
	printf("\n\e[31mKernel Panic: %s:%d: %s: %s\n", file, line, func, msg);
	
	asm volatile(
		"hlt;"
	);
	
	while(1){}
}

