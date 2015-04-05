/**
 * @file dev/com.h
 * Functions for handling COM ports.
 * @author Conlan Wesson
 */

#ifndef __DEV_COM_H_
#define __DEV_COM_H_

#include <stdbool.h>
#include <stdint.h>

const uint32_t COM_BAUD_MAX = 115200u;
typedef enum {
	COM_PARITY_NONE  = 0x00,
	COM_PARITY_ODD   = 0x08,
	COM_PARITY_EVEN  = 0x18,
	COM_PARITY_MARK  = 0x28,
	COM_PARITY_SPACE = 0x38
} com_parity;

/**
 * Initial setup of COM devices.
 */
void com_init();

/**
 * Opens a COM port for read/write.
 * @param line The COM port number (1-4).
 * @param baud The baud rate of the connection.
 * @param data The number of data bits (5-8).
 * @param stop The number of stop bits (1 or 2).
 * @param parity The mode of the parity bit.
 * @return true if the port could be opened.
 */
bool com_open(uint8_t line, uint32_t baud, uint8_t data, uint8_t stop, com_parity parity);

/**
 * Closes an open COM port.
 * @param line The COM port number (1-4).
 * @return Error code, or EOK is successful.
 */
int com_close(uint8_t line);

/**
 * Reads a character from a COM port.
 * @param line The COM port number (1-4).
 * @return The character read.
 */
char com_read(uint8_t line);

/**
 * Writes a character to a COM port.
 * @param line The COM port number (1-4).
 * @param ch The character to write.
 * @return true if the character was written.
 */
bool com_write(uint8_t line, char ch);

#endif

