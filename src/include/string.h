/**
 * @file include/string.h
 * Standard library string functions.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_STRING_H_
#define __INCLUDE_STRING_H_

#include <stddef.h>

#ifndef NULL
	#define NULL ((void*)0)    //!< Null pointer
#endif

/**
 * Copies the first n bytes of src to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
void *memcpy(void *restrict dest, const void *restrict src, size_t n);

/**
 * Copies the first n bytes of src to dest or until c is encountered.
 * @param dest The destination address.
 * @param src The source address.
 * @param c Character to search for.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
void *memccpy(void *restrict dest, const void *restrict src, char c, size_t n);

/**
 * Copies the first n bytes of src to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
void *memmove(void *dest, const void *src, size_t n);

/**
 * Overwrites dest with n copies of c.
 * @param dest The destination address.
 * @param c The value to write.
 * @param n The number of bytes to write.
 * @return A pointer to dest.
 */
void *memset(void *dest, char c, size_t n);

/**
 * Finds the first instance of c in the first n bytes of src.
 * @param src The source address.
 * @param c The value to find.
 * @param n The number of bytes to check.
 * @return A pointer to the location, or NULL if not found.
 */
void *memchr(const void *src, char c, size_t n);

/**
 * Compares the contents of two memory address.
 * @param s1 The first address.
 * @param s2 The second address.
 * @param n The number of bytes to compare.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int memcmp(const void *s1, const void *s2, size_t n);

/**
 * Appends the string src to dest.
 * @param dest The destination string.
 * @param src The source string.
 * @return The destination string.
 */
char *strcat(char *restrict dest, const char *restrict src);

/**
 * Compares two strings lexicographically.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int strcmp(const char *s1, const char *s2);

/**
 * Copies the first string to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @return A pointer to dest.
 */
char *strcpy(char *restrict dest, const char *restrict src);

/**
 * Compares two strings lexicographically.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param max The maximum length to compare.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int strncmp(const char *s1, const char *s2, size_t max);

/**
 * Finds the length of a string.
 * @param str The string to check.
 * @return The length of the string.
 */
size_t strlen(const char *str);

#endif
