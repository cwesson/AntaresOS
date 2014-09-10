/**
 * @file kernel.c
 * Main C code for the kernel.
 * @author Conlan Wesson
 */

#include "multiboot.h"

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <kernel/int.h>
#include <kernel/panic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dev/bda.h"
#include "dev/keyboard.h"
#include "dev/mouse.h"
#include "dev/pit.h"
#include "dev/ram.h"
#include "dev/rtc.h"
#include "dev/vga.h"
#include "hal/acpi.h"
#include "hal/console.h"
#include "sys/interrupt/dt.h"
#include "sys/paging.h"
#include "sys/syscall.h"
#include "tools/cpuid/cpuid.h"
#include "tools/date/date.h"
#include "tools/memmap/memmap.h"
#include "tools/pciscan/pciscan.h"

static const char *const OS_NAME     = "ConlanOS";  //!< Operating System name string.
static const char *const OS_VERSION  = "0.0.1pre";  //!< Operating System version string.
static const char *const OS_CODENAME = "Antares";   //!< Operating System code name string.
static const char *const OS_REVISION = REVISION;    //!< Operating System source revision.   

//! List of available commands.
static const char *const commands = "cpuid  date  memmap  pciscan  rand  shutdown\n";

extern uint32_t kend;                       //!< End of space used by the kernel.
#define KERNEL_SPACE ((void*)0x00200000)    //!< Maximum address allocated to the kernel.

/**
 * Main C function for the kernel.
 * @param mbd Pointer to the multiboot structure.
 * @param magic The multiboot magic number, should be 0x2BADB002.
 */
int kmain(const struct multiboot_info *const mbd, unsigned int magic){
	// Initialize VGA controller.
	vga_init();
	
	if(magic != MULTIBOOT_BOOTLOADER_MAGIC){
		// Something went wrong with multiboot.
		printf("\e[31mMagic number incorrect: %X", magic);
		return MULTIBOOT_BOOTLOADER_MAGIC;
	}
	
	void *end_kernel = (void*)(&kend);
	if(end_kernel >= KERNEL_SPACE){
		printf("\e[31mKernel exceeds allocated space: %p", end_kernel);
	}else{
		end_kernel = KERNEL_SPACE;
	}
	
	char *cmdline = (char*)mbd->cmdline;
	printf("\e[34m%s\e[0m\n", cmdline);
	
	paging_init();
	// Setup descriptor tables.
	puts("Initializing Descriptor Tables\n");
	descriptor_tables_init();
	
	// Enable system calls.
	puts("Initializing System Calls\n");
	syscall_init();
	
	// Scan memory map.
	ram_init((struct mmap_entry*)mbd->mmap_addr, mbd->mmap_length, end_kernel);
	
	// Set the interval timer to 100,000Hz.
	pit_init(100000);
	rtc_init();
	
	// Initialize the mouse and keyboard.
	keyboard_init();
	mouse_init();
	
	// Enable interrupts.
	sti();
	
	acpi_init();
	
	char *boot_loader_name = (char*)mbd->boot_loader_name;
	printf("\e[31m%s\n", boot_loader_name);
	printf("\e[32m%s %s-%s %s\e[0m\n", OS_NAME, OS_VERSION, OS_REVISION, OS_CODENAME);
	
	
	// heap_init((void*[]){end_kernel, 0}, (void*[]){end_kernel + 536870912, 0});
	// for(int i = 0; i < 3; ++i){
		// int size = rand()%1024;
		// if(size < 0){
			// size = -size;
		// }
		// int *ptr = (int*)malloc(size);
		// printf("%d bytes @ 0x%x\n", size, ptr);
	// }
	
	memmap_print();
	
	// Begin interactive shell.
	puts("Beginning Shell\n");
	char str[100];
	while(1){
		puts("\e[0m> ");
		console_desc.flush();
		gets((char*)&str);
		if(!strcmp((char*)&str, "cmd")){
			puts(commands);
		}else if(!strcmp((char*)&str, "cpuid")){
			cpuid_run();
		}else if(!strcmp((char*)&str, "date")){
			date_print();
		}else if(!strcmp((char*)&str, "memmap")){
			memmap_print();
		}else if(!strcmp((char*)&str, "pciscan")){
			pciscan_run();
		}else if(!strcmp((char*)&str, "rand")){
			putint(rand());
			putchar('\n');
		}else if(!strcmp((char*)&str, "shutdown")){
			break;
		}else if(strcmp((char*)&str, "")){
			perror("\e[31;40mUnkown Command: ");
			perror((char*)&str);
			perror("\e[0m\n");
		}
	}
	
	acpi_shutdown();
	
	perror("\e[31;40mHalt");
	return EXIT_SUCCESS;
}

