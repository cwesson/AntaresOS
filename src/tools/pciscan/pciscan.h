/**
 * @file tools/pciscan/pciscan.h
 * Functions for scanning the PCI bus.
 * @author Conlan Wesson
 */

#ifndef TOOLS_PCISCAN_PCISCAN_H
#define TOOLS_PCISCAN_PCISCAN_H

#include "../../dev/pci.h"

/**
 * Scans the PCI bus for devices and prints info about each.
 */
void pciscan_run();

#endif
