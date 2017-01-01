/**
 * @file include/stdlib.h
 * Standard library functions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDLIB_H_
#define __INCLUDE_STDLIB_H_

#include <stddef.h>

enum {
	EXIT_FAILURE = -1,    //!< Unsuccessful termination.
	EXIT_SUCCESS =  0,    //!< Successful termination.
};

/**
 * Initializes the heap.
 * @param start Null terminated array of pointers to the start of heap spaces.
 * @param end Null terminated array of pointers to the end of heap spaces.
 * @return Error code, or EOK if successful.
 */
int heap_init(void*[], void*[]);

/**
 * Allocates memory on the heap.
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated space.
 */
void *malloc(size_t);

/**
 * Deallocates space on the heap.
 * @param ptr Pointer to the space to deallocate.
 */
void free(void *ptr);

/**
 * Generates a random number.
 * @return A random number.
 */
int rand();

/**
 * Seeds the random number generator.
 * @param val The value to seed the RNG.
 */
void srand(unsigned int val);

#endif /* __INCLUDE_STDLIB_H_ */

