/**
 * @file include/fcntl.h
 * Implementation of the C file control operations.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_FCNTL_H_
#define __INCLUDE_FCNTL_H_

#include "sys/files.h"

//! Values used for cmd in fcntl().
enum {
	F_DUPFD,    //!< Duplicate file descriptor.
	F_GETFD,    //!< Get file descriptor flags.
	F_SETFD,    //!< Set file descriptor flags.
	F_GETFL,    //!< Get file status flags and file access modes.
	F_SETFL,    //!< Set file status flags.
	F_GETLK,    //!< Get record locking information.
	F_SETLK,    //!< Set record locking information.
	F_SETLKW,   //!< Set record locking information; wait if blocked.
	// File descriptor flags used for fcntl()
	FD_CLOEXEC, //!< Close the file descriptor upon execution of an exec family function.
	// Values for l_type used for record locking with fcntl()
	F_RDLCK,    //!< Shared or read lock.
	F_UNLCK,    //!< Unlock.
	F_WRLCK,    //!< Exclusive or write lock.
};

//! Values used for flags in open().
enum {
	O_CREAT    = 0x1000, //!< Create file if it does not exist.
	O_EXCL     = 0x2000, //!< Exclusive use flag.
	O_NOCTTY   = 0x4000, //!< Do not assign controlling terminal.
	O_TRUNC    = 0x8000, //!< Truncate flag.
	// File status flags used for open() and fcntl()
	O_APPEND   = 0x0010, //!< Set append mode.
	O_DSYNC    = 0x0020, //!< Write according to synchronised I/O data integrity completion.
	O_NONBLOCK = 0x0040, //!< Non-blocking mode.
	O_RSYNC    = 0x0080, //!< Synchronised read I/O operations.
	O_SYNC     = 0x0100, //!< Write according to synchronised I/O file integrity completion.
	//File access modes used for open() and fcntl()
	O_RDONLY   = 0x0004, //!< Open for reading only.
	O_WRONLY   = 0x0002, //!< Open for writing only.
	O_RDWR     = O_RDONLY | O_WRONLY, //!< Open for reading and writing.
	// Mask for use with file access modes
	O_ACCMODE  = O_RDONLY | O_WRONLY, //!< Mask for file access modes.
};

typedef struct {
	short l_type;      //!< Type of lock.
	short l_whence;    //!< Flag for starting offset.
	off_t l_start;     //!< Relative offset in bytes.
	off_t l_len;       //!< Size, if 0 then until EOF.
	pid_t l_pid;       //!< Process ID of the process holding the lock.
} flock;

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

/**
 * Closes the specified file.
 * @param fd File descriptor for the open file.
 * @return 0 if successful, -1 otherwise.
 */
int close(int fd){
	syscall_payload payload;
	payload.i = fd;
	syscall(SYSCALL_CLOSE, &payload);
	errno = payload.i;
	if(payload.i == EOK){
		return 0;
	}else{
		return -1;
	}
}

#endif /* __INCLUDE_FCNTL_H_ */

