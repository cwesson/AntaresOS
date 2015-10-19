/**
 * @file tools/date/date.c
 * Date/Time functions.
 * @author Conlan Wesson
 */

#include "date.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "dev/rtc.h"

//! Array of full month names.
const char *date_months[] = {
	"January", "February", "March", "April", "May", "June",
	"July", "August", "September", "October", "November", "December"
};

//! Array of short month names.
const char *date_months_short[] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

//! Array of full day of the week names.
const char *date_week_days[] = {
	"Sunday", "Monday", "Tuesday", "Wednesday",
	"Thursday", "Friday", "Saturday"
};

//! Array of short day of the week names.
const char *date_week_days_short[] = {
	"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
};

/**
 * Prints the current date and time.
 */
void date_print(){
	struct datetime dt;
	rtc_time(&dt);
	printf("%s %04d-%02d-%02d %02d:%02d:%02d\n", date_week_days_short[date_day_of_week(dt.year, dt.month, dt.day)],
			dt.year, dt.month, dt.day, dt.hour, dt.min, dt.sec);
}

/**
 * Determines if the specified year is a leap year.
 * @param year The year to check.
 * @return true if the year is a leap year, false otherwise.
 */
bool date_leap_year(uint16_t year){
	if(year%400 == 0){
		return true;
	}else if(year%100 == 0){
		return false;
	}else if(year%4 == 0){
		return true;
	}else{
		return false;
	}
}

/**
 * Calculates the day of the week.
 * @param year The year of the date to calculate.
 * @param month The month of the date to calculate (1-12).
 * @param day The day of the month of the date to calculate (1-31).
 * @return The day of the week, 0 => Sunday, 6 => Saturday.
 */
uint8_t date_day_of_week(uint16_t year, uint8_t month, uint8_t day){
	int a = (14-month)/12;
	int y = year-a;
	int m = month + 12*a - 2;
	return (day + y + y/4 - y/100 + y/400 + (31*m/12)) % 7;
}

