/**
 * @file tools/date/date.h
 * Date/Time functions.
 * @author Conlan Wesson
 */

#ifndef __TOOLS_DATE_DATE_H_
#define __TOOLS_DATE_DATE_H_

#include <stdbool.h>
#include <stdint.h>

//! Array of full month names.
extern const char *date_months[];

//! Array of short month names.
extern const char *date_months_short[];

//! Array of full day of the week names.
extern const char *date_week_days[];

//! Array of short day of the week names.
extern const char *date_week_days_short[];

/**
 * Prints the current date and time.
 */
void date_print();

/**
 * Determines if the specified year is a leap year.
 * @param year The year to check.
 * @return true if the year is a leap year, false otherwise.
 */
bool date_leap_year(uint16_t year);

#endif

