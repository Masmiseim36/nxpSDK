/*
 * Copyright 2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _APP_H_
#define _APP_H_

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/*${macro:start}*/
#define LPSPI_BASE                LPSPI1
#define SECTOR_SIZE               0x1000 /* 4K */
#define FLASH_PAGE_SIZE           256
#define NOR_FLASH_START_ADDRESS   (4 * 1024U)   /* Use the second sector in this example. */
#define FLASH_BAUDRATE           86000000

#define LPSPI1_PCS0_GPIO_BASE     RGPIO1
#define LPSPI1_PCS0_GPIO_PIN_NUM  (5)

/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
/*${prototype:end}*/

#endif /* _APP_H_ */
