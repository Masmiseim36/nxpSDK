/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_CORE0_H_
#define _APP_CORE0_H_

/*${header:start}*/
#include "fsl_trdc.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
/* Address of memory, from which the secondary core will boot */
#define CORE1_BOOT_ADDRESS    (void *)0x303C0000
#define CORE1_KICKOFF_ADDRESS 0x0
#if defined(__CC_ARM) || defined(__ARMCC_VERSION)
extern uint32_t Image$$CORE1_REGION$$Base;
extern uint32_t Image$$CORE1_REGION$$Length;
#define CORE1_IMAGE_START &Image$$CORE1_REGION$$Base
#elif defined(__ICCARM__)
#pragma section = "__core1_image"
#define CORE1_IMAGE_START __section_begin("__core1_image")
#elif (defined(__GNUC__)) && (!defined(__MCUXPRESSO))
extern const char core1_image_start[];
extern const char *core1_image_end;
extern int core1_image_size;
#define CORE1_IMAGE_START ((void *)core1_image_start)
#define CORE1_IMAGE_SIZE  ((void *)core1_image_size)
#endif

/* TRDC definition for communication memory between two cores */
#define EXAMPLE_TRDC_COMM_INSTANCE         TRDC1
#define EXAMPLE_TRDC_COMM_MRC_INDEX        1
#define EXAMPLE_TRDC_COMM_MRC_REGION_INDEX 1
#define EXAMPLE_TRDC_COMM_START_ADDR       0x04000000UL
#define EXAMPLE_TRDC_COMM_END_ADDR         0x040FFFFFUL

/* TRDC related definitions. */
#define EXAMPLE_TRDC_SECURE_DOMAIN           2
#define EXAMPLE_TRDC_NONSECURE_DOMAIN        1
#define EXAMPLE_TRDC_INSTANCE                TRDC1
#define EXAMPLE_TRDC_MRC_INDEX               1            /* FlexSPI2 */
#define EXAMPLE_TRDC_MRC_REGION_INDEX        0
#define EXAMPLE_TRDC_MRC_START_ADDRESS       0x04100000UL /* FLEXSPI2 Hyperram */
#define EXAMPLE_TRDC_MRC_END_ADDRESS         0x04200000UL
#define EXAMPLE_TRDC_MBC_INDEX               0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX         2            /* GPIO1 */
#define EXAMPLE_TRDC_MBC_MEMORY_INDEX        0
#define EXAMPLE_TRDC_MBC_START_ADDRESS       0x47400000UL /* GPIO1 start address */
#define EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE TRDC2
/* These flags are stored in memory for the 2 cores to communicate, should align with the secondary core. */
#define EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS 0x04000000UL
#define EXAMPLE_NONSECURE_MRC_RESOLVED_FLAG_ADDRESS     0x04000004UL
#define EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS 0x04000008UL
#define EXAMPLE_NONSECURE_MBC_RESOLVED_FLAG_ADDRESS     0x0400000CUL
#define EXAMPLE_SECONDARY_CORE_START_FLAG_ADDRESS       0x04000010UL
/* Secondary core definitions. */
#define EXAMPLE_TRDC_SECONDARY_CORE_INSTANCE          TRDC2
#define EXAMPLE_TRDC_SECONDARY_CORE_SECURE_DOMAIN     4
#define EXAMPLE_TRDC_SECONDARY_CORE_NONSECURE_DOMAIN  3
#define EXAMPLE_TRDC_SECONDARY_CORE_MRC_INDEX         6 /* NETC */
#define EXAMPLE_TRDC_SECONDARY_CORE_MRC_REGION_INDEX  0
#define EXAMPLE_TRDC_SECONDARY_CORE_MRC_START_ADDRESS 0x60000000UL
#define EXAMPLE_TRDC_SECONDARY_CORE_MRC_END_ADDRESS   0x60100000UL
#define EXAMPLE_TRDC_SECONDARY_CORE_MBC_INDEX         0
#define EXAMPLE_TRDC_SECONDARY_CORE_MBC_SLAVE_INDEX   0 /* AIPS2 */
#define EXAMPLE_TRDC_SECONDARY_CORE_MBC_MEMORY_INDEX  0
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
void APP_SetTrdcAccessControl(void);
void APP_SetTrdcDacConfigSecureDomain(void);
void APP_SetTrdcDacConfigNonsecureDomain(void);
void APP_SetPid(uint8_t pid);
void APP_SetMrcUnaccessible(uint8_t domain, bool nseEnable);
void APP_SetMbcUnaccessible(uint8_t domain, bool nseEnable);
void APP_TouchMrcMemory(void);
void APP_TouchMbcMemory(void);
void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error);
void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error);
void APP_ResolveSecondaryCoreMrcAccessError(void);
void APP_ResolveSecondaryCoreMbcAccessError(void);
#ifdef CORE1_IMAGE_COPY_TO_RAM
uint32_t get_core1_image_size(void);
#endif
/*${prototype:end}*/

#endif /* _APP_CORE0_H_ */
