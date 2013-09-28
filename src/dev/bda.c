/**
 * @file dev/bda.c
 * BIOS Data Area device driver.
 * @author Conlan Wesson
 */

#include "bda.h"

#include <errno.h>

#define MIN_ADDR 0x00000400u
#define MAX_ADDR 0x00000497u

//! BIOS Data Area device descriptor.
device_descriptor bda_desc = {
	"BIOS Data Area",
	DEVICE_FLAG_IN | DEVICE_FLAG_BLOCK | DEVICE_FLAG_PHYSICAL,
	MIN_ADDR, MAX_ADDR,
	0, 0,
	bda_read, 0,
	0, 0, 0
};

/**
 * Read from BIOS Data Area.
 * @param addr The address to read from.
 * @return The value read.
 */
int bda_read(unsigned int addr){
	if(addr == BDA_COM1_IOPORT){
		return *((uint16_t*)BDA_COM1_IOPORT);
	}else if(addr == BDA_LPT1_IOPORT){
		return *((uint16_t*)BDA_LPT1_IOPORT);
	}else if(addr == BDA_EBDA_BASE){
		return *((uint16_t*)BDA_EBDA_BASE);
	}else if(addr == BDA_HW_DETECT){
		return *((uint16_t*)BDA_HW_DETECT);
	}else if(addr == BDA_DISP_MODE){
		return (int)(*((uint8_t*)BDA_DISP_MODE));
	}else if(addr == BDA_VIDEO_IOPORT){
		return (int)(*((uint16_t*)BDA_VIDEO_IOPORT));
	}else if(addr == BDA_NUM_TICKS){
		return *((uint16_t*)BDA_NUM_TICKS);
	}else if(addr == BDA_NUM_HDD){
		return (int)(*((uint8_t*)BDA_NUM_HDD));
	}else if(addr == BDA_KEYBOARD_LED){
		return (int)(*((uint8_t*)BDA_KEYBOARD_LED));
	}
	return EDOM;
}
