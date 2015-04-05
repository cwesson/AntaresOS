/**
 * @file include/float.h
 * Floating-point values.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_FLOAT_H_
#define __INCLUDE_FLOAT_H_

typedef float float32;
#define FLOAT32_PINIFINITY ((float32)0x7F800000)
#define FLOAT32_MINIFINITY ((float32)0xFF800000)

typedef double float64;
#define FLOAT64_PINIFINITY ((float64)0x7FF0000000000000)
#define FLOAT64_MINIFINITY ((float64)0xFFF0000000000000)

#endif /* __INCLUDE_FLOAT_H_ */

