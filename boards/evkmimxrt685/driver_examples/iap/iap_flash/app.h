/*
 * Copyright 2019 NXP
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

/* Predefined flash option */
#define EXAMPLE_NOR_FLASH_MXIC_OSPI      0xC1503051U /* Macronix OctalSPI flash */
#define EXAMPLE_NOR_FLASH_MXIC_OSPI_OPT1 0x20000014U

#define EXAMPLE_NOR_FLASH         EXAMPLE_NOR_FLASH_MXIC_OSPI /* MXIC flash on EVK board */
#define EXAMPLE_NOR_FLASH_OPTION1 EXAMPLE_NOR_FLASH_MXIC_OSPI_OPT1

#define FLASH_NEED_RESET 1
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
AT_QUICKACCESS_SECTION_CODE(status_t BOARD_FlexspiInit(uint32_t instance,
                                                       flexspi_nor_config_t *config,
                                                       serial_nor_config_option_t *option));

/*${prototype:end}*/

#endif /* _APP_H_ */
