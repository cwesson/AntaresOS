/**
 * @file lib/std/stdlib.c
 * Standard library functions.
 * @author Conlan Wesson
 */

#include "stdlib.h"

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include "dev/ram.h"
#include "hal/rand.h"
#include "constraint.h"

typedef uint32_t HEAP_T;               //!< Integer type to use for heap allocations.
const uint8_t HEAP_HEAD_SIZE = 2;              //!< Number of HEAP_T's in the header.
const uint8_t HEAP_ALIGN = sizeof(HEAP_T);     //!< Alignment of all heap allocations.
const uint32_t HEAP_BLOCK_USED = ((HEAP_T)0x80000000);    //!< Block used flag.

static HEAP_T *heap = 0;

static uint32_t used = 0;
static uint32_t free_count = 0;

/**
 * Initializes the heap.
 * @param start Null terminated array of pointers to the start of heap spaces.
 * @param end Null terminated array of pointers to the end of heap spaces.
 * @return Error code, or EOK if successful.
 */
int heap_init(void *start[], void *end[]){
	// Do not reinitialize the heap!
	if(heap != 0){
		return ECANCELED;
	}
	
	heap = (HEAP_T*)start[0];
	for(int i = 0; start[i] && end[i]; ++i){
		HEAP_T *head = (HEAP_T*)start[i];
		HEAP_T *tail = ((HEAP_T*)end[i] - 1);
		
		// Create the heap header.
		head[0] = (HEAP_T)start[i+1];    // Pointer to the next heap block.
		head[1] = (HEAP_T)(((uint8_t*)end[i] - (uint8_t*)start[i]) / HEAP_ALIGN);    // Size of the heap block.
		free_count += head[1];
		*tail = 0;    // Null Terminate the heap.
		
		// Allocate the first block.
		head[2] = (HEAP_T)NULL;    // Pointer to the previous allocation block.
		head[3] = head[1] - HEAP_HEAD_SIZE + 1;    // Size of the allocation block.
	}
	
	return EOK;
}

/**
 * Allocates memory on the heap.
 * @param size Number of bytes to allocate.
 * @return Pointer to the allocated space.
 */
void *malloc(uint32_t size){
	HEAP_T *head = heap;
	HEAP_T *block = heap + HEAP_HEAD_SIZE;
	
	size += HEAP_ALIGN - (size % HEAP_ALIGN);
	size = size / HEAP_ALIGN;    // Size is now in number of HEAP_T's.
	size += HEAP_HEAD_SIZE;
	
	while(head){
		while(block < head + head[1]){
			if(!(block[1] & HEAP_BLOCK_USED) && block[1] >= size){
				// Allocate this block.
				if(block[1] > size){
					if(block[1] - size >= HEAP_HEAD_SIZE){
						// There is at least enough space for the header.
						HEAP_T *next = block + size;
						next[0] = (HEAP_T)block;
						next[1] = block[1] - size;
					}else{
						// Allocate the extra space with this block.
						size = block[1];
					}
				}
				block[1] = size | HEAP_BLOCK_USED;    // Mark the block as used.
				free_count -= size;
				used += size;
				printf("Free = %u    Used = %u\n", free_count, used);
				return (void*)(block + HEAP_HEAD_SIZE);
			}
			
			block = block + (block[1] & ~HEAP_BLOCK_USED);    // Advance to next allocation block.
		}
		
		head = (HEAP_T*)head[0];    // Advance to next heap block.
	}
	
	return NULL;
}

/**
 * Deallocates space on the heap.
 * @param ptr Pointer to the space to deallocate.
 */
void free(void *ptr){
	HEAP_T *block = (HEAP_T*)ptr;
	block -= HEAP_HEAD_SIZE;
	
	HEAP_T *prev = (HEAP_T*)block[0];
	HEAP_T *next = block + block[1];
	
	block[1] = block[1] & ~HEAP_BLOCK_USED;    // Mark the block free.
	free_count += block[1];
	used -= block[1];
	
	// Check if previous block is free.
	if(prev){
		if(prev[1] & ~HEAP_BLOCK_USED){
			prev[1] += block[1];
			block = prev;
		}
	}
	
	// Check if next block is free.
	if(next){
		if(next[1] & ~HEAP_BLOCK_USED){
			block[1] += next[1];
		}
	}
}


/**
 * Generates a random number.
 * @return A random number.
 */
int rand(){
	uint32_t one = rand_desc.sread();
	uint32_t two = rand_desc.sread();
	uint32_t three = rand_desc.sread();
	uint32_t four = rand_desc.sread();
	return (one << 24) ^ (two << 16) ^ (three << 8) ^ four;
}

/**
 * Seeds the random number generator.
 * @param val The value to seed the RNG.
 */
void srand(unsigned int val){
	rand_desc.swrite((char)val);
}

/**
 * Causes abnormal program termination.
 */
void abort(){
	raise(SIGABRT);
}

/**
 * Set the constraint handler function.
 * @param handler New constraint callback function.
 * @return Old constraint callback function.
 */
constraint_handler_t set_constraint_handler_s(constraint_handler_t handler){
	return __set_constraint_handler_s_actual(handler);
}

/**
 * Aborts execution of the program.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void abort_handler_s(const char *restrict msg, void *restrict ptr, errno_t error){
	(void)msg;
	(void)ptr;
	(void)error;
	abort();
}

/**
 * Return without performing any action.
 * @param msg Description of the error.
 * @param ptr Pointer to error details.
 * @param error error number that occured.
 */
void ignore_handler_s(const char *restrict msg, void *restrict ptr, errno_t error){
	(void)msg;
	(void)ptr;
	(void)error;
}

