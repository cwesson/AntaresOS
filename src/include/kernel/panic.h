/**
 * @file include/kernel/panic.h
 * Kernel panic handler.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_PANIC_H_
#define __INCLUDE_KERNEL_PANIC_H_

#include <stdint.h>

/**
 * Throw a kernel panic handler.
 * @param msg Explanation of the panic.
 */
#define panic(msg) __panic_actual((msg), __FILE__, __LINE__, __func__)

/**
 * Actual kernel panic handler.
 * @param msg Explanation of the panic.
 * @param file File in which the panic occured.
 * @param line Line on which the panic occured.
 * @param func Function in which the panic occured.
 */
void __panic_actual(const char *msg, const char *file, uint32_t line, const char *func);

#endif /* __INCLUDE_KERNEL_PANIC_H_ */

