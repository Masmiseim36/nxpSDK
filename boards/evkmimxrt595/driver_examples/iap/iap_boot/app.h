/*
 * Copyright 2020 NXP
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
#define EXAMPLE_BOOT_INTERFACES        "0: USART 2: SPI 3: USB HID 4:FlexSPI 6:SD 7:MMC"
#define EXAMPLE_BOOT_INTERFACE_FLEXSPI (4U)
#define EXAMPLE_BOOT_INTERFACE_SD      (6U)
#define EXAMPLE_BOOT_INTERFACE_MMC     (7U)
/*${macro:end}*/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/*${prototype:start}*/
void BOARD_InitHardware(void);
bool EXAMPLE_IsValidInterface(uint8_t idx);
/*${prototype:end}*/

#endif /* _APP_H_ */
