/**
 * @file dev/sdt.c
 * System Description Table data structure controller.
 * @author Conlan Wesson
 */

#include "sdt.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "bda.h"

//! Structure for using the Root System Description Pointer.
struct rsdp_descriptor{
	char signature[8];    //!< Must contain "RSD PTR ".
	uint8_t checksum;       //!< Checksum byte for rev. 1.0.
	char oemid[6];        //!< OEM identifier string.
	uint8_t revision;       //!< RDST revision number.
	uint32_t rsdt_addr;     //!< Address of the RDST.
	
	// The following are only valid for rev. 2.0 and up.
	uint32_t length;        //!< Length of the table.
	uint64_t xsdt_addr;     //!< Address of the XDST.
	uint8_t xchecksum;      //!< Checksum byte for rev. 2.0.
	uint8_t _reserved[3];   //!< Ignored.
};

struct rsdt_entry{
	struct sdt_header header;
	uint32_t sdt[];
};

struct xsdt_entry{
	struct sdt_header header;
	uint64_t sdt[];
};

static struct rsdp_descriptor *rsdp = 0;    //!< RSDP pointer.
static struct rsdt_entry *rsdt = 0;         //!< RSDT pointer.
static struct xsdt_entry *xsdt = 0;         //!< XSDT pointer.

//! System Description Table device descriptor.
device_descriptor sdt_desc = {
	"System Description Table",
	DEVICE_FLAG_IN | DEVICE_FLAG_BLOCK | DEVICE_FLAG_PHYSICAL,
	0, 0,
	0, 0,
	sdt_read, 0,
	0, 0, 0
};

/**
 * Initializes the SDT for use.
 * @return Error code on failure, or EOK.
 */
int sdt_init(){
	// Find the RSDP.
	char *scan = (char*)(bda_desc.bread(BDA_EBDA_BASE) << 4);
	char *end = scan + 1024;
	bool found = false;
	for(int i = 0; i < 2; ++i){
		while(scan < end && !found){
			if(strncmp(scan, "RSD PTR ", 8) == 0){
				rsdp = (struct rsdp_descriptor*)scan;
				found = true;
			}else{
				scan += 16;    // Signature is garunteed to be on a 16byte boundry.
			}
		}
		if(!found){
			scan = (char*)0x000E0000;
			end = (char*)0x000FFFFF;
		}
	}
	if(!found){
		return ENXIO;
	}
	
	// Validate the rev. 1.0 checksum.
	uint8_t sum = 0;
	for(int i = 0; i < 20; ++i){
		sum += scan[i];
	}
	if(sum){
		return EFAULT;
	}
	
	rsdt = (struct rsdt_entry*)rsdp->rsdt_addr;
	
	// Check revision and validate the rev. 2.0 checksum.
	if(rsdp->revision > 0){
		sum = 0;
		for(int i = 0; i < 34; ++i){
			sum += (int)scan[i];
		}
		if(sum){
			return EFAULT;
		}
		xsdt = (struct xsdt_entry*)(uint32_t)rsdp->xsdt_addr;
	}
	
	return EOK;
}

/**
 * Read from the System Description Table.
 * @param addr The address to read from.
 * @return The value read.
 */
int sdt_read(unsigned int addr){
	int entries;
	if(xsdt){
		entries = (xsdt->header.length - sizeof(xsdt->header)) / 8;
	}else{
		entries = (rsdt->header.length - sizeof(rsdt->header)) / 4;
	}
	
	for(int i = 0; i < entries; ++i){
		sdt_header *head = 0;
		if(xsdt){
			head = (sdt_header*)(uint32_t)xsdt->sdt[i];
		}else{
			head = (sdt_header*)rsdt->sdt[i];
		}
		if(strncmp((char*)&head->signature, (char*)addr, 4) == 0){
			return (int)head;
		}
	}
	
	return 0;
}

