/**
 * @file include/assert.h
 * Implementation of the C time types.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_TIME_H_
#define __INCLUDE_TIME_H_

#include <sys/types.h>

typedef struct tm {
	int tm_sec;      //!< Seconds [0,60].
	int tm_min;      //!< Minutes [0,59].
	int tm_hour;     //!< Hour [0,23].
	int tm_mday;     //!< Day of month [1,31].
	int tm_mon;      //!< Month of year [0,11].
	int tm_year;     //!< Years since 1900.
	int tm_wday;     //!< Day of week [0,6] (Sunday=0).
	int tm_yday;     //!< Day of the year [0,365].
	int tm_isdst;    //!< Daylight savings flag.
} tm;

#ifndef NULL
	//! Null pointer constant.
	#define NULL ((void*)NULL)
#endif

//! A number used to convert the value returned by clock() into seconds.
#define CLOCKS_PER_SEC (1000000)

//! The identifier of the CPU-time clock associated with the process.
#define CLOCK_PROCESS_CPUTIME_ID

//! The identifier of the CPU-time clock associated with the thread.
#define CLOCK_THREAD_CPUTIME_ID

typedef struct timespec {
	time_t tv_sec;     //!< Seconds
	long   tv_nsec;    //!< Nanoseconds
} timespec;

typedef struct itimerspec {
	struct timespec it_interval;    //!< Timer period.
	struct timespec it_value;       //!< Timer expiration.
} itimerspec;

enum {
	CLOCK_REALTIME,  //!< The identifier of the system-wide realtime clock.
	CLOCK_MONOTONIC, //!< The identifier of the system-wide monotonic clock.
};

//! Flag indicating time is absolute.
#define TIMER_ABSTIME (0)

#endif /* __INCLUDE_TIME_H_ */

