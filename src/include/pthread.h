/**
 * @file include/pthread.h
 * Implementation if the pthread specification.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_PTHREAD_H_
#define __INCLUDE_PTHREAD_H_

enum {
	PTHREAD_PRIO_NONE,
	PTHREAD_PRIO_INHERIT,
	PTHREAD_PRIO_PROTECT,
};

enum {
	PTHREAD_PROCESS_PRIVATE,
	PTHREAD_PROCESS_SHARED,
};

enum {
	PTHREAD_MUTEX_NORMAL,
	PTHREAD_MUTEX_ERRORCHECK,
	PTHREAD_MUTEX_RECURSIVE,
	PTHREAD_MUTEX_DEFAULT,
};

//! Mutex attributes
typedef struct {
	char prio;
	char protocol;
	char pshared;
	char type;
} pthread_mutexattr_t;

//! Mutex data
typedef struct {
	volatile int lock;
	volatile int count;
	pthread_mutexattr_t attr;
} pthread_mutex_t;

/**
 * Initialize pthread mutex.
 * @param mutex Mutex to initialize.
 * @param attr Attributes to assign to mutex.
 * @return EOK is successful, error code otherwise.
 */
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr);

/**
 * Try to lock pthread mutex.
 * @param mutex Mutex to lock.
 * @return EOK is successful, error code otherwise.
 */
int pthread_mutex_trylock(pthread_mutex_t* mutex);

/**
 * Lock pthread mutex.
 * @param mutex Mutex to lock.
 * @return EOK is successful, error code otherwise.
 */
int pthread_mutex_lock(pthread_mutex_t* mutex);

/**
 * Unlock pthread mutex.
 * @param mutex Mutex to unlock.
 * @return EOK is successful, error code otherwise.
 */
int pthread_mutex_unlock(pthread_mutex_t* mutex);

#endif

