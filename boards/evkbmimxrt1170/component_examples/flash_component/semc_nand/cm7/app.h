/*
 * Copyright 2020 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#include "fsl_cache.h"

#define EXAMPLE_SEMC                        SEMC
#define EXAMPLE_SEMC_START_ADDRESS          (0x80000000U)
#define EXAMPLE_SEMC_NAND_AXI_START_ADDRESS (0x9E000000U)
#define EXAMPLE_SEMC_NAND_IPG_START_ADDRESS (0x00000000U)
#define FLASH_PAGE_SIZE                     (2048)
#define CACHE_MAINTAIN                      (1)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
