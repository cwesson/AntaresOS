/**
 * @file sys/files.c
 * File control operations.
 * @author Conlan Wesson
 */

#include "files.h"

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include "syscall.h"
#include "hal/device.h"

//! File descriptor control block.
typedef struct {
	device_descriptor *dev;    //!< Device descriptor block.
	off_t offset;              //!< Seek position into file.
} file_descriptor;

static file_descriptor fds[512];

static int next_fd(){
	static unsigned int next = 0;
	unsigned int first = next;
	while(fds[next].dev != NULL){
		next++;
		if(next >= sizeof(fds)/sizeof(file_descriptor)){
			next = 0;
		}
		if(next == first){
			return -1;
		}
	}
	return next;
}

/**
 * File open system call.
 * @param arg Syscall payload pointing to file open struct.
 * @return File descriptor for the file.
 */
void files_open(syscall_payload *arg){
	struct files_open_arg *args = (struct files_open_arg*)arg->v;
	printf("files_open %s\n", args->path);
	arg->i = next_fd();
}

/**
 * File close system call.
 * @param arg Syscall payload containing file descriptor.
 * @return EOK if successful, error code otherwise
 */
void files_close(syscall_payload *arg){
	int fd = arg->i;
	if(fds[fd].dev != NULL){
		printf("files_close %s\n", fds[fd].dev);
		fds[fd].offset = 0;
		fds[fd].dev = NULL;
		arg->i = EOK;
	}else{
		arg->i = EBADF;
	}
}

/**
 * Initialize file tree.
 */
void files_init(){
	syscall_register(SYSCALL_OPEN, files_open);
	syscall_register(SYSCALL_CLOSE, files_close);
}

