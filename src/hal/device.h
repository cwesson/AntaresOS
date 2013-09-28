/**
 * @file hal/device.h
 * Common device descriptor definition.
 * @author Conlan Wesson
 */

#ifndef __HAL_DEVICE_H_
#define __HAL_DEVICE_H_

#include <stdint.h>
#include <stdbool.h>

#define DEVICE_FLAG_IN    0x01    //!< Flag for input devices.
#define DEVICE_FLAG_OUT   0x02    //!< Flag for output devices.
#define DEVICE_FLAG_INOUT 0x03    //!< Flag for both input and output devices.

#define DEVICE_FLAG_BLOCK  0x04    //!< Flag for block type devices.
#define DEVICE_FLAG_STREAM 0x08    //!< Flag for stream type devices.
#define DEVICE_FLAG_MIXED  0x0C    //!< Flag for block and stream devices.

#define DEVICE_FLAG_PHYSICAL 0x10    //!< Flag for physical devices.
#define DEVICE_FLAG_VIRTUAL  0x20    //!< Flag for virtual devices.

/**
 * Common device descriptor structure.
 */
typedef struct device_descriptor{
	char *name;      //!< Printable name of the device.
	uint32_t flags;    //!< Mode and type flags.
	
	unsigned int min_addr;    //!< Minimum address for block devices.
	unsigned int max_addr;    //!< Maximum address for block devices.
	unsigned int read_count;     //!< Number of bytes read.
	unsigned int write_count;    //!< Number of bytes wrote;
	
	/**
	 * Read from block device.
	 * @param addr Address to read from.
	 * @return The value read.
	 */
	int (*bread)(unsigned int addr);
	
	/**
	 * Write to block device.
	 * @param addr Address to write to.
	 * @param value Value to write.
	 * @return Error code or EOK.
	 */
	int (*bwrite)(unsigned int addr, int value);
	
	/**
	 * Read from stream device.
	 * @return The value read.
	 */
	char (*sread)();
	
	/**
	 * Write to stream device.
	 * @param value Value to write.
	 * @return Error code or EOK.
	 */
	int (*swrite)(char value);
	
	/**
	 * Flushes stream device.
	 * @return Error code or EOK.
	 */
	int (*flush)();
} device_descriptor;

#endif /* __HAL_DEVICE_H_ */
