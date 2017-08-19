/**
 * @file constraint.c
 * Internal constraint functions.
 * @author Conlan Wesson
 */

#include "constraint.h"
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * Default constraint handler callback function.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void default_handler_s(const char *msg, void *ptr, errno_t error){
	constraint_ptr_t *details = (constraint_ptr_t*)ptr;
	printf("\n\e[31m%s: %s:%u: %s: Error %d\e[0m\n", msg, details->file, details->line, details->func, error);
}

//! Constraint handler callback.
static constraint_handler_t constraint_handler = default_handler_s;

/**
 * Set the constraint handler function.
 * @param handler New constraint callback function.
 * @return Old constraint callback function.
 */
constraint_handler_t __set_constraint_handler_s_actual(constraint_handler_t handler){
	constraint_handler_t ret = constraint_handler;
	if(handler == NULL){
		constraint_handler = default_handler_s;
	}else{
		constraint_handler = handler;
	}
	return ret;
}

/**
 * Constraint handler callback function.
 * @param msg Description of the error.
 * @param file File where the constraint occured.
 * @param line Line number where the constraint occured.
 * @param func Funtion where the constraint occured.
 * @param error error number that occured.
 */
void __constraint(const char *msg, const char *file, unsigned int line, const char *func, errno_t error){
	constraint_ptr_t ptr = {
		.file = file,
		.func = func,
		.line = line
	};
	
	if(constraint_handler){
		constraint_handler(msg, &ptr, error);
	}
}

