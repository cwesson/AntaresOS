/**
 * @file include/constraint.c
 * Internal constraint functions.
 * @author Conlan Wesson
 */

#include <errno.h>
#include <stdlib.h>

/**
 * Default constraint handler callback function.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void default_handler_s(const char *msg, void *ptr, errno_t error){
	(void)msg;
	(void)ptr;
	(void)error;
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
 * @param error error number that occured.
 */
void __constraint(char *msg, errno_t error){
	if(constraint_handler){
		constraint_handler(msg, NULL, error);
	}
}

