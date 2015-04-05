/**
 * @file include/math.h
 * Implementation of standard math library.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_MATH_H_
#define __INCLUDE_MATH_H_

#define M_E        ((double)2.71828182845905)    //!< The base of natural logarithms.
#define M_LOG2E    ((double)1.44269504088896)    //!< The logarithm base 2 of M_E.
#define M_LOG10E   ((double)0.43429448190325)    //!< The logarithm base 10 of M_E.
#define M_LN2      ((double)0.69314718055995)    //!< The natural logarithm of 2.
#define M_LN10     ((double)2.30258509299405)    //!< The natural logarithm of 10.
#define M_PI       ((double)3.14159265358979)    //!< Pi, the ratio of a circle's circumference to its diameter.
#define M_PI_2     (M_PI/2.0)                    //!< Pi divided by two.
#define M_PI_4     (M_PI/4.0)                    //!< Pi divided by four.
#define M_1_PI     (1.0/M_PI)                    //!< The reciprocal of pi (1/pi).
#define M_2_PI     (2.0*M_1_PI)                  //!< Two times the reciprocal of pi.
#define M_2_SQRTPI ((double)1.12837916709551)    //!< Two times the reciprocal of the square root of pi.
#define M_SQRT2    ((double)1.41421356237310)    //!< The square root of two.
#define M_SQRT1_2  ((double)0.70710678118655)    //!< The reciprocal of the square root of two (also the square root of 1/2).

/**
 * Absolute value.
 * @param x The number to get the absolute value of.
 * @return The absolute value of x.
 */
inline double fabs(double x){
	return (x >= 0) ? x : (-x);
}

#endif

