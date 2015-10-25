/**
 * @file sys/files.h
 * File control operations.
 * @author Conlan Wesson
 */

#ifndef __SYS_FILES_H_
#define __SYS_FILES_H_

#include "sys/syscall.h"

struct files_open_arg{
	const char *path;
	int flags;
};

/**
 * Initialize file tree.
 */
void files_init();

/**
 * File open system call.
 * @param arg Syscall payload pointing to file open struct.
 */
void files_open(syscall_payload *arg);

#endif

