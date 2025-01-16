/*
 * Copyright 2021 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_
/*${header:start}*/
#include "fsl_iap.h"
/*${header:end}*/
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define EXAMPLE_NOR_INSTANCE       0U        /* FLEXSPI */
#define NOR_FLASH_OP_START_ADDRESS 0x200000U /* Operation on 2MB offset */
#define NOR_FLASH_OP_SIZE          0x2000U   /* Test 8KB region */
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
AT_QUICKACCESS_SECTION_CODE(status_t Demo_FlexspiNorInit(uint32_t instance, flexspi_nor_config_t *config));

/*${prototype:end}*/

#endif /* _APP_H_ */
