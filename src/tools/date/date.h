/**
 * @file tools/date/date.h
 * Date/Time functions.
 * @author Conlan Wesson
 */

#ifndef TOOLS_DATE_DATE_H
#define TOOLS_DATE_DATE_H

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

/**
 * Calculates the day of the week.
 * @param year The year of the date to calculate.
 * @param month The month of the date to calculate (1-12).
 * @param day The day of the month of the date to calculate (1-31).
 * @return The day of the week, 0 => Sunday, 6 => Saturday.
 */
uint8_t date_day_of_week(uint16_t year, uint8_t month, uint8_t day);

#endif
