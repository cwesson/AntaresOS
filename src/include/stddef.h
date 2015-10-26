/**
 * @file include/stddef.h
 * Implementation of the C stddef specification.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDDEF_H_
#define __INCLUDE_STDDEF_H_

#ifndef NULL
	#define NULL ((void*)0)    //!< Null pointer
#endif

/**
 * Calculates the offset in bytes of a structure member, from the beginning of the structure.
 * @param type The type of the structure.
 * @param member The name of the structure member.
 * @return The offset in bytes of the member.
 */
#define offsetof(type, member)  __builtin_offsetof(type, member)

//! Signed integer type of the result of subtracting two pointers.
typedef signed int ptrdiff_t;

//! Integer type whose range of values can represent all members of the character set.
typedef unsigned char wchar_t;

#ifndef size_t
	//! Unsigned integer type of the result of the sizeof operator.
	typedef unsigned int size_t;
#endif

#endif /* __INCLUDE_STDDEF_H_ */

