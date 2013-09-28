/**
 * @file tools/cpuid/cpuid.h
 * Functions for running CPUID tests.
 * @author Conlan Wesson
 */

#ifndef TOOLS_CPUID_CPUID_H
#define TOOLS_CPUID_CPUID_H

#include <stdbool.h>

/**
 * Checks the vendor string of the CPU.
 * @param vendor Location to store the output string, must be at least 13bytes.
 * @return Pointer to the vendor string.
 */
char *cpuid_vendor(char *);

/**
 * Checks the name string of the CPU.
 * @param name Location to store the output string, must be at least 48bytes.
 * @return Pointer to the name string.
 */
char *cpuid_name(char *);

/**
 * Runs a series of CPUID checks and prints the results.
 * @return true if CPUID is supported, false otherwise.
 */
bool cpuid_run();

#endif
