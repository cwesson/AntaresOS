/**
 * @file hal/acpi.c
 * Advanced Configuration and Power Interface controller.
 * @author Conlan Wesson
 */

#include "acpi.h"

#include <errno.h>
#include <kernel/ioport.h>
#include <kernel/panic.h>
#include <stdio.h>
#include "../dev/fadt.h"
#include "../dev/sdt.h"

const struct fadt *fadt;

/**
 * Initialize the ACPI controller.
 * @return Error code or EOK on success.
 */
int acpi_init(){
	// Locate RSDT.
	puts("Locating RSDT\n");
	int ret = sdt_init();
	if(ret != EOK){
		panic("RSDP not found!");
	}
	
	// Locate FADT.
	puts("Locating FADT\n");
	fadt = (struct fadt*)sdt_desc.bread((unsigned int)fadt_sig);
	if(fadt == NULL){
		panic("FADT not found!");
	}
	
	if(!(fadt->smi == 0 && fadt->enable == 0 && fadt->disable == 0)){
		outb(fadt->smi, fadt->enable);
	}
	
	return ret;
}

/**
 * Shutdown the system.
 * @return Error code, or no return if successful.
 */
int acpi_shutdown(){
	//outw(fadt->PM1aControlBlock, SLP_TYPa | 1<<13 );
	return ECANCELED;
}

