/**
 * @file dev/bda.h
 * BIOS Data Area device driver.
 * @author Conlan Wesson
 */

#ifndef __DEV_BDA_H_
#define __DEV_BDA_H_

#include "hal/device.h"

typedef enum {
	BDA_COM1_IOPORT  = 0x0400,
	BDA_LPT1_IOPORT  = 0x0408,
	BDA_EBDA_BASE    = 0x040E,
	BDA_HW_DETECT    = 0x0410,
	BDA_DISP_MODE    = 0x0449,
	BDA_VIDEO_IOPORT = 0x0463,
	BDA_NUM_TICKS    = 0x046C,
	BDA_NUM_HDD      = 0x0475,
	BDA_KEYBOARD_LED = 0x0497
} bda_port;

//! BIOS Data Area device descriptor.
extern device_descriptor bda_desc;

/**
 * Read from BIOS Data Area.
 * @param addr The address to read from.
 * @return The value read.
 */
int bda_read(bda_port);

#endif

