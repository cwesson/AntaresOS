/**
 * @file include/sys/types.h
 * Implementation of the C data types.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_SYS_TYPES_H_
#define __INCLUDE_SYS_TYPES_H_

//! Used for file block counts.
typedef signed long long blkcnt_t;

//! Used for block sizes.
typedef signed int blksize_t;

//! Used for system times in clock ticks.
typedef unsigned long long clock_t;

//! Used for clock ID type in the clock and timer functions.
typedef unsigned int clockid_t;

//! Used for device IDs.
typedef unsigned int dev_t;

//! Used for file system block counts.
typedef unsigned long long fsblkcnt_t;

//! Used for file system file counts.
typedef unsigned int fsfilcnt_t;

//! Used for group IDs.
typedef unsigned int gid_t;

//! Used as a general indentifier.
typedef unsigned long long id_t;

//! Used for file serial numbers.
typedef unsigned long long ino_t;

//! Used for XSI interprocess communication.
typedef unsigned long long key_t;

//! Used for some file attributes.
typedef unsigned int mode_t;

//! Used for file sizes.
typedef signed long long off_t;

//! Used for process IDs and process group IDs.
typedef signed int pid_t;

#ifndef size_t
	//! Used for sizes of objects.
	typedef unsigned int size_t;
#endif

//! Used for a count of bytes or an error indication.
typedef signed int ssize_t;

//! Used for time in microseconds.
typedef signed long long suseconds_t;

//! Used for time in seconds.
typedef unsigned long long time_t;

//! Used for timer ID returned by timer_create().
typedef unsigned int timer_t;

//! Used for user IDs.
typedef unsigned long long uid_t;

//! Used for time in microseconds.
typedef unsigned long long useconds_t;

#endif /* __INCLUDE_SYS_TYPES_H_ */

