/**
 * @file dev/fadt.h
 * Fixed ACPI Description Table data structure controller.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_FADT_H_
#define __INCLUDE_FADT_H_

#include <stdint.h>
#include "sdt.h"

struct GenericAddressStructure{
	uint8_t AddressSpace;
	uint8_t BitWidth;
	uint8_t BitOffset;
	uint8_t AccessSize;
	uint64_t Address;
};

struct fadt{
	struct sdt_header header;
	uint32_t firmwarectrl;
	uint32_t dsdt;

	// field used in ACPI 1.0; no longer in use, for compatibility only
	uint8_t  _reserved;

	uint8_t  PreferredPowerManagementProfile;
	uint16_t SCI_Interrupt;
	uint32_t smi;
	uint8_t  enable;
	uint8_t  disable;
	uint8_t  S4BIOS_REQ;
	uint8_t  PSTATE_Control;
	uint32_t PM1aEventBlock;
	uint32_t PM1bEventBlock;
	uint32_t PM1aControlBlock;
	uint32_t PM1bControlBlock;
	uint32_t PM2ControlBlock;
	uint32_t PMTimerBlock;
	uint32_t GPE0Block;
	uint32_t GPE1Block;
	uint8_t  PM1EventLength;
	uint8_t  PM1ControlLength;
	uint8_t  PM2ControlLength;
	uint8_t  PMTimerLength;
	uint8_t  GPE0Length;
	uint8_t  GPE1Length;
	uint8_t  GPE1Base;
	uint8_t  CStateControl;
	uint16_t WorstC2Latency;
	uint16_t WorstC3Latency;
	uint16_t FlushSize;
	uint16_t FlushStride;
	uint8_t  DutyOffset;
	uint8_t  DutyWidth;
	uint8_t  DayAlarm;
	uint8_t  MonthAlarm;
	uint8_t  Century;

	// reserved in ACPI 1.0; used since ACPI 2.0+
	uint16_t BootArchitectureFlags;

	uint8_t  _reserved2;
	uint32_t flags;

	// 12 byte structure; see below for details
	struct GenericAddressStructure ResetReg;

	uint8_t ResetValue;
	uint8_t _reserved3[3];

	// 64bit pointers - Available on ACPI 2.0+
	uint64_t X_FirmwareControl;
	uint64_t X_Dsdt;

	struct GenericAddressStructure X_PM1aEventBlock;
	struct GenericAddressStructure X_PM1bEventBlock;
	struct GenericAddressStructure X_PM1aControlBlock;
	struct GenericAddressStructure X_PM1bControlBlock;
	struct GenericAddressStructure X_PM2ControlBlock;
	struct GenericAddressStructure X_PMTimerBlock;
	struct GenericAddressStructure X_GPE0Block;
	struct GenericAddressStructure X_GPE1Block;
};

static char const *const fadt_sig = "FACP";

#endif /* __INCLUDE_FADT_H_ */

