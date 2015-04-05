/**
 * @file dev/pit.h
 * Driver for the Programmable Interval Timer (PIT).
 * @author Conlan Wesson
 */

#ifndef __DEV_PIT_H_
#define __DEV_PIT_H_

#include <stdint.h>

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

#endif /* __DEV_PT_H_ */ 

