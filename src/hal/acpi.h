/**
 * @file hal/acpi.h
 * Advanced Configuration and Power Interface controller.
 * @author Conlan Wesson
 */

#ifndef __HAL_ACPI_H_
#define __HAL_ACPI_H_

/**
 * Initialize the ACPI controller.
 * @return Error code or EOK on success.
 */
int acpi_init();

/**
 * Shutdown the system.
 * @return Error code, or no return if successful.
 */
int acpi_shutdown();

#endif /* __HAL_ACPI_H_ */

