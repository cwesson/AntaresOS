/**
 * @file dev/pci.c
 * Functions for handling the PCI bus.
 * @author Conlan Wesson
 */

#include "pci.h"

#include <kernel/ioport.h>
#include <stdint.h>

/**
 * Reads a 16bit word from the PCI bus.
 * @param bus The bus numebr to read from.
 * @param slot The device number to read from.
 * @param func The device function to read from.
 * @param offset The register to read from.
 * @return The value read from the PCI bus.
 */
uint16_t pci_read(uint16_t bus, uint16_t slot, uint16_t func, uint16_t offset){
	uint32_t address;
	uint32_t lbus = (uint32_t)bus;
	uint32_t lslot = (uint32_t)slot;
	uint32_t lfunc = (uint32_t)func;

	/* create configuration address as per Figure 1 */
	address = (uint32_t)((lbus << 16) | (lslot << 11) | (lfunc << 8) | (offset & 0xfc) | ((uint32_t)0x80000000));

	/* write out the address */
	outl(0xCF8, address);
	/* read in the data */
	return (uint16_t)((inl(0xCFC) >> ((offset & 2) * 8)) & 0xffff);
}
