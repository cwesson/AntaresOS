/**
 * @file include/stdarg.h
 * Implementation if the stdarg specification.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDARG_H_
#define __INCLUDE_STDARG_H_

//! Type for iterating over arguments.
typedef __builtin_va_list va_list;

/**
 * Start iterating over arguments.
 * @param ap The va_list to start.
 * @param parmN The last define argument.
 */
#define va_start(ap, parmN) __builtin_va_start(ap, parmN)

/**
 * Fetch the next argument.
 * @param ap The va_list to fetch an argument from.
 * @param type The type of the argument.
 * @return The argument fetched.
 */
#define va_arg(ap, type) __builtin_va_arg(ap, type)

/**
 * Stop iterating over arguments.
 * @param ap The va_list to stop.
 */
#define va_end(ap) __builtin_va_end(ap)

/**
 * Copy one va_list to another.
 * @param dest The destination of the copy.
 * @param src The source of the copy.
 */
#define va_copy(dest, src) __builtin_va_copy(dest, src)

#endif /* __INCLUDE_STDARG_H_ */
