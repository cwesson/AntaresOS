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

typedef unsigned char uint8_t;
#define UINT8_MAX 255u
#define UINT8_MIN 0

typedef signed short int16_t;
#define INT16_MAX 32767
#define INT16_MIN -32768

typedef unsigned short uint16_t;
#define UINT16_MAX 65536u
#define UINT16_MIN 0

typedef signed int int32_t;
#define INT32_MAX 2147483647
#define INT32_MIN -2147483648

typedef unsigned int uint32_t;
#define UINT32_MAX 4294967295u
#define UINT32_MIN 0

typedef signed long long int64_t;
#define INT64_MAX 9223372036854775807ll
#define INT64_MIN -9223372036854775808ll

typedef unsigned long long uint64_t;
#define UINT64_MAX 18446744073709551615ull
#define UINT63_MIN 0

typedef int32_t intptr_t;
typedef uint32_t uintptr_t;

typedef int64_t intmax_t;
typedef uint64_t uintmax_t;

#endif /* __INCLUDE_STDINT_H_ */
