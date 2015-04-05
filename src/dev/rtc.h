/**
 * @file dev/rtc.h
 * Driver for the CMOS Real-Time Clock (RTC).
 * @author Conlan Wesson
 */

#ifndef __DEV_RTC_H_
#define __DEV_RTC_H_

#include <stdint.h>
#include "../sys/interrupt/isr.h"

enum {
	RTC_REG_STATA   = 0x0A,
	RTC_REG_STATB   = 0x0B,
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
 * Struct to store date and time information.
 */
typedef struct datetime{
	uint8_t sec;   //!< Number of seconds (0-60, 60 being leap second)
	uint8_t min;   //!< Number of minute (0-59)
	uint8_t hour;  //!< Number of hour (0-23)
	uint8_t day;   //!< Day of the month (1-31)
	uint8_t month; //!< Month of the year (1-12)
	uint16_t year; //!< The full year
} datetime;

/**
 * Gets the current date and time from the RTC.
 * @param dt Pointer to the date/time struct to fill.
 * @return Pointer to the date/time struct with the current time.
 */
struct datetime *rtc_time(struct datetime *);

/**
 * Initializes the CMOS real-time clock.
 */
void rtc_init();

#endif /* __DEV_RTC_H_ */

