/**
 * @file dev/vga.h
 * Functions for video output device.
 * @author Conlan Wesson
 */

#ifndef DEV_VGA_H
#define DEV_VGA_H

#include <errno.h>

#include "../hal/device.h"

#define VGA_ADDR_CURSOR 0x000007D1u    //!< Special address to set the cursor position.

//! Video out device descriptor.
device_descriptor vga_desc;

/**
 * Initiliaze the video output device.
 */
void vga_init();

/**
 * Read from the video output.
 * @param addr Address to read from.
 * @return The value read.
 */
int vga_read(unsigned int);

/**
 * Write to the video output.
 * @param addr Address to write to.
 * @param value Value to write.
 * @return Error code or EOK.
 */
int vga_write(unsigned int, int);

#endif
