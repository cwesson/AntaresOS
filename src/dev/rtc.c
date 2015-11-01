/**
 * @file dev/rtc.c
 * Driver for the CMOS Real-Time Clock (RTC).
 * @author Conlan Wesson
 */

#include "rtc.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include "sys/interrupt/isr.h"
#include <stdio.h>

enum {
	RTC_REG_STATA   = 0x0A,
	RTC_REG_STATB   = 0x0B,
	RTC_REG_STATC   = 0x0C,
	RTC_REG_ADDRESS = 0x70,
	RTC_REG_DATA    = 0x71
};
enum {
	RTC_FLAG_UPDATING = 0x80,
	RTC_FLAG_24H      = 0x02,
	RTC_FLAG_BIN      = 0x04,
	RTC_FLAG_PM       = 0x80,
	RTC_FLAG_IRQ      = 0x40
};
enum {
	RTC_REG_SEC   = 0x00,
	RTC_REG_MIN   = 0x02,
	RTC_REG_HOUR  = 0x04,
	RTC_REG_DAY   = 0x07,
	RTC_REG_MONTH = 0x08,
	RTC_REG_YEAR  = 0x09
};

#define RTC_YEAR_BASE     (2000)

#define RTC_IRQ (IRQ8)

#define RTC_IRQ_FREQ (1024)

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
 * Calculates the day of the week.
 * @param year The year of the date to calculate.
 * @param month The month of the date to calculate (1-12).
 * @param day The day of the month of the date to calculate (1-31).
 * @return The day of the week, 0 => Sunday, 6 => Saturday.
 */
uint8_t day_of_week(uint16_t year, uint8_t month, uint8_t day){
	int a = (14-month)/12;
	int y = year-a;
	int m = month + 12*a - 2;
	return (day + y + y/4 - y/100 + y/400 + (31*m/12)) % 7;
}

/**
 * Gets the current date and time from the RTC.
 * @param dt Pointer to the date/time struct to fill.
 * @return Pointer to the date/time struct with the current time.
 */
struct tm *rtc_time(struct tm *dt){
	// Wait for any update to finish
	while(inb(RTC_REG_STATA) & RTC_FLAG_UPDATING){}
	
	uint8_t bcd = !inb(RTC_REG_STATB & RTC_FLAG_BIN);
	
	// Read seconds
	outb(RTC_REG_ADDRESS, RTC_REG_SEC);
	dt->tm_sec = inb(RTC_REG_DATA);
	// Read minutes
	outb(RTC_REG_ADDRESS, RTC_REG_MIN);
	dt->tm_min = inb(RTC_REG_DATA);
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
	dt->tm_hour = hour;
	// Read day of month
	outb(RTC_REG_ADDRESS, RTC_REG_DAY);
	dt->tm_mday = inb(RTC_REG_DATA);
	// Read month
	outb(RTC_REG_ADDRESS, RTC_REG_MONTH);
	dt->tm_mon = inb(RTC_REG_DATA);
	// Read year
	outb(RTC_REG_ADDRESS, RTC_REG_YEAR);
	dt->tm_year = inb(RTC_REG_DATA);
	
	// Correct for BCD
	if(bcd){
		dt->tm_sec = bcd_to_int(dt->tm_sec);
		dt->tm_min = bcd_to_int(dt->tm_min);
		dt->tm_mday = bcd_to_int(dt->tm_mday);
		dt->tm_mon = bcd_to_int(dt->tm_mon);
		dt->tm_year = bcd_to_int(dt->tm_year);
	}
	dt->tm_year += RTC_YEAR_BASE-1900;
	
	dt->tm_mon--;
	dt->tm_wday = day_of_week(dt->tm_year+1900, dt->tm_mon+1, dt->tm_mday);
	
	return dt;
}

/**
 * Real-time clock interrupt handler.
 * @param regs Register struct from common ISR.
 */
static void rtc_isr(isr_regs regs){
	static uint64_t count = 0;
	static uint8_t prev_min = 255;
	(void)regs;
	
	++count;
	
	if(count == RTC_IRQ_FREQ){
		struct tm dt;
		rtc_time(&dt);
		if(dt.tm_sec == 0 && prev_min != dt.tm_min){
			printf("\e[s\e[0;0H\e[K\e[7m%04d-%02d-%02d %02d:%02d\e[27m\e[u", dt.tm_year+1900, dt.tm_mon+1, dt.tm_mday, dt.tm_hour, dt.tm_min);
		}
		count = 0;
		prev_min = dt.tm_min;
	}
	
	// Reset interrupt
	outb(RTC_REG_ADDRESS, RTC_REG_STATC);
	(void)inb(RTC_REG_DATA);
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

