/**
 * @file sys/syscall.h
 * System call handlers.
 * @author Conlan Wesson
 */

#ifndef __SYS_SYSCALL_H_
#define __SYS_SYSCALL_H_

#include <stdint.h>
#include "interrupt/isr.h"

typedef enum
{
	SYSCALL_NONE = 0,
	SYSCALL_OPEN,
	SYSCALL_CLOSE,
	SYSCALL_READ,
	SYSCALL_WRITE,
	SYSCALL_LSEEK,
	
	SYSCALL_BAD = 0xBAD5CA11
} syscall_num;

//! Stores system call payload.
typedef union syscall_payload{
	int i;
	float f;
	void *v;
} syscall_payload;

/**
 * Function pointer to system call handlers.
 * @param registers The registers at the time of the interrupt.
 */
typedef void (*sys_func)(syscall_payload*);

/**
 * System call handler.
 * @param num System call number.
 * @param payload System call specfic information.
 * @return System call specific return data.
 * @mscfile syscall.msc
 */
syscall_payload *syscall(int num, syscall_payload *payload);

/**
 * Initializes the system call handler.
 */
void syscall_init();

/**
 * Registers a system call handler.
 * @param n The syscall number.
 * @param handler Function pointer of the callback function.
 */
void syscall_register(int n, sys_func handler);

#endif

