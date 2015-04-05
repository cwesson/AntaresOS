/**
 * @file sys/paging.c
 * Functions for controlling paging.
 * @author Conlan Wesson
 */

#include "paging.h"

#include <kernel/panic.h>
#include <stdio.h>
#include <stdint.h>
#include "../sys/interrupt/isr.h"

enum {
	PAGING_FLAG_PRESENT  = 0x0001,
	PAGING_FLAG_RW       = 0x0002,
	PAGING_FLAG_USER     = 0x0004,
	PAGING_FLAG_WTHROUGH = 0x0008,
	PAGING_FLAG_CACHEDIS = 0x0010,
	PAGING_FLAG_ACCESSED = 0x0020,
	PAGING_FLAG_DIRTY    = 0x0040,
	PAGING_FLAG_PGESIZE  = 0x0080,
	PAGING_FLAG_GLOBAL   = 0x0100
};

static uint64_t *pdpt    = (uint64_t*)0x1000;    //!< Pointer to the first Page Directory Pointer Table.
static uint64_t *pdt     = (uint64_t*)0x2000;    //!< Pointer to the first Page Directory Table.
static uint64_t *pt      = (uint64_t*)0x3000;    //!< Pointer to the first Page Table.
static uint64_t *pdttemp = (uint64_t*)0x4000;
static uint64_t *pttemp  = (uint64_t*)0x5000;


static void paging_isr(isr_regs regs){
	uint32_t addr;
	asm volatile(
		"mov %%cr2, %0"
		: "=r"(addr)
	);
	
	int present = regs.err_code & PAGING_FLAG_PRESENT;      // Page not present?
	int rw = regs.err_code & PAGING_FLAG_RW;                // Write operation?
	int user = regs.err_code & PAGING_FLAG_USER;            // Processor was in user-mode?
	int reserved = regs.err_code & PAGING_FLAG_WTHROUGH;    // Overwritten CPU-reserved bits of page entry?
	
	printf("\e[1;33mPage Fault @ 0x%X ", addr);
	if(!present){
		puts("not-present ");
	}
	if(rw){
		puts("read-only ");
	}
	if(user){
		puts("user ");
	}
	if(reserved){
		puts("reserved ");
	}
	puts("\e[0m\n");
	
	if(!present){
		uint32_t pdptindex = (addr & 0xC0000000) >> 30;
		uint32_t pdtindex  = (addr & 0x3FE00000) >> 21;
		uint32_t ptindex   = (addr & 0x001FF000) >> 12;
		uint32_t address   = (addr & 0xFFFFF000);
		
		if(pdptindex == 0){
			pdt[pdtindex] = (uint64_t)(uint32_t)&pttemp[0] | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
			pttemp[ptindex] = (uint64_t)address | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
		}else{
			pdpt[pdptindex] = (uint64_t)(uint32_t)&pdttemp[0] | PAGING_FLAG_PRESENT;
			pdttemp[pdtindex] = (uint64_t)(uint32_t)&pttemp[0] | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
			pttemp[ptindex] = (uint64_t)address | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
		}
	}else{
		panic("Page Fault");
	}
}

/**
 * Initializing paging directory and tables.
 */
void paging_init(){
	for(int i = 0; i < 512; ++i){
		pdpt[i] = 0;
		pdt[i] = 0;
		pdttemp[i] = 0;
		pttemp[i] = 0;
	}
	
	// Map the first page table
	uint64_t address = 0;
	for(int i = 0; i < 512; ++i){
		pt[i] = address | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
		address += 0x1000;
	}
	
	pdpt[0] = (uint64_t)(uint32_t)&pdt[0] | PAGING_FLAG_PRESENT;
	pdt[0] = (uint64_t)(uint32_t)&pt[0] | PAGING_FLAG_PRESENT | PAGING_FLAG_RW;
	
	// Enable PAE.
	asm volatile(
		"movl %cr4, %eax;"
		"bts $5, %eax;"
		"movl %eax, %cr4"
	);
	
	// Load the PDPT into CR3
	asm volatile(
		"movl %0, %%cr3"
		:: "r" (pdpt)
	);
	
	isr_register(14, paging_isr);
	
	// Enable paging.
	asm volatile(
		"movl %cr0, %eax;"
		"orl $0x80000000, %eax;"
		"movl %eax, %cr0;"
	);
}

