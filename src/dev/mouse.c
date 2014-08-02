/**
 * @file dev/mouse.c
 * Functions for handling mouse input.
 * @author Conlan Wesson.
 */

#include "mouse.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include "../sys/interrupt/isr.h"
#include <stdio.h>

static int8_t mouse_x = 0;
static int8_t mouse_y = 0;

/**
 * ISR for mouse input.
 * @param regs Registers from before the interrupt.
 */
static void mouse_isr(isr_regs regs){
	static uint8_t mouse_cycle = 0;
	static int8_t mouse_byte[] = {0, 0, 0};
	static int8_t prev_x = -1;
	static int8_t prev_y = -1;
	
	(void)regs;
	
	switch(mouse_cycle){
		case 0:
			mouse_byte[0] = inb(0x60);
			if(mouse_byte[0] & 0x08){
				++mouse_cycle;
			}
			break;
		case 1:
			mouse_byte[1] = inb(0x60);
			++mouse_cycle;
			break;
		case 2:
			mouse_byte[2] = inb(0x60);
			mouse_cycle = 0;
			if(!(mouse_byte[0] & 0xC0)){
				mouse_x += mouse_byte[1]/2;
				mouse_y -= mouse_byte[2]/2;
				if(mouse_x < 0){
					mouse_x = 0;
				}else if(mouse_x > 79){
					mouse_x = 79;
				}
				if(mouse_y < 0){
					mouse_y = 0;
				}else if(mouse_y > 24){
					mouse_y = 24;
				}
				if(prev_x != mouse_x || prev_y != mouse_y){
					prev_x = mouse_x;
					prev_y = mouse_y;
				}
			}
			break;
	}
}

static void mouse_wait(uint8_t type){
	uint32_t time_out = 100000;
	if(type == 0){
		while(--time_out){
			if((inb(0x64) & 1) == 1){
				return;
			}
		}
		return;
	}else{
		while(--time_out){
			if((inb(0x64) & 2) == 0){
				return;
			}
		}
		return;
	}
}

static void mouse_write(uint8_t val){
	//Wait to be able to send a command
	mouse_wait(1);
	//Tell the mouse we are sending a command
	outb(0x64, 0xD4);
	//Wait for the final part
	mouse_wait(1);
	//Finally write
	outb(0x60, val);
}

static uint8_t mouse_read(){
	//Get's response from mouse
	mouse_wait(0);
	return inb(0x60);
}

/**
 * Setup the mouse driver.
 */
void mouse_init(){
	uint8_t status;
	
	//Enable the auxiliary mouse device
	mouse_wait(1);
	outb(0x64, 0xA8);
	
	//Enable the interrupts
	mouse_wait(1);
	outb(0x64, 0x20);
	mouse_wait(0);
	status = (inb(0x60) | 2);
	mouse_wait(1);
	outb(0x64, 0x60);
	mouse_wait(1);
	outb(0x60, status);
	
	//Tell the mouse to use default settings
	mouse_write(0xF6);
	mouse_read();  //Acknowledge
	
	//Enable the mouse
	mouse_write(0xF4);
	mouse_read();  //Acknowledge
	
	//Setup the mouse handler
	isr_register(IRQ12, &mouse_isr);
}

