/**
 * @file lib/std/constraint.h
 * Internal constraint functions.
 * @author Conlan Wesson
 */

#ifndef __LIB_STD_CONSTRAINT_H_
#define __LIB_STD_CONSTRAINT_H_

#include <errno.h>
#include <stdlib.h>

/**
 * Tests a runtime constraint.
 * @param COND Condition of the constraint.
 * @param ERR Error to occur if the constraint fails.
 * @param RET Return value if the constraint fails.
 */
#define constraint(COND, ERR, RET) if(!(COND)){ __constraint(("Constraint '" #COND "' failed."), __FILE__, __LINE__, __func__, (ERR)); return (RET); }

/**
 * Set the constraint handler function.
 * @param handler New constraint callback function.
 * @return Old constraint callback function.
 */
constraint_handler_t __set_constraint_handler_s_actual(constraint_handler_t handler);

/**
 * Constraint handler callback function.
 * @param msg Description of the error.
 * @param file File where the constraint occured.
 * @param line Line number where the constraint occured.
 * @param func Funtion where the constraint occured.
 * @param error error number that occured.
 */
void __constraint(const char *msg, const char *file, unsigned int line, const char *func, errno_t error);

#endif

