/**
 * @file include/constraint.h
 * Internal constraint functions.
 * @author Conlan Wesson
 */

#include <errno.h>
#include <stdlib.h>

/**
 * Set the constraint handler function.
 * @param handler New constraint callback function.
 * @return Old constraint callback function.
 */
constraint_handler_t __set_constraint_handler_s_actual(constraint_handler_t handler);

void __constraint(char *msg, errno_t error);

