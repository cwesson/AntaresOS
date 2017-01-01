/**
 * @file lib/std/string.c
 * Standard library string functions.
 * @author Conlan Wesson
 */

#include "string.h"

#include <stddef.h>

/**
 * Copies the first n bytes of src to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
inline void *memcpy(void *restrict dest, const void *restrict src, size_t n){
	return memmove(dest, src, n);
}

/**
 * Copies the first n bytes of src to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
void *memmove(void *dest, const void *src, size_t n){
	unsigned char *pdest = dest;
	const unsigned char *psrc = src;
	if(pdest < psrc){
		while(n){
			*pdest = *psrc;
			++pdest;
			++psrc;
			--n;
		}
	}else if(pdest > psrc){
		pdest = pdest + n - 1;
		psrc = psrc + n - 1;
		while(n){
			*pdest = *psrc;
			--pdest;
			--psrc;
			--n;
		}
	}
	return dest;
}

/**
 * Copies the first n bytes of src to dest or until c is encountered.
 * @param dest The destination address.
 * @param src The source address.
 * @param c Character to search for.
 * @param n The Number of bytes to copy.
 * @return A pointer to dest.
 */
void *memccpy(void *restrict dest, const void *restrict src, char c, size_t n){
	unsigned char *pdest = dest;
	const unsigned char *psrc = src;
	char last = c + 1;
	if(pdest < psrc){
		while(n && last != c){
			*pdest = *psrc;
			last = *pdest;
			++pdest;
			++psrc;
			--n;
		}
	}else if(pdest > psrc){
		pdest = pdest + n - 1;
		psrc = psrc + n - 1;
		while(n && last != c){
			*pdest = *psrc;
			last = *pdest;
			--pdest;
			--psrc;
			--n;
		}
	}else{
		while(n && last != c){
			last = *pdest;
			++pdest;
			--n;
		}
	}
	if(last == c){
		return (void*)pdest;
	}else{
		return NULL;
	}
}

/**
 * Overwrites dest with n copies of c.
 * @param dest The destination address.
 * @param c The value to write.
 * @param n The number of bytes to write.
 * @return A pointer to dest.
 */
void *memset(void *dest, char c, size_t n){
	unsigned char *pdest = dest;
	while(n){
		*pdest = c;
		++pdest;
		--n;
	}
	return dest;
}

/**
 * Finds the first instance of c in the first n bytes of src.
 * @param src The source address.
 * @param c The value to find.
 * @param n The number of bytes to check.
 * @return A pointer to the location, or NULL if not found.
 */
void *memchr(const void *src, char c, size_t n){
	const unsigned char *psrc = src;
	while(n){
		if(*psrc == c){
			return (void *)psrc;
		}
		++psrc;
		--n;
	}
	return NULL;
}

/**
 * Compares the contents of two memory address.
 * @param s1 The first address.
 * @param s2 The second address.
 * @param n The number of bytes to compare.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int memcmp(const void *s1, const void *s2, size_t n){
	const unsigned char *p1 = s1;
	const unsigned char *p2 = s2;
	while(n){
		if(*p1 != *p2){
			return *p1 - *p2;
		}
		++p1;
		++p2;
		--n;
	}
	return 0;
}

/**
 * Appends the string src to dest.
 * @param dest The destination string.
 * @param src The source string.
 * @return The destination string.
 */
char *strcat(char *restrict dest, const char *restrict src){
	unsigned int d = 0;
	unsigned int s = 0;
	while(dest[d]){
		++d;
	}
	while(src[s]){
		dest[d+s] = src[s];
		++s;
	}
	dest[d+s] = '\0';
	return dest;
}

/**
 * Compares two strings lexicographically.
 * @param s1 The first string.
 * @param s2 The second string.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int strcmp(const char *s1, const char *s2){
	while(*s1){
		if(*s1 != *s2){
			return *s1 - *s2;
		}
		++s1;
		++s2;
	}
	return 0;
}

/**
 * Compares two strings lexicographically.
 * @param s1 The first string.
 * @param s2 The second string.
 * @param max The maximum length to compare.
 * @return Negative if s1 < s2, 0 if s1 == s2, positive if s1 > s2.
 */
int strncmp(const char *s1, const char *s2, size_t max){
	while((*s1 || *s2) && max){
		if(*s1 != *s2){
			return *s1 - *s2;
		}
		++s1;
		++s2;
		--max;
	}
	return 0;
}

/**
 * Copies the first string to dest.
 * @param dest The destination address.
 * @param src The source address.
 * @return A pointer to dest.
 */
char *strcpy(char *restrict dest, const char *restrict src){
	unsigned char *pdest = (unsigned char *)dest;
	unsigned char *psrc = (unsigned char *)src;
	if(pdest < psrc){
		while(*psrc){
			*pdest = *psrc;
			++pdest;
			++psrc;
		}
	}else if(pdest > psrc){
		size_t n = strlen(src);
		pdest = pdest + n - 1;
		psrc = psrc + n - 1;
		while(*psrc){
			*pdest = *psrc;
			--pdest;
			--psrc;
		}
	}
	return dest;
}

/**
 * Finds the length of a string.
 * @param str The string to check.
 * @return The length of the string.
 */
size_t strlen(const char *str){
	size_t len = 0;
	while(str[len]){
		++len;
	}
	return len;
}

