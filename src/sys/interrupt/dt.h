/**
 * @file interrupt/dt.h
 * Functions for setting up and using the descriptor tables.
 * @author Conlan Wesson
 */

#ifndef __INTERRUPT_DT_H_
#define __INTERRUPT_DT_H_

#include <stdint.h>

/**
 * Initializes the descriptor tables.
 */
void descriptor_tables_init();

#endif

