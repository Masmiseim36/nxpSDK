/*
 * Copyright 2019 NXP
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
#define EXAMPLE_NOR_INSTANCE       1U        /* FLEXSPI0 */
#define NOR_FLASH_OP_START_ADDRESS 0x200000U /* Operation on 2MB offset */
#define NOR_FLASH_OP_SIZE          0x2000U   /* Test 8KB region */

/* Predefined flash option */
#define EXAMPLE_NOR_FLASH_QSPI      0xC0000000U               /* QuadSPI flash */
#define EXAMPLE_NOR_FLASH_QSPI_DDR  0xC0100000U               /* QuadSPI DDR flash */
#define EXAMPLE_NOR_FLASH_HYPER_1V8 0xC0233001U               /* 1.8V Hyper flash */
#define EXAMPLE_NOR_FLASH_HYPER_3V0 0xC0333000U               /* 3.0V Hyper flash */
#define EXAMPLE_NOR_FLASH_MXIC_OSPI 0xC0403000U               /* Macronix OctalSPI flash */
#define EXAMPLE_NOR_FLASH_MCRN_OSPI 0xC0600000U               /* Micron OctalSPI flash */
#define EXAMPLE_NOR_FLASH_ADST_OSPI 0xC0803000U               /* Adesto OctalSPI flash */

#define EXAMPLE_NOR_FLASH         EXAMPLE_NOR_FLASH_MXIC_OSPI /* MXIC flash on EVK board */
#define EXAMPLE_NOR_FLASH_OPTION1 0U
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
