/**
 * @file include/queue.h
 * Queue Data Structure (FIFO).
 * @author Conlan Wesson
 */

#ifndef INCLUDE_QUEUE_H
#define INCLUDE_QUEUE_H

#include <stdbool.h>

/**
 * Struct to store queues of ints.
 */
typedef struct queue{
	unsigned int capacity;
	unsigned int size;
	unsigned int in;
	unsigned int out;
	int *data;
} queue;

/**
 * Initializes a new queue.
 * @param q Pointer to the queue to initialize.
 * @param data Pointer to an array of ints of length size.
 * @param size The maximum length of the queue.
 * @return Pointer to the new queue.
 */
queue *new_queue(queue *, int *, unsigned int);

/**
 * Adds a value to the end of the queue.
 * @param q Pointer to the queue to add the value to.
 * @param val The value to add to the queue.
 * @return true if the value was added successfully.
 */
bool enqueue(queue *, int);

/**
 * Removes the value from the begining of the queue.
 * @param q Pointer to the queue to remove the value from.
 * @return The value removed from the queue.
 */
int dequeue(queue *);

/**
 * Clears all values from the queue.
 * @param q The queue to clear.
 */
void clear_queue(queue *);

#endif
