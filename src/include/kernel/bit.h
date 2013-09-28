/**
 * @file include/kernel/bit.h
 * Kernel operations for manipulating bit fields.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_KERNEL_BIT_H_
#define __INCLUDE_KERNEL_BIT_H_

/**
 * Computes the value of bit X.
 * @param X The bit to compute the value of.
 * @return The value of the bit (1, 2, 4, 8, 16, ...).
 */
#define bit(X) (1<<(X))

/**
 * Sets bit Y of X to 1.
 * @param X The value to set the bit in.
 * @param Y The bit to set (0 least significant).
 * @return The resulting value.
 */
#define bitset(X, Y) ((X) | (1<<(Y)))

/**
 * Sets bit Y of X to 0.
 * @param X The value to clear the bit in.
 * @param Y The bit to clear (0 least significant).
 * @return The resulting value.
 */
#define bitclear(X, Y) ((X) & ~(1<<(Y)))

/**
 * Toggles bit Y of X.
 * @param X The value to toggle a bit in.
 * @param Y The bit to toggle (0 least significant).
 * @return The resulting value.
 */
#define bittoggle(X, Y) ((X) ^ (1<<(Y)))

/**
 * Sets bits Y of X to 1.
 * @param X The value to set a bit in.
 * @param Y The bit mask to set.
 * @return The resulting value.
 */
#define bitsset(X, Y) ((X) | (Y))

/**
 * Sets bits Y of X to 0.
 * @param X The value to clear a bit in.
 * @param Y The bit mask to clear.
 * @return The resulting value.
 */
#define bitsclear(X, Y) ((X) & (~(Y)))

/**
 * Toggles bits Y of X.
 * @param X The value to clear a bit in.
 * @param Y The bit mask to toggle.
 * @return The resulting value.
 */
#define bitstoggle(X, Y) ((X) ^ (Y))

/**
 * Reads bit Y of X.
 * @param X The value to read the bit from.
 * @param Y The bit to read (0 least significant).
 * @return The value of the bit (0 or 1).
 */
#define bitread(X, Y) (((X) >> (Y)) & 1)

/**
 * Sets bit Y of X to Z.
 * @param X The value to write the bit to.
 * @param Y The bit to write (0 least significant).
 * @param Z The value to write (0 or 1).
 * @return The resulting value.
 */
#define bitwrite(X, Y, Z) ((Z) ? (bitset(X, Y)) : (bitclear(X, Y)))

#endif /* __INCLUDE_KERNEL_BIT_H_ */

