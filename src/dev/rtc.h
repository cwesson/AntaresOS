/**
 * @file dev/rtc.h
 * Driver for the CMOS Real-Time Clock (RTC).
 * @author Conlan Wesson
 */

#ifndef __DEV_RTC_H_
#define __DEV_RTC_H_

#include <stdint.h>
#include <time.h>

/**
 * Gets the current date and time from the RTC.
 * @param dt Pointer to the date/time struct to fill.
 * @return Pointer to the date/time struct with the current time.
 */
struct tm *rtc_time(struct tm *);

/**
 * Initializes the CMOS real-time clock.
 */
void rtc_init();

#endif /* __DEV_RTC_H_ */

