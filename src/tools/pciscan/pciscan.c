/**
 * @file tools/pciscan/pciscan.c
 * Functions for scanning the PCI bus.
 * @author Conlan Wesson
 */

#include "pciscan.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "dev/pci.h"

//! Array of PCI device class names.
const char *const pci_classes[] = {
	"Device Prior to class code",
	"Mass Storage Controller",
	"Network Controller",
	"Display Controller",
	"Multimedia Controller",
	"Memory Controller",
	"Bridge Device",
	"Simple Communication Controller",
	"Base System Peripheral",
	"Input Device",
	"Docking Station",
	"Processor",
	"Serial Bus Controller",
	"Wireless Controller",
	"Intelligent I/O Controller",
	"Satellite Communication Controller",
	"Encryption/Decryption Controller",
	"Data Acquisition and Signal Processing Controller"
};

/**
 * Scans the PCI bus for devices and prints info about each.
 */
void pciscan_run(){
	uint16_t bus;
	for(bus = 0; bus < 256; ++bus){
		uint16_t dev;
		bool bus_first = true;
		for(dev = 0; dev < 32; ++dev){
			uint16_t vendor = pci_read(bus, dev, 0, 0);
			if(vendor != 0xFFFF){
				uint16_t device = pci_read(bus, dev, 0, 2);
				uint16_t class = pci_read(bus, dev, 0, 10) >> 8;
				if(bus_first){
					printf("Bus %d\n", bus);
					bus_first = false;
				}
				printf("  Device %d\n    Vendor 0x%4X Device 0x%4X ", dev, vendor, device);
				if(class < sizeof(pci_classes)/sizeof(char*)){
					puts(pci_classes[class]);
				}
				putchar('\n');
			}
		}
	}
}

