/**
 * @file interrupt/isr.h
 * Functions for interrupt handlers.
 * @author Conlan Wesson
 */

#ifndef __INTERRUPT_ISR_H_
#define __INTERRUPT_ISR_H_

#include <stdint.h>

// Master PIC IRQs
#define IRQ0  32  //!< PIT Ch 0
#define IRQ1  33  //!< Keyboard
#define IRQ2  34  //!< PIC Cascade
#define IRQ3  35  //!< COM2
#define IRQ4  36  //!< COM1
#define IRQ5  37
#define IRQ6  38  //!< Floppy
#define IRQ7  39
// Slave PIC IRQs
#define IRQ8  40  //!< CMOS real-time clock
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44  //!< Mouse
#define IRQ13 45
#define IRQ14 46  //!< Primary ATA
#define IRQ15 47  //!< Secondary ATA
#define ISR_SYSCALL 128  //!< System Call

#define PIC_SLAVE_IRQ_START IRQ8  //!< First IRQ on the slave PIC

#define PIC_MASTER_CMD  0x20    //!< Command register for master PIC
#define PIC_MASTER_DATA 0x21    //!< Data register for master PIC
#define PIC_SLAVE_CMD   0xA0    //!< Command register for slave PIC
#define PIC_SLAVE_DATA  0xA1    //!< Data register for slave PIC

/**
 * Struct for storing registers for interrupts.
 */
typedef struct isr_regs{
   uint32_t ds;                  //!< Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; //!< Pushed by pusha.
   uint32_t int_no, err_code;    //!< Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; //!< Pushed by the processor automatically.
} isr_regs;

/**
 * Function pointer to ISR handlers.
 * @param registers The registers at the time of the interrupt.
 */
typedef void (*isr)(isr_regs);

/**
 * Registers an interrupt handler.
 * @param n The interrupt number.
 * @param handler Function pointer of the callback function.
 */
void isr_register(uint8_t, isr);

#endif
