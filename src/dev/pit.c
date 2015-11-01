/**
 * @file dev/pit.c
 * Driver for the Programmable Interval Timer (PIT).
 * @author Conlan Wesson
 */

#include "pit.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include <stdio.h>
#include "sys/interrupt/isr.h"

#define PIT_CH0_IRQ (IRQ0)
#define PIT_BEEP_ENABLE (0x03)

enum {
	PIT_CH0_PORT = 0x40,    //!< PIT channel 0 data port.
	PIT_CH2_PORT = 0x42,    //!< PIT channel 2 data port.
	PIT_MODE_REG = 0x43,    //!< PIT mode/command port.
	PIT_BEEP_REG = 0x61     //!< PC Speaker register.
};
enum {
	PIT_FLAG_CH0   = 0x00,    //!< Select channel 0.
	PIT_FLAG_CH2   = 0x80,    //!< Select channel 2.
	PIT_FLAG_HILO  = 0x30,    //!< Use high/low access mode.
	PIT_FLAG_MODE2 = 0x04,    //!< Rate generator.
	PIT_FLAG_MODE3 = 0x06,    //!< Square wave generator.
	PIT_FLAG_BIN   = 0x00     //!< 16bit binary mode.
};

const float PIT_CLOCK_FREQ = 1193180;

/**
 * Frequency of PIT Channel 0.
 */
static uint32_t ch0_freq = 0;
static float resolution = 0;
static uint64_t tick = 0;

/**
 * Callback function for interrupt from PIT Channel 0.
 * @param regs Register struct from common ISR.
 */
static void pit_callback(isr_regs regs){
	(void)regs;
	
	++tick;
}


/**
 * Plays a tone over the PC speaker.
 * @param frequency The frequency of the tone.
 */
void pit_tone(uint32_t frequency){
	// Calculate the clock divisor.
	uint32_t divisor = (int)((((PIT_CLOCK_FREQ / (float)frequency)*10.0)+0.5)/10.0);
	
	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t lo = (uint8_t)(divisor & 0xFF);
	uint8_t hi = (uint8_t)((divisor >> 8) & 0xFF);
	
	// Send the command byte.
	outb(PIT_MODE_REG, (PIT_FLAG_CH2 | PIT_FLAG_HILO | PIT_FLAG_MODE3 | PIT_FLAG_BIN));
	// Send the frequency divisor.
	outb(PIT_CH2_PORT, lo);
	outb(PIT_CH2_PORT, hi);
	
	// Enable the tone
	outb(PIT_BEEP_REG, inb(PIT_BEEP_REG) | PIT_BEEP_ENABLE);
}

/**
 * Stops playing a tone over the PC speaker.
 */
void pit_nosound(){
	outb(PIT_BEEP_REG, inb(PIT_BEEP_REG) & ~PIT_BEEP_ENABLE);
}

/**
 * Initialize the PIT Channel 0 timer.
 * @param frequency The frequency of interrupts to generate (19 - 1193180Hz).
 */
void pit_init(uint32_t frequency){
	ch0_freq = frequency;
	// Register the timer callback.
	isr_register(PIT_CH0_IRQ, &pit_callback);
	
	// Calculate the clock divisor.
	uint32_t divisor = (int)((((PIT_CLOCK_FREQ / (float)frequency)*10.0)+0.5)/10.0);
	
	// Calulate actual tick resolution in microseconds
	resolution = (1.0 / (PIT_CLOCK_FREQ / (float)divisor)) * 1000000.0;
	
	// Divisor has to be sent byte-wise, so split here into upper/lower bytes.
	uint8_t lo = (uint8_t)(divisor & 0xFF);
	uint8_t hi = (uint8_t)((divisor >> 8) & 0xFF);
	
	// Send the command byte.
	outb(PIT_MODE_REG, (PIT_FLAG_CH0 | PIT_FLAG_HILO | PIT_FLAG_MODE2 | PIT_FLAG_BIN));
	// Send the frequency divisor.
	outb(PIT_CH0_PORT, lo);
	outb(PIT_CH0_PORT, hi);
}

