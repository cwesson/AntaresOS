/**
 * @file dev/vga.c
 * Functions for video output device.
 * @author Conlan Wesson
 */

#include "vga.h"

#include <errno.h>
#include <kernel/ioport.h>
#include <stdint.h>
#include "bda.h"

#define MIN_ADDR 0x00000000u    //!< Minimum supported address.
#define MAX_ADDR 0x000007D1u    //!< Maximum supported address.  (80*25) + 1    (1 for special addresses).
#define CURSOR_LO_COMMAND 0x0F    //!< Command to write low byte to I/O port
#define CURSOR_HI_COMMAND 0x0E    //!< Command to write high byte to I/O port

static volatile uint16_t *videoram = (uint16_t *)0xB8000;    //!< Video output buffer.
static uint16_t ioport = 0;                                //!< Video IOport base address from BDA.

//! Video out device descriptor.
device_descriptor vga_desc = {
	"Video Output",
	DEVICE_FLAG_INOUT | DEVICE_FLAG_BLOCK | DEVICE_FLAG_PHYSICAL,
	MIN_ADDR, MAX_ADDR,
	0, 0,
	vga_read, vga_write,
	0, 0, 0
};

/**
 * Initiliaze the video output device.
 */
void vga_init(){
	ioport = bda_desc.bread(BDA_VIDEO_IOPORT);
	for(unsigned int i = MIN_ADDR; i <= MAX_ADDR; ++i){
		videoram[i] = 0x0F00 | ' ';
	}
}

/**
 * Read from the video output.
 * @param addr Address to read from.
 * @return The value read.
 */
int vga_read(unsigned int addr){
	if(addr == VGA_ADDR_CURSOR){
		return 0;
	}else if(addr <= MAX_ADDR){
		vga_desc.read_count += 2;
		return videoram[addr];
	}
	return 0;
}

/**
 * Write to the video output.
 * @param addr Address to write to.
 * @param value Value to write.
 * @return Error code or EOK.
 */
int vga_write(unsigned int addr, int value){
	if(addr == VGA_ADDR_CURSOR){
		if(!ioport){
			vga_init();
		}
		// Write cursor low byte.
		outb(ioport, 0x0F);
		outb(ioport+1, (uint8_t)((uint16_t)value & 0xFF));
		// Write cursor high byte.
		outb(ioport, 0x0E);
		outb(ioport+1, (uint8_t)((((uint16_t)value) >> 8) & 0xFF));
	}else if(addr <= MAX_ADDR){
		videoram[addr] = value;
	}else{
		return EDOM;
	}
	vga_desc.write_count += 2;
	return EOK;
}

