/**
 * @file dev/pit.h
 * Driver for the Programmable Interval Timer (PIT).
 * @author Conlan Wesson
 */

#ifndef DEV_PIT_H
#define DEV_PIT_H

#include "../sys/interrupt/isr.h"

#define PIT_CH0_PORT 0x40
#define PIT_CH2_PORT 0x42
#define PIT_CH0_IRQ IRQ0
#define PIT_MODE_REG 0x43
#define PIT_BEEP_REG 0x61
#define PIT_BEEP_ENABLE 0x03
#define PIT_FLAG_CH0 0x00
#define PIT_FLAG_CH2 0x80
#define PIT_FLAG_HILO 0x30
#define PIT_FLAG_MODE2 0x04
#define PIT_FLAG_MODE3 0x06
#define PIT_FLAG_BIN 0x00

#define PIT_CLOCK_FREQ 1193180

/**
 * Plays a tone over the PC speaker.
 * @param frequency The frequency of the tone.
 */
void pit_tone(uint32_t frequency);

/**
 * Stops playing a tone over the PC speaker.
 */
void pit_nosound();

/**
 * Initialize the PIT Channel 0 timer.
 * @param frequency The frequency of interrupts to generate (19 - 1193180Hz).
 */
void pit_init(uint32_t frequency);

#endif
