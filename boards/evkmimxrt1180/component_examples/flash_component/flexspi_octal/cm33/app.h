/*
 * Copyright 2022 NXP
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
#define EXAMPLE_FLEXSPI           FLEXSPI1
#define FLASH_SIZE                0x10000 /* 512Mb/KByte */
#define EXAMPLE_FLEXSPI_AMBA_BASE FlexSPI1_AMBA_BASE
#define FLASH_PAGE_SIZE           256
#define EXAMPLE_SECTOR            20
#define SECTOR_SIZE               0x1000 /* 4K */
#define EXAMPLE_FLEXSPI_CLOCK     kCLOCK_Flexspi1
#define FLASH_PORT                kFLEXSPI_PortA1
#define NOR_FLASH_START_ADDRESS   (8 * 1024U * 1024U)   /* 8MB */

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
