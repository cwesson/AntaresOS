/**
 * @file sys/file.c
 * File control operations.
 * @author Conlan Wesson
 */

#include "files.h"

#include <stdio.h>
#include "syscall.h"

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
	arg->i = 0;
}

