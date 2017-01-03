/**
 * @file include/stdlib.h
 * Standard library functions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDLIB_H_
#define __INCLUDE_STDLIB_H_

#include <errno.h>
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

/**
 * Constraint handler callback function.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
typedef void (*constraint_handler_t)(const char *msg, void *ptr, errno_t error);

/**
 * Set the constraint handler function.
 * @param handler New constraint callback function.
 * @return Old constraint callback function.
 */
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler);

/**
 * Aborts execution of the program.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void abort_handler_s(const char *msg, void *ptr, errno_t error);

/**
 * Return without performing any action.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void ignore_handler_s(const char *msg, void *ptr, errno_t error);

#endif /* __INCLUDE_STDLIB_H_ */

