/**
 * @file dev/sdt.h
 * System Description Table data structure controller.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_SDT_H_
#define __INCLUDE_SDT_H_

#include "hal/device.h"

typedef struct sdt_header{
	char signature[4];
	uint32_t length;
	uint8_t revision;
	uint8_t checksum;
	char oemid[6];
	char oem_tableid[8];
	uint32_t oem_rev;
	uint32_t creator_id;
	uint32_t creator_rev;
} sdt_header;

//! System Description Table device descriptor.
device_descriptor sdt_desc;

/**
 * Initializes the SDT for use.
 * @return Error code on failure, or EOK.
 */
int sdt_init();

/**
 * Read from the System Description Table.
 * @param addr The address to read from.
 * @return The value read.
 */
int sdt_read(unsigned int);

#endif

