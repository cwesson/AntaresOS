/**
 * @file dev/com.c
 * Functions for handling COM ports.
 * @author Conlan Wesson
 */

#include "com.h"

#include <errno.h>
#include <kernel/ioport.h>
#include <queue.h>
#include <stdbool.h>
#include <stdint.h>
#include "../sys/interrupt/isr.h"

#define COM_PORT_1 0x03F8    //!< I/O port for COM1.
#define COM_PORT_2 0x02F8    //!< I/O port for COM2.
#define COM_PORT_3 0x03E8    //!< I/O port for COM3.
#define COM_PORT_4 0x02E8    //!< I/O port for COM4.

#define COM_DATA_5 0x00
#define COM_DATA_6 0x01
#define COM_DATA_7 0x02
#define COM_DATA_8 0x03

#define COM_STOP_1 0x00
#define COM_STOP_2 0x04

#define COM_DLAB 0x80
#define COM_BUFFER_SIZE 256
#define COM_INTERRUPT_ENABLE 0x03

static bool is_open[4] = {false, false, false, false};

static queue buffer_in[4];
static int buff_data_in[4][COM_BUFFER_SIZE];
static queue buffer_out[4];
static int buff_data_out[4][COM_BUFFER_SIZE];

static void com_isr(isr_regs regs){
	(void)regs;
	
}

/**
 * Initial setup of COM devices.
 */
void com_init(){
	// Disable interrupts.
	outb(COM_PORT_1 + 1, 0x00);
	outb(COM_PORT_2 + 1, 0x00);
	outb(COM_PORT_3 + 1, 0x00);
	outb(COM_PORT_4 + 1, 0x00);
	
	// Setup input queues.
	new_queue(&buffer_in[0], (int*)&buff_data_in[0], COM_BUFFER_SIZE);
	new_queue(&buffer_in[1], (int*)&buff_data_in[1], COM_BUFFER_SIZE);
	new_queue(&buffer_in[2], (int*)&buff_data_in[2], COM_BUFFER_SIZE);
	new_queue(&buffer_in[3], (int*)&buff_data_in[3], COM_BUFFER_SIZE);
	
	// Setup output queues.
	new_queue(&buffer_out[0], (int*)&buff_data_out[0], COM_BUFFER_SIZE);
	new_queue(&buffer_out[1], (int*)&buff_data_out[1], COM_BUFFER_SIZE);
	new_queue(&buffer_out[2], (int*)&buff_data_out[2], COM_BUFFER_SIZE);
	new_queue(&buffer_out[3], (int*)&buff_data_out[3], COM_BUFFER_SIZE);
	
	// Register interrupt handlers.
	isr_register(IRQ4, &com_isr);
	isr_register(IRQ3, &com_isr);
}

/**
 * Opens a COM port for read/write.
 * @param line The COM port number (1-4).
 * @param baud The baud rate of the connection.
 * @param data The number of data bits (5-8).
 * @param stop The number of stop bits (1 or 2).
 * @param parity The mode of the parity bit.
 * @return true if the port could be opened.
 */
bool com_open(uint8_t line, uint16_t baud, uint8_t data, uint8_t stop, uint8_t parity){
	// Determine the rate divisor.
	uint16_t divisor = baud / COM_BAUD_MAX;
	uint8_t divlo = (divisor & 0x0F);
	uint8_t divhi = (divisor & 0xF0) >> 8;
	
	// Determine the data bit setting.
	if(data >= 5 && data <= 8){
		data -= 5;
	}else{
		return false;
	}
	
	// Determine the stop bit setting.
	if(stop == 1 || stop == 2){
		stop = (stop - 1) * 4;
	}else{
		return false;
	}
	
	// Determine the IO port.
	uint16_t port = 0;
	if(line == 1){
		port = COM_PORT_1;
	}else if(line == 2){
		port = COM_PORT_2;
	}else if(line == 3){
		port = COM_PORT_3;
	}else if(line == 4){
		port = COM_PORT_4;
	}else{
		return false;
	}
	
	is_open[line-1] = true;
	
	outb(port + 1, 0);           // Disable interrupts.
	outb(port + 3, COM_DLAB);    // Enable DLAB in order to set baud rate.
	outb(port + 0, divlo);       // Set baud rate.
	outb(port + 1, divhi);
	outb(port + 3, data | stop | parity);    // Bit settings.
	outb(port + 1, COM_INTERRUPT_ENABLE);    // Enable interrupts
	return true;
}

/**
 * Closes an open COM port.
 * @param line The COM port number (1-4).
 * @return Error code, or EOK is successful.
 */
int com_close(uint8_t line){
	// Determine the IO port.
	uint16_t port = 0;
	if(line == 1){
		port = COM_PORT_1;
	}else if(line == 2){
		port = COM_PORT_2;
	}else if(line == 3){
		port = COM_PORT_3;
	}else if(line == 4){
		port = COM_PORT_4;
	}else{
		return EDOM;
	}
	
	outb(port + 1, 0);    // Disable interrupts.
	
	if(is_open[line-1]){
		is_open[line-1] = false;
		clear_queue(&buffer_out[line-1]);
		clear_queue(&buffer_in[line-1]);
	}
	
	return EOK;
}

/**
 * Reads a character from a COM port.
 * @param line The COM port number (1-4).
 * @return The character read.
 */
char com_read(uint8_t line){
	if(line >= 1 && line <= 4 && is_open[line-1]){
		return (char)dequeue(&buffer_in[line-1]);
	}
	return 0;
}

/**
 * Writes a character to a COM port.
 * @param line The COM port number (1-4).
 * @param ch The character to write.
 * @return true if the character was written.
 */
bool com_write(uint8_t line, char ch){
	if(line >= 1 && line <= 4 && is_open[line-1]){
		enqueue(&buffer_out[line-1], ch);
		return true;
	}
	return false;
}
