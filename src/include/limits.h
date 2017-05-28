/**
 * @file include/limit.h
 * Standard limits definitions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_LIMITS_H_
#define __INCLUDE_LIMITS_H_

// Minimum Values
enum {
	_POSIX_AIO_LISTIO_MAX = 2,
	_POSIX_AIO_MAX = 1,
	_POSIX_ARG_MAX = 4096,
	_POSIX_CHILD_MAX = 25,
	_POSIX_DELAYTIMER_MAX = 32,
	_POSIX_HOST_NAME_MAX = 255,
	_POSIX_LINK_MAX = 8,
	_POSIX_LOGIN_NAME_MAX = 9,
	_POSIX_MAX_CANON = 255,
	_POSIX_MAX_INPUT = 255,
	_POSIX_MQ_OPEN_MAX = 8,
	_POSIX_MQ_PRIO_MAX = 32,
	_POSIX_NAME_MAX = 14,
	_POSIX_NGROUPS_MAX = 8,
	_POSIX_OPEN_MAX = 20,
	_POSIX_PATH_MAX = 255,
	_POSIX_PIPE_BUF = 512,
	_POSIX_RE_DUP_MAX = 255,
	_POSIX_RTSIG_MAX = 8,
	_POSIX_SEM_NSEMS_MAX = 256,
	_POSIX_SEM_VALUE_MAX = 32767,
	_POSIX_SIGQUEUE_MAX = 32,
	_POSIX_SSIZE_MAX = 32767,
	_POSIX_SS_REPL_MAX = 4,
	_POSIX_STREAM_MAX = 8,
	_POSIX_SYMLINK_MAX = 255,
	_POSIX_SYMLOOP_MAX = 8,
	_POSIX_THREAD_DESTRUCTOR_ITERATIONS = 4,
	_POSIX_THREAD_KEYS_MAX = 128,
	_POSIX_THREAD_THREADS_MAX = 64,
	_POSIX_TIMER_MAX = 32,
	_POSIX_TRACE_EVENT_NAME_MAX = 30,
	_POSIX_TRACE_NAME_MAX = 8,
	_POSIX_TRACE_SYS_MAX = 8,
	_POSIX_TRACE_USER_EVENT_MAX = 32,
	_POSIX_TTY_NAME_MAX = 9,
	_POSIX_TZNAME_MAX = 6,
	_POSIX2_BC_BASE_MAX = 99,
	_POSIX2_BC_DIM_MAX = 2048,
	_POSIX2_BC_SCALE_MAX = 99,
	_POSIX2_BC_STRING_MAX = 1000,
	_POSIX2_CHARCLASS_NAME_MAX = 14,
	_POSIX2_COLL_WEIGHTS_MAX = 2,
	_POSIX2_EXPR_NEST_MAX = 32,
	_POSIX2_LINE_MAX = 2048,
	_POSIX2_RE_DUP_MAX = 255,
	_XOPEN_IOV_MAX = 16,
	_XOPEN_NAME_MAX = 255,
	_XOPEN_PATH_MAX = 1024,
};

//! Runtime Invariant Values
enum {
	AIO_LISTIO_MAX = _POSIX_AIO_LISTIO_MAX,
	AIO_MAX = _POSIX_AIO_MAX,
	AIO_PRIO_DELTA_MAX = 0,
	ARG_MAX = _POSIX_ARG_MAX,
	ATEXIT_MAX = 32,
	CHILD_MAX = _POSIX_CHILD_MAX,
	DELAYTIMER_MAX = _POSIX_DELAYTIMER_MAX,
	HOST_NAME_MAX = _POSIX_HOST_NAME_MAX,
	IOV_MAX = _XOPEN_IOV_MAX,
	LOGIN_NAME_MAX = _POSIX_LOGIN_NAME_MAX,
	MQ_OPEN_MAX = _POSIX_MQ_OPEN_MAX,
	MQ_PRIO_MAX = _POSIX_MQ_PRIO_MAX,
	OPEN_MAX = _POSIX_OPEN_MAX,
	PAGESIZE = 4096,
	PAGE_SIZE = PAGESIZE,
	PTHREAD_DESTRUCTOR_ITERATIONS = _POSIX_THREAD_DESTRUCTOR_ITERATIONS,
	PTHREAD_KEYS_MAX = _POSIX_THREAD_KEYS_MAX,
	PTHREAD_STACK_MIN = 0,
	PTHREAD_THREADS_MAX = _POSIX_THREAD_THREADS_MAX,
	RTSIG_MAX = _POSIX_RTSIG_MAX,
	SEM_NSEMS_MAX = _POSIX_SEM_NSEMS_MAX,
	SEM_VALUE_MAX = _POSIX_SEM_VALUE_MAX,
	SIGQUEUE_MAX = _POSIX_SIGQUEUE_MAX,
	SS_REPL_MAX = _POSIX_SS_REPL_MAX,
	STREAM_MAX = _POSIX_STREAM_MAX,
	SYMLOOP_MAX = _POSIX_SYMLOOP_MAX,
	TIMER_MAX = _POSIX_TIMER_MAX,
	TRACE_EVENT_NAME_MAX = _POSIX_TRACE_EVENT_NAME_MAX,
	TRACE_NAME_MAX = _POSIX_TRACE_NAME_MAX,
	TRACE_SYS_MAX = _POSIX_TRACE_SYS_MAX,
	TRACE_USER_EVENT_MAX = _POSIX_TRACE_USER_EVENT_MAX,
	TTY_NAME_MAX = _POSIX_TTY_NAME_MAX,
	TZNAME_MAX = _POSIX_TZNAME_MAX
};

//! Pathname Variable Values
enum {
	FILESIZEBITS = 32,
	LINK_MAX = _POSIX_LINK_MAX,
	MAX_CANON = _POSIX_MAX_CANON,
	MAX_INPUT = _POSIX_MAX_INPUT,
	NAME_MAX = _XOPEN_NAME_MAX,
	PATH_MAX = _XOPEN_PATH_MAX,
	PIPE_BUF = _POSIX_PIPE_BUF,
	SYMLINK_MAX = _POSIX_SYMLINK_MAX,
};

//! Numerical Limits
enum {
	CHAR_BIT = sizeof(char)*8,
	SHRT_BIT = sizeof(short)*8,
	LONG_BIT = sizeof(long)*8,
	WORD_BIT = sizeof(int)*8,
	LLONG_BIT = sizeof(long long)*8,
};

#endif
