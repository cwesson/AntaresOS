/**
 * @file include/queue.c
 * Queue Data Structure (FIFO).
 * @author Conlan Wesson
 */

#include "queue.h"

#include <stdbool.h>

/**
 * Initializes a new queue.
 * @param q Pointer to the queue to initialize.
 * @param data Pointer to an array of ints of length size.
 * @param size The maximum length of the queue.
 * @return Pointer to the new queue.
 */
queue *new_queue(queue *q, int *data, unsigned int size){
	q->capacity = size;
	q->size = 0;
	q->in = 0;
	q->out = 0;
	q->data = data;
	return q;
}

/**
 * Adds a value to the end of the queue.
 * @param q Pointer to the queue to add the value to.
 * @param val The value to add to the queue.
 * @return true if the value was added successfully.
 */
bool enqueue(queue *q, int val){
	q->data[q->in] = val;
	q->in++;
	if(q->in > q->capacity){
		q->in = 0;
	}
	if(q->size == q->capacity){
		// An old value was overwritten.
		q->out++;
		if(q->out > q->capacity){
			q->out = 0;
		}
		return false;
	}
	q->size++;
	return true;
}

/**
 * Removes the value from the begining of the queue.
 * @param q Pointer to the queue to remove the value from.
 * @return The value removed from the queue.
 */
int dequeue(queue *q){
	if(q->size){
		int ret = q->data[q->out];
		q->size--;
		q->out++;
		if(q->out > q->capacity){
			q->out = 0;
		}
		return ret;
	}
	return 0;
}

/**
 * Clears all values from the queue.
 * @param q The queue to clear.
 */
void clear_queue(queue *q){
	q->size = 0;
	q->in = 0;
	q->out = 0;
}
