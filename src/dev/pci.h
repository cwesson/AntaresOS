/**
 * @file dev/pci.h
 * Functions for handling the PCI bus.
 * @author Conlan Wesson
 */

#ifndef DEV_PCI_H
#define DEV_PCI_H

#include <stdint.h>

/**
 * Reads a 16bit word from the PCI bus.
 * @param bus The bus numebr to read from.
 * @param slot The device number to read from.
 * @param func The device function to read from.
 * @param offset The register to read from.
 * @return The value read from the PCI bus.
 */
uint16_t pci_read(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset);

#endif
