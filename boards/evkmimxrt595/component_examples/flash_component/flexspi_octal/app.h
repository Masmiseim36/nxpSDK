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
#define EXAMPLE_FLEXSPI           FLEXSPI0
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI0_AMBA_BASE
#define FLASH_SIZE                0x10000 /* 64Mb/KByte */
#define FLASH_PAGE_SIZE           256
#define NOR_FLASH_START_ADDRESS   (20U * 0x1000U)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);

/*${prototype:end}*/

#endif /* _APP_H_ */
