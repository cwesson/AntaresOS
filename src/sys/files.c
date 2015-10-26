/**
 * @file sys/files.c
 * File control operations.
 * @author Conlan Wesson
 */

#include "files.h"

#include <stdio.h>
#include <stdint.h>
#include "syscall.h"
#include "hal/device.h"

typedef struct {
	device_descriptor *dev;
	uint64_t position;
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
 * Initialize file tree.
 */
void files_init(){
	syscall_register(SYSCALL_OPEN, files_open);
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

