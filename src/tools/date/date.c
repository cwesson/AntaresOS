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
	struct tm dt;
	rtc_time(&dt);
	printf("%s %04d-%02d-%02d %02d:%02d:%02d\n", date_week_days_short[dt.tm_wday],
			dt.tm_year+1900, dt.tm_mon+1, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
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

