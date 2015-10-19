/**
 * @file dev/ata.c
 * Functions for handling the ATA buses.
 * @author Conlan Wesson
 */

#include "ata.h"

#include <kernel/ioport.h>
#include <stdint.h>
#include "sys/interrupt/isr.h"

#define ATA_PRI_BASE 0x1F0
#define ATA_SEC_BASE 0x170
#define ATA_DATA_OFFSET  0
#define ATA_INFO_OFFSET  1
#define ATA_COUNT_OFFSET 2
#define ATA_SECT_OFFSET  3
#define ATA_CYLHI_OFFSET 4
#define ATA_CYLLO_OFFSET 5
#define ATA_DRIVE_OFFSET 6
#define ATA_CMD_OFFSET   7
#define ATA_PRI_PORT 0x3F6
#define ATA_SEC_PORT 0x376

#define ATA_PRI_IRQ IRQ14
#define ATA_SEC_IRQ IRQ15

#define ATA_STATUS_ERR 0x01
#define ATA_STATUS_DRQ 0x08
#define ATA_STATUS_SRV 0x10
#define ATA_STATUS_DF  0x20
#define ATA_STATUS_RDY 0x40
#define ATA_STATUS_BSY 0x80

