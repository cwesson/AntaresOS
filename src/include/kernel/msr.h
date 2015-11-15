/**
 * @file include/kernel/msr.h
 * Kernel operations for model specific registers.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_MSR_H_
#define __INCLUDE_KERNEL_MSR_H_

#include <stdint.h>

/**
 * Reads from a model specific register.
 * @param msr The model specific register to read.
 * @param hi Pointer to a store the high-order value at.
 * @param lo Pointer to a store the low-order value at.
 */
static inline void rdmsr(uint32_t msr, uint32_t *hi, uint32_t *lo){
	asm volatile(
		"rdmsr"
		:"=a"(*lo), "=d"(*hi)
		:"c"(msr)
	);
}

/**
 * Writes to a model specific register.
 * @param msr The model specific register to write.
 * @param hi High-order value to write.
 * @param lo Low-order value to write.
 */
static inline void wrmsr(uint32_t msr, uint32_t hi, uint32_t lo){
	asm volatile(
		"wrmsr"
		::"a"(lo), "d"(hi), "c"(msr)
	);
}

#endif /* __INCLUDE_KERNEL_MSR_H_ */

