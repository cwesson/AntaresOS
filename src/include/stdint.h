/**
 * @file include/stdint.h
 * Implementation of the C stdint specification.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDINT_H_
#define __INCLUDE_STDINT_H_

typedef signed char int8_t;
#define INT8_MAX 127
#define INT8_MIN -128
_Static_assert(sizeof(int8_t)==1, "Size of int8_t is not 1.");

typedef unsigned char uint8_t;
#define UINT8_MAX 255u
#define UINT8_MIN 0
_Static_assert(sizeof(uint8_t)==1, "Size of uint8_t is not 1.");

typedef signed short int16_t;
#define INT16_MAX 32767
#define INT16_MIN -32768
_Static_assert(sizeof(int16_t)==2, "Size of int16_t is not 2.");

typedef unsigned short uint16_t;
#define UINT16_MAX 65536u
#define UINT16_MIN 0
_Static_assert(sizeof(uint16_t)==2, "Size of uint16_t is not 2.");

typedef signed int int32_t;
#define INT32_MAX 2147483647
_Static_assert(sizeof(int32_t)==4, "Size of int32_t is not 4.");
#define INT32_MIN -2147483648

typedef unsigned int uint32_t;
#define UINT32_MAX 4294967295u
#define UINT32_MIN 0
_Static_assert(sizeof(uint32_t)==4, "Size of uint32_t is not 4.");

typedef signed long long int64_t;
#define INT64_MAX 9223372036854775807ll
#define INT64_MIN -9223372036854775808ll
_Static_assert(sizeof(int64_t)==8, "Size of int64_t is not 8.");

typedef unsigned long long uint64_t;
#define UINT64_MAX 18446744073709551615ull
#define UINT63_MIN 0
_Static_assert(sizeof(uint64_t)==8, "Size of uint64_t is not 8.");

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#endif /* __INCLUDE_STDINT_H_ */

