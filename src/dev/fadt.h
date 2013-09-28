/**
 * @file dev/fadt.h
 * Fixed ACPI Description Table data structure controller.
 * @author Conlan Wesson
 */

#ifndef __INCLUDE_FADT_H_
#define __INCLUDE_FADT_H_

/**
 * Initialize the FADT.
 * @return Error code on failure, or EOK.
 */
int fadt_init();

#endif /* __INCLUDE_FADT_H_ */
