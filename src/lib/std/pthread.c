/**
 * @file lib/std/pthread.c
 * Implementation if the pthread specification.
 * @author Conlan Wesson
 */

#include <pthread.h>

#include <errno.h>
#include <stddef.h>

int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t* attr){
	mutex->lock = 0;
	mutex->count = 1;
	if(attr != NULL){
		mutex->attr.prio = attr->prio;
		mutex->attr.protocol = attr->protocol;
		mutex->attr.pshared = attr->pshared;
		mutex->attr.type = attr->type;
	}else{
		mutex->attr.prio = PTHREAD_PRIO_NONE;
		mutex->attr.protocol = 0;
		mutex->attr.pshared = PTHREAD_PROCESS_PRIVATE;
		mutex->attr.type = PTHREAD_MUTEX_DEFAULT;
	}
	return EOK;
}

static int test_and_set(volatile int* lock){
	int bit = 1;
	asm volatile(
		"lock xchg %[lock], %[bit]"
		: [lock] "+m" (*lock), [bit] "+r" (bit)
	);
	return !bit;
}

int pthread_mutex_trylock(pthread_mutex_t* mutex){
	errno_t ret = EBUSY;
	if(test_and_set(&mutex->lock)){
		--mutex->count;
		if(mutex->count >= 0){
			ret = EOK;
		}else{
			++mutex->count;
		}
		mutex->lock = 0;
	}
	return ret;
}

int pthread_mutex_lock(pthread_mutex_t* mutex){
	while(pthread_mutex_trylock(mutex) == EBUSY){
	}
	return EOK;
}

int pthread_mutex_unlock(pthread_mutex_t* mutex){
	for(;;){
		if(test_and_set(&mutex->lock)){
			++mutex->count;
			mutex->lock = 0;
			break;
		}
	}
	return EOK;
}

