/**
 * @file hal/zero.c
 * Zero Device functions.
 * @author Conlan Wesson
 */

#include "zero.h"

#include <errno.h>

/**
 * Always returns zero.
 * @return Zero.
 */
char zero_read(){
	return 0;
}

/**
 * Discards all input.
 * @param value for the sake of value.
 * @return EOK
 */
int zero_write(char value){
	(void)value;
	return EOK;
}

/**
 * Always returns EOK.
 * @return EOK
 */
int zero_flush(){
	return EOK;
}

/**
 * Zero device descriptor.
 */
device_descriptor zero_desc = {
	"Zero Device.",
	DEVICE_FLAG_INOUT | DEVICE_FLAG_STREAM | DEVICE_FLAG_VIRTUAL,
	0, 0,
	0, 0,
	0, 0,
	zero_read, zero_write, zero_flush
};
