/**
 * @file include/stdio.h
 * Standard I/O functions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STDIO_H_
#define __INCLUDE_STDIO_H_

#include <stdint.h>
#include "hal/device.h"

enum {
	EOF = -1,    //!< End-of-File indicator.
};

#ifndef NULL
	#define NULL ((void*)0)    //!< Null pointer
#endif

device_descriptor *stdout;    //!< Pointer to standard output stream.
device_descriptor *stderr;    //!< Pointer to standard error stream.
device_descriptor *stdin;     //!< Pointer to standard input stream.

/**
 * Enum to determine how putnum displays the sign.
 */
enum putnum_sign {
	PUTNUM_SIGN_NONE,      //!< Never show a sign.
	PUTNUM_SIGN_NLEAD,     //!< Show only negative sign before padding.
	PUTNUM_SIGN_NEMBED,    //!< Show only negative sign after padding.
	PUTNUM_SIGN_PLEAD,     //!< Show both negative and positive sign before padding.
	PUTNUM_SIGN_PEMBED     //!< Show both negative and positive sign after padding.
};

/**
 * Writes a string to stdout.
 * @param str String to write.
 * @return Non-negative if successful, EOF otherwise.
 */
int puts(const char *);

/**
 * Prints a string to stdout using the specified format.
 * @param format Format string.
 * @param ... The values used in the format.
 * @return Number of characters written if successful, EOF otherwise.
 */
int printf(const char *, ...);

/**
 * Writes a string to stderr.
 * @param str String to write.
 */
void perror(const char *);

/**
 * Writes a character to stdout.
 * @param ch Character to write.
 * @return ch if successful, EOF otherwise.
 */
char putchar(char);

/**
 * Writes an integet to stdout.
 * @param num The integer to write.
 * @param base The base to use for the output.
 * @param group_size The number of digits per group.
 * @param digit_sep The digit seperator character.
 * @param group_sep The group seperator character.
 * @param length The minimum length of the output string.
 * @param pad The padding character.
 * @param neg How to handle signs.
 * @param cap true to use capital letters, false otherwise.
 * @return Number of characters written.
 */
unsigned int putnum(int, unsigned int, unsigned int, char, char, unsigned int, char, enum putnum_sign, bool);

/**
 * Writes a binary integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int putbin(unsigned int);

/**
 * Writes an octal integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int putoct(unsigned int);

/**
 * Outputs a decimal number to the console.
 * @param num The number to output.
 * @return Number of characters written.
 */
int putint(int);

/**
 * Writes a hexadecimal integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int puthex(unsigned int);

/**
 * Writes a floating point number to stdout.
 * @param num The number to write.
 * @param prec The number of decimal places to use.
 * @return Number of characters written.
 */
int putfloat(double, uint8_t);

/**
 * Gets a single character from input.
 * @return The character received.
 */
unsigned char getchar();

/**
 * Reads a string from stdin.
 * @param str Buffer to store the string to.
 * @return str if successful, NULL otherwise.
 */
char *gets(char *);

#endif

