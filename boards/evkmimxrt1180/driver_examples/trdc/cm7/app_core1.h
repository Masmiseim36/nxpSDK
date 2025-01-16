/*
 * Copyright 2022-2023 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_CORE1_H_
#define _APP_CORE1_H_

/*${header:start}*/
#include "fsl_trdc.h"
#include "fsl_debug_console.h"
/*${header:end}*/

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_TRDC_INSTANCE          TRDC2
#define EXAMPLE_TRDC_SECURE_DOMAIN     4
#define EXAMPLE_TRDC_NONSECURE_DOMAIN  3
#define EXAMPLE_TRDC_MRC_INDEX         6 /* NETC */
#define EXAMPLE_TRDC_MRC_REGION_INDEX  0
#define EXAMPLE_TRDC_MRC_START_ADDRESS 0x60000000UL
#define EXAMPLE_TRDC_MRC_END_ADDRESS   0x60100000UL
#define EXAMPLE_TRDC_MBC_INDEX         0
#define EXAMPLE_TRDC_MBC_SLAVE_INDEX   0            /* AIPS2 */
#define EXAMPLE_TRDC_MBC_MEMORY_INDEX  0
#define EXAMPLE_TRDC_MBC_START_ADDRESS 0x42000000UL /* AIPS2 start address */
/* These flags are stored in memory for the 2 cores to communicate, should align with the primary core. */
#define EXAMPLE_NONSECURE_MRC_NEED_RESOLVE_FLAG_ADDRESS 0x04000000UL
#define EXAMPLE_NONSECURE_MRC_RESOLVED_FLAG_ADDRESS     0x04000004UL
#define EXAMPLE_NONSECURE_MBC_NEED_RESOLVE_FLAG_ADDRESS 0x04000008UL
#define EXAMPLE_NONSECURE_MBC_RESOLVED_FLAG_ADDRESS     0x0400000CUL
#define EXAMPLE_SECONDARY_CORE_START_FLAG_ADDRESS       0x04000010UL
/*${macro:end}*/
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
void APP_SetTrdcGlobalConfig(void);
void APP_SetTrdcAccessControl(void);
void APP_SetTrdcDacConfigSecureDomain(void);
void APP_SetTrdcDacConfigNonsecureDomainMbc(void);
void APP_SetTrdcDacConfigNonsecureDomainMrc(void);
void APP_SetMrcUnaccessible(uint8_t domain, bool nseEnable);
void APP_SetMbcUnaccessible(uint8_t domain, bool nseEnable);
void APP_TouchMrcMemory(void);
void APP_TouchMbcMemory(void);
void APP_CheckAndResolveMrcAccessError(trdc_domain_error_t *error);
void APP_CheckAndResolveMbcAccessError(trdc_domain_error_t *error);
/*${prototype:end}*/

#endif /* _APP_CORE1_H_ */
