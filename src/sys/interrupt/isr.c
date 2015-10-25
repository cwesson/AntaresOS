/**
 * @file interrupt/isr.c
 * Functions for interrupt handlers.
 * @author Conlan Wesson
 */

#include "isr.h"

#include <kernel/ioport.h>
#include <kernel/panic.h>
#include <stdint.h>
#include <stdio.h>
#include "dev/ram.h"

#define PIC_EOI 0x20    //!< End-of-Interrupt command

/**
 * Strings for standard BIOS interrupts.
 */
static const char *const int_labels[] = {
	"Divide by Zero",
	"Debug Exception",
	"Non-maskable Interrupt",
	"Breakpoint Exception",
	"Into Detected Overflow",
	"Out of Bounds Exception",
	"Invalid Opcode Exception",
	"No Coprocessor Exception",
	"Double Fault",
	"Coprocessor Segment Overrun",
	"Bad TSS",
	"Segment Not Present",
	"Stack Fault",
	"General Protection Fault",
	"Page Fault",
	"Unknown Interrupt Exception",
	"Coprocessor Fault",
	"Alignment Check Exception",
	"Machine Check Exception"
};

//! Array of ISR callback functions.
static isr interrupt_handlers[256] = {0};

/**
 * Handler for standard ISRs.
 * @param regs Registers from before the interrupt.
 */
void isr_handler(isr_regs regs){
	regs.int_no &= 0xFF;
	if(interrupt_handlers[regs.int_no] != 0){
		isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}else if(regs.int_no != 1){
		printf("Unhandled ISR:  %u", regs.int_no);
		if(regs.int_no < sizeof(int_labels)/sizeof(char*)){
			putchar(' ');
			puts(int_labels[regs.int_no]);
		}
		printf("\nError Code:  %u\n", regs.err_code);
		printf("eax = %8X       ebx = %8X       ecx = %8X       edx = %8X\n", regs.eax, regs.ebx, regs.ecx, regs.edx);
		ram_dump((uint8_t*)regs.esp);
		panic("Unhandled ISR");
	}
}

/**
 * Handler for standard IRQs.
 * @param regs Registers from before the interrupt.
 */
void irq_handler(isr_regs regs){
	if(interrupt_handlers[regs.int_no] != 0){
		isr handler = interrupt_handlers[regs.int_no];
		handler(regs);
	}else{
		printf("Unhandled IRQ:  %u\n", regs.int_no);
	}

	// Send EOI signal to the PICs.
	if(regs.int_no >= PIC_SLAVE_IRQ_START){
		outb(PIC_SLAVE_CMD, PIC_EOI);
	}
	outb(PIC_MASTER_CMD, PIC_EOI);
}

/**
 * Registers an interrupt handler.
 * @param n The interrupt number.
 * @param handler Function pointer of the callback function.
 */
void isr_register(uint8_t n, isr handler){
	interrupt_handlers[n] = handler;
}

