/**
 * @file dev/rtc.c
 * Driver for the CMOS Real-Time Clock (RTC).
 * @author Conlan Wesson
 */

#include "rtc.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include "sys/interrupt/isr.h"

/**
 * Converts a BCD number to a standard integer.
 * @param bcd The BCD number.
 * @return The standard integer equivelent.
 */
static inline uint8_t bcd_to_int(register uint8_t bcd){
	return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

/**
 * Converts a standard integer to a BCD number.
 * @param num The integer.
 * @return The BCD equivelent.
 */
static inline uint8_t int_to_bcd(register uint8_t num){
	return ((num / 10) << 4) | (num % 10);
}

/**
 * Gets the current date and time from the RTC.
 * @param dt Pointer to the date/time struct to fill.
 * @return Pointer to the date/time struct with the current time.
 */
struct datetime *rtc_time(datetime *dt){
	// Wait for any update to finish
	while(inb(RTC_REG_STATA) & RTC_FLAG_UPDATING){}
	
	uint8_t bcd = !inb(RTC_REG_STATB & RTC_FLAG_BIN);
	
	// Read seconds
	outb(RTC_REG_ADDRESS, RTC_REG_SEC);
	dt->sec = inb(RTC_REG_DATA);
	// Read minutes
	outb(RTC_REG_ADDRESS, RTC_REG_MIN);
	dt->min = inb(RTC_REG_DATA);
	// Read hours
	outb(RTC_REG_ADDRESS, RTC_REG_HOUR);
	uint8_t hour = inb(RTC_REG_DATA);
	// Correct hour
	if(!inb(RTC_REG_STATB & RTC_FLAG_24H)){
		if(hour & RTC_FLAG_PM){
			hour = hour & ~RTC_FLAG_PM;
			if(bcd){
				hour = bcd_to_int(hour);
			}
			hour = hour + 12;
			if(hour == 12){
				hour = 0;
			}
		}else{
			hour = bcd_to_int(hour);
		}
	}
	dt->hour = hour;
	// Read day of month
	outb(RTC_REG_ADDRESS, RTC_REG_DAY);
	dt->day = inb(RTC_REG_DATA);
	// Read month
	outb(RTC_REG_ADDRESS, RTC_REG_MONTH);
	dt->month = inb(RTC_REG_DATA);
	// Read year
	outb(RTC_REG_ADDRESS, RTC_REG_YEAR);
	dt->year = inb(RTC_REG_DATA);
	
	// Correct for BCD
	if(bcd){
		dt->sec = bcd_to_int(dt->sec);
		dt->min = bcd_to_int(dt->min);
		dt->day = bcd_to_int(dt->day);
		dt->month = bcd_to_int(dt->month);
		dt->year = bcd_to_int(dt->year);
	}
	dt->year += RTC_YEAR_BASE;
	
	return dt;
}

/**
 * Real-time clock interrupt handler.
 * @param regs Register struct from common ISR.
 */
static void rtc_isr(isr_regs regs){
	(void)regs;
}

/**
 * Initializes the CMOS real-time clock.
 */
void rtc_init(){
	// Register the timer callback
	isr_register(RTC_IRQ, &rtc_isr);
	
	// Read the current value of register B
	outb(RTC_REG_ADDRESS, RTC_REG_STATB);
	uint8_t prev = inb(RTC_REG_DATA);
	// Enable the IRQ
	outb(RTC_REG_ADDRESS, RTC_REG_STATB);
	outb(RTC_REG_DATA, prev | RTC_FLAG_IRQ);
}

