/**
 * @file include/ctype.h
 * Implementation if the ctype specification.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_CTYPE_H_
#define __INCLUDE_CTYPE_H_

/**
 * Bit map of ASCII characters to corresponding character classifications.
 * Bits mapped as:  CULD XPSB
 * C = Control
 * U = Uppercase
 * L = Lowercase
 * D = Digit
 * X = Hex Digit
 * P = Punctuation
 * S = Space
 * B = Blank
 */
extern const unsigned char __CTYPE_MAP_ASCII_[];

/**
 * Tests if a character is a lowercase letter.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define islower(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x20)

/**
 * Tests if a character is an uppercase letter.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isupper(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x40)

/**
 * Tests if a character is a letter.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isalpha(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x60)

/**
 * Tests if a character is a number.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isdigit(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x10)

/**
 * Tests if a character is a letter or number.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isalnum(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x70)

/**
 * Tests if a character is a blank character.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isblank(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x01)

/**
 * Tests if a character is a control character.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define iscntrl(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x80)

/**
 * Tests if a character is whitespace.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isspace(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x02)

/**
 * Tests if a character is printable.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isprint(ch) (!(__CTYPE_MAP_ASCII_[(int)(ch)] & 0x80))

/**
 * Tests if a character is printable and not whitespace.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isgraph(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x74)

/**
 * Tests if a character is printable and not alphanumeric.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define ispunct(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x04)

/**
 * Tests if a character is a hexidecimal digit.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isxdigit(ch) (__CTYPE_MAP_ASCII_[(int)(ch)] & 0x08)

/**
 * Tests if a character is a 7bit ASCII character.
 * @param ch The character to test.
 * @return true if it is, false otherwise.
 */
#define isascii(ch) ((unsigned int)(ch) <= 127)

/**
 * Converts a alphabetic character to lowercase.
 * @param ch The character to convert.
 * @return The lowercase equivalent, or ch if the character is not alphabetic.
 */
int tolower(int ch);

//! @see int tolower(int)
#define _tolower(ch) tolower(ch)

/**
 * Converts a alphabetic character to uppercase.
 * @param ch The character to convert.
 * @return The uppercase equivalent, or ch if the character is not alphabetic.
 */
int toupper(int ch);

//! @see int toupper(int)
#define _toupper(ch) toupper(ch)

/**
 * Convert its argument into a 7bit ASCII character.
 * @param ch The character to convert.
 * @return A 7bit ASCII character.
 */
#define toascii(ch) (((int)ch) & 0x7F)


#endif /* __INCLUDE_CTYPE_H_ */

