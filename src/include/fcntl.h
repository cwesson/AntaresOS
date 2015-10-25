/**
 * @file include/fcntl.h
 * Implementation of the C file control operations.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_FCNTL_H_
#define __INCLUDE_FCNTL_H_

#include "sys/files.h"

enum {
	O_CREAT    = 0x0001, //!< Create file if it does not exist.
	O_EXCL     = 0x0002, //!< Exclusive use flag.
	O_NOCTTY   = 0x0004, //!< Do not assign controlling terminal.
	O_TRUNC    = 0x0008, //!< Truncate flag.
	// File status flags used for open() and fcntl()
	O_APPEND   = 0x0010, //!< Set append mode.
	O_DSYNC    = 0x0020, //!< Write according to synchronised I/O data integrity completion.
	O_NONBLOCK = 0x0040, //!< Non-blocking mode.
	O_RSYNC    = 0x0080, //!< Synchronised read I/O operations.
	O_SYNC     = 0x0100, //!< Write according to synchronised I/O file integrity completion.
	// Mask for use with file access modes
	O_ACCMODE  = 0x0200, //!< Mask for file access modes.
	//File access modes used for open() and fcntl()
	O_RDONLY   = 0x0400, //!< Open for reading only.
	O_RDWR     = 0x0800, //!< Open for reading and writing.
	O_WRONLY   = 0x1000, //!< Open for writing only.
};

/**
 * Opens the specified file.
 * @param path Path to the file to open.
 * @param flags Open file flags.
 * @return File descriptor for the open file.
 */
int open(const char *path, int flags){
	struct files_open_arg arg = {
		path, flags
	};
	syscall_payload payload;
	payload.v = &arg;
	syscall(SYSCALL_OPEN, &payload);
	return payload.i;
}

#endif /* __INCLUDE_FCNTL_H_ */

