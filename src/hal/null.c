/**
 * @file hal/null.c
 * NULL Device functions.
 * @author Conlan Wesson
 */

#include "null.h"

#include <errno.h>

/**
 * Always returns EOF.
 * @return EOF.
 */
char null_read(){
	return -1;
}

/**
 * Discards all input.
 * @param value for the sake of value.
 * @return EOK
 */
int null_write(char value){
	(void)value;
	return EOK;
}

/**
 * Always returns EOK
 * @return EOK
 */
int null_flush(){
	return EOK;
}

/**
 * NULL device descriptor.
 */
device_descriptor null_desc = {
	"Null Device.",
	DEVICE_FLAG_INOUT | DEVICE_FLAG_STREAM | DEVICE_FLAG_VIRTUAL,
	0, 0,
	0, 0,
	0, 0,
	null_read, null_write, null_flush
};
