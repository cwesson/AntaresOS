/**
 * @file include/assert.h
 * Implementation of the C assert.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_ASSERT_H_
#define __INCLUDE_ASSERT_H_

#include <kernel/panic.h>

/**
 * Checks that EXP is true, and causes a kernel panic if not.
 * @param EXP The expression to check.
 * @return Zero if EXP is true, otherwise does not return.
 */
#define assert(EXP) ((EXP) ? (void)0 : panic("Assertion '" #EXP "' failed."))

#endif /* __INCLUDE_ASSERT_H_ */
