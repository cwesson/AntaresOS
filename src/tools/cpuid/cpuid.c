/**
 * @file tools/cpuid/cpuid.c
 * Functions for running CPUID tests.
 * @author Conlan Wesson
 */

#include "cpuid.h"

#include <stdbool.h>
#include <stdio.h>

/**
 * Struct for returning CPUID outputs.
 */
struct cpuid_out{
	int eax;  //!< Contents of the EAX register.
	int ebx;  //!< Contents of the EBX register.
	int ecx;  //!< Contents of the ECX register.
	int edx;  //!< Contents of the EDX register.
};

/**
 * Checks if CPUID is supported by the processor.
 * @return true if supported, false otherwise.
 */
extern bool cpuid_allowed();

/**
 * Runs a CPUID instruction.
 * @param func The function code to run CPUID.
 * @return Register output of the CPUID instruction.
 */
static inline struct cpuid_out cpuid(int func){
	struct cpuid_out out;
	asm volatile(
		"cpuid;"
		:"=a"(out.eax), "=b"(out.ebx), "=c"(out.ecx), "=d"(out.edx)
		:"eax"(func)
	);
	return out;
}

/**
 * Checks the vendor of the CPU.
 * @param vendor Location to store the output string, must be at least 13bytes.
 * @return Pointer to the vendor string.
 */
char *cpuid_vendor(char *vendor){
	struct cpuid_out out = cpuid(0x00000000);
	int vend[3];
	vend[0] = out.ebx;
	vend[1] = out.edx;
	vend[2] = out.ecx;
	int i;
	for(i = 0; i < 12; ++i){
		vendor[i] = ((char*)&(vend))[i];
	}
	vendor[12] = '\0';
	return vendor;
}

/**
 * Checks the name string of the CPU.
 * @param name Location to store the output string, must be at least 48bytes.
 * @return Pointer to the name string.
 */
char *cpuid_name(char *name){
	int vend[12];
	struct cpuid_out out = cpuid(0x80000002);
	vend[0] = out.eax;
	vend[1] = out.ebx;
	vend[2] = out.ecx;
	vend[3] = out.edx;
	out = cpuid(0x80000003);
	vend[4] = out.eax;
	vend[5] = out.ebx;
	vend[6] = out.ecx;
	vend[7] = out.edx;
	out = cpuid(0x80000004);
	vend[8] = out.eax;
	vend[9] = out.ebx;
	vend[10] = out.ecx;
	vend[11] = out.edx;
	int i;
	for(i = 0; i < 48; ++i){
		name[i] = ((char*)&(vend))[i];
	}
	return name;
}

/**
 * Runs a series of CPUID checks and prints the results.
 * @return true if CPUID is supported, false otherwise.
 */
bool cpuid_run(){
	bool rtn = cpuid_allowed();
	if(rtn){
		puts("CPUID exists, running tests...\n");
		// Check the CPU vendor
		char vendor[13];
		cpuid_vendor(vendor);
		puts(vendor);
		putchar('\n');
		// Check the CPU name
		char name[48];
		cpuid_name(name);
		puts(name);
		putchar('\n');
		// Run other checks
		struct cpuid_out out = cpuid(0x00000001);
		if(out.edx & (1<<19)){
			puts("CLFLUSH\n");
		}
		if(out.edx & (1<<8)){
			puts("CMPXCHG8B\n");
		}
		if(out.ecx & (1<<13)){
			puts("CMPXCHG16B\n");
		}
		if(out.edx & (1<<15)){
			puts("CMOVcc\n");
		}
		if(out.edx & (1<<5)){
			puts("RDMSR and WRMSR\n");
		}
		if(out.edx & (1<<4)){
			puts("RDTSC\n");
		}
		if(out.edx & (1<<11)){
			puts("SYSENTER and SYSEXIT\n");
		}
		if(out.edx & (1<<0)){
			puts("x87 Legacy floating-point\n");
		}
		if(out.edx & (1<<23)){
			puts("MMX Instructions\n");
		}
		if(out.edx & (1<<25)){
			puts("SSE Instructions\n");
		}
		if(out.edx & (1<<26)){
			puts("SSE2 Instructions\n");
		}
		if(out.ecx & (1<<0)){
			puts("SSE3 Instructions\n");
		}
		if(out.ecx & (1<<9)){
			puts("SSE3 Supplemental\n");
		}
		if(out.ecx & (1<<19)){
			puts("SSE4.1 Instructions\n");
		}
		if(out.edx & (1<<9)){
			puts("APIC Enabled\n");
		}
		out = cpuid(0x80000001);
		if(out.ecx & (1<<11)){
			puts("SSE5 Instructions\n");
		}
		if(out.edx & (1<<31)){
			puts("3DNow! Instructions\n");
		}
		if(out.edx & (1<<30)){
			puts("3DNow! Extensions\n");
		}
		if(out.ecx & (1<<9)){
			puts("3DNow! Prefetch Support\n");
		}
		if(out.edx & (1<<29)){
			puts("Long Mode Available\n");
		}
	}else{
		puts("No CPUID\n");
	}
	return rtn;
}

