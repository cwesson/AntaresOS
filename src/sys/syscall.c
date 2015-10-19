/**
 * @file sys/syscall.c
 * System call handlers.
 * @author Conlan Wesson
 */

#include "syscall.h"

#include <stdint.h>
#include "sys/interrupt/isr.h"

//! Array of system call handler functions.
static sys_func sys_funcs[256] = {0};

/**
 * System call handler.
 * @param num System call number.
 * @param payload System call specific information.
 * @return System call specific return data.
 */
inline syscall_payload *syscall(int num, syscall_payload *payload){
	asm volatile(
		"int $0x80;"
		:: "a"(num), "b"(payload)
	);
	return payload;
}

/**
 * System call ISR handler.
 * @param regs The registers at the time of the interrupt.
 */
static void syscall_isr(isr_regs regs){
	if(sys_funcs[regs.eax]){
		sys_func handler = sys_funcs[regs.eax];
		*(uint32_t*)regs.ebx = (uint32_t)handler(regs);
	}else{
		*(uint32_t*)regs.ebx = SYS_CALL_BAD;
	}
}

/**
 * Initializes the system call handler.
 */
void syscall_init(){
	isr_register(ISR_SYSCALL, &syscall_isr);
}

/**
 * Registers a system call handler.
 * @param n The syscall number.
 * @param handler Function pointer of the callback function.
 */
void syscall_register(uint8_t n, sys_func handler){
	sys_funcs[n] = handler;
}

