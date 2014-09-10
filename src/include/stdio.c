/**
 * @file include/stdio.c
 * Standard I/O functions.
 * @author Conlan Wesson
 */

#include <stdio.h>

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include "../dev/vga.h"
#include "../dev/keyboard.h"

#define MAX_LENGTH 255    //!< Maximum string length.

device_descriptor *stdout = &console_desc;
device_descriptor *stderr = &console_desc;
device_descriptor *stdin = &console_desc;

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
 * @param depth Depth of the recursive calls.
 * @param sign true if the original number was negative, false otherwise.
 * @return Number of characters written.
 */
static unsigned int __putnum_actual(int num, unsigned int base, unsigned int group_size, char digit_sep, char group_sep, unsigned int length, char pad, enum putnum_sign neg, bool cap, unsigned int depth, bool sign){
	unsigned int count = 0;
	if(neg != PUTNUM_SIGN_NONE){
		if(num < 0){
			if(neg == PUTNUM_SIGN_NLEAD || neg == PUTNUM_SIGN_PLEAD){
				putchar('-');
				++depth;
				++count;
			}
			num = -num;
			sign = true;
		}else if(depth == 0 && neg == PUTNUM_SIGN_PLEAD){
			putchar('+');
			++depth;
			++count;
		}
	}
	
	register unsigned int quo = num / base;
	register unsigned int rem = num % base;
	
	if(quo != 0){
		// Iterate to through the digits.
		count += __putnum_actual(quo, base, group_size, digit_sep, group_sep, length, pad, neg, cap, depth+1, sign);
	}else if(pad){
		if(length <= depth){
			if(sign){
				if(neg == PUTNUM_SIGN_NEMBED){
					putchar('-');
					++count;
				}
			}else{
				if(neg == PUTNUM_SIGN_PEMBED || neg == PUTNUM_SIGN_NEMBED){
					putchar('+');
					++count;
				}
			}
		}
		// Print any padding.
		for(unsigned int i = length; i > depth+1; --i){
			if(i == depth+2 && sign && (neg == PUTNUM_SIGN_NEMBED || neg == PUTNUM_SIGN_PEMBED)){
				putchar('-');
				++count;
			}else if(i == depth+2 && !sign && neg == PUTNUM_SIGN_PEMBED){
				putchar('+');
				++count;
			}else{
				putchar(pad);
				++count;
				// Print group and digit seperators in padding.
				if(group_sep && (i-1) % group_size == 0){
					putchar(group_sep);
					++count;
				}else if(digit_sep){
					putchar(digit_sep);
					++count;
				}
			}
		}
	}
	
	// Print the current digit.
	if(base < 36){
		if(rem < 10){
			putchar(rem + '0');
			++count;
		}else{
			if(cap){
				putchar(rem - 10 + 'A');
				++count;
			}else{
				putchar(rem - 10 + 'a');
				++count;
			}
		}
	}else{
		// Any base over 36 is printed in decimal.
		count += __putnum_actual(rem, 10, 0, 0, 0, 0, 0, PUTNUM_SIGN_NONE, cap, 0, sign);
	}
	
	// Print group and digit seperators.
	if(depth){
		if(group_sep && depth % group_size == 0){
			putchar(group_sep);
			++count;
		}else if(digit_sep){
			putchar(digit_sep);
			++count;
		}
	}
	
	return count;
}

/**
 * Writes a string to stdout.
 * @param str String to write.
 * @return Non-negative if successful, EOF otherwise.
 */
int puts(const char *str){
	int count = 0;
	for(int i = 0; str[i] && i < MAX_LENGTH; ++i){
		if(putchar(str[i]) != str[i]){
			return EOF;
		}
		++count;
	}
	return count;
}

/**
 * Prints a string to stdout using the specified format.
 * @param format Format string.
 * @param ... The values used in the format.
 * @return Number of characters written if successful, EOF otherwise.
 */
int printf(const char *format, ...){
	int index = 0;
	unsigned int count = 0;
	va_list ap;
	va_start(ap, format);
	for(int i = 0; format[i] && i < MAX_LENGTH; ++i){
		if(format[i] != '%'){
			if(putchar(format[i]) != format[i]){
				return EOF;
			}
			++count;
		}else{
			bool handled = false;
			bool left = false;
			bool show_sign = false;
			bool zero_pad = false;
			unsigned int width = 0;
			bool dot = false;
			unsigned int precision = 0;
			unsigned int lastcount = count;
			while(!handled){
				++i;
				char type = format[i];
				if(type == 'p'){
					int ret = puts("0x");
					if(ret < 0){
						return EOF;
					}
					count += ret;
					type = 'X';
					width = 8;
				}
				if(type == '%'){
					if(putchar(format[i]) != format[i]){
						return EOF;
					}
					++count;
					handled = true;
				}else if(type == 'c'){
					putchar((char)(va_arg(ap, int) & 0x000000FF));
					++count;
					handled = true;
				}else if(type == 's'){
					int ret = puts(va_arg(ap, char*));
					if(ret < 0){
						return EOF;
					}
					count += ret;
					handled = true;
				}else if(type == 'i' || type == 'd'){
					enum putnum_sign sign = PUTNUM_SIGN_NLEAD;
					char pad = ' ';
					if(show_sign){
						sign = PUTNUM_SIGN_PLEAD;
					}
					if(zero_pad){
						pad = '0';
					}
					count += putnum(va_arg(ap, int), 10, 3, 0, 0, width, pad, sign, false);
					handled = true;
				}else if(type == 'u'){
					char pad = ' ';
					if(zero_pad){
						pad = '0';
					}
					count += putnum(va_arg(ap, int), 10, 3, 0, 0, width, pad, PUTNUM_SIGN_NONE, false);
					handled = true;
				}else if(type == 'x'){
					count += putnum(va_arg(ap, int), 16, 4, 0, 0, width, '0', PUTNUM_SIGN_NONE, false);
					handled = true;
				}else if(type == 'X'){
					count += putnum(va_arg(ap, int), 16, 4, 0, 0, width, '0', PUTNUM_SIGN_NONE, true);
					handled = true;
				}else if(type == 'o'){
					count += putnum(va_arg(ap, int), 8, 3, 0, 0, width, '0', PUTNUM_SIGN_NONE, false);
					handled = true;
				}else if(type == 'f'){
					count += putfloat(va_arg(ap, double), precision);
					handled = true;
				}else if(type == 'n'){
					*(va_arg(ap, int*)) = count;
					handled = true;
				}else if(type == '-'){
					left = true;
				}else if(type == '+'){
					show_sign = true;
				}else if(type == '0' && (width == 0 || dot)){
					zero_pad = true;
				}else if(type >= '0' && type <= '9'){
					if(!dot){
						width = (width * 10) + (type - '0');
					}else{
						precision = (precision * 10) + (type - '0');
					}
				}else if(type == '.'){
					dot = true;
				}
				++index;
			}
			if(left && lastcount + width < count){
				for(unsigned int i = 0; i < lastcount + width; ++i){
					putchar(' ');
					++count;
				}
			}
		}
	}
	return count;
}

/**
 * Writes a string to stderr.
 * @param str String to write.
 */
void perror(const char *str){
	for(int i = 0; str[i] && i < MAX_LENGTH; ++i){
		stderr->swrite(str[i]);
	}
}

/**
 * Writes a character to stdout.
 * @param ch Character to write.
 * @return ch if successful, EOF otherwise.
 */
char putchar(char ch){
	if(stdout->swrite(ch) == EOK){
		return ch;
	}
	return EOF;
}

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
unsigned int putnum(int num, unsigned int base, unsigned int group_size, char digit_sep, char group_sep, unsigned int length, char pad, enum putnum_sign neg, bool cap){
	return __putnum_actual(num, base, group_size, digit_sep, group_sep, length, pad, neg, cap, 0, false);
}

/**
 * Writes a binary integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int putbin(unsigned int num){
	return putnum(num, 2, 4, 0, 0, 0, 0, PUTNUM_SIGN_NONE, false);
}

/**
 * Writes an octal integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int putoct(unsigned int num){
	return putnum(num, 8, 3, 0, 0, 0, 0, PUTNUM_SIGN_NONE, false);
}

/**
 * Outputs a decimal number to the console.
 * @param num The number to output.
 * @return Number of characters written.
 */
int putint(int num){
	return putnum(num, 10, 3, 0, 0, 0, 0, PUTNUM_SIGN_NEMBED, false);
}

/**
 * Writes a hexadecimal integer to sdtout.
 * @param num The integer to write.
 * @return Number of characters written.
 */
int puthex(unsigned int num){
	return putnum(num, 16, 4, 0, 0, 0, 0, PUTNUM_SIGN_NONE, true);
}

/**
 * Outputs a floating point number to the console.
 * @param num The number to output.
 * @param prec The number of places after the decimal.
 * @return Number of characters written.
 */
int putfloat(double num, uint8_t prec){
	int count = 0;
	count += putint((int)num);
	putchar('.');
	++count;
	num -= (int)num;
	for(int i = 0; i < prec; ++i){
		num *= 10;
		putchar((int)num+'0');
		++count;
		num -= (int)num;
	}
	return count;
}

/**
 * Gets a single character from stdin.
 * @return The character received.
 */
unsigned char getchar(){
	return stdin->sread();
}

/**
 * Reads a string from stdin.
 * @param str Buffer to store the string to.
 * @return str if successful, NULL otherwise.
 */
char *gets(char *str){
	// Build input string
	unsigned int loc = 0;
	unsigned char ch = 0;
	while(ch != '\n'){
		ch = getchar();
		if(ch == '\b'){
			if(loc){
				--loc;
			}
		}else if(ch == '\t'){
			// Do nothing, for now.
		}else if(ch && loc < MAX_LENGTH-1 && ch != '\n'){
			str[loc] = ch;
			++loc;
		}
	}
	putchar('\n');
	str[loc] = 0;
	return str;
}

