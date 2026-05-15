/*
 * Copyright 2025 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _APP_H_
#define _APP_H_

/*${header:start}*/
#include "board.h"
#include "fsl_rgpio.h"
/*${header:end}*/

#define    FOE_PWD                      0x12369874

#define    MAX_FIRMWARE_NAME_SIZE       32

// MAX_FIREWARE_SIZE: Maximum file size
#define MAX_FIREWARE_SIZE               0x40000

#define MCUBOOT_IMAGE_NUMBER            1

#define BOOT_FIRST_FLASH                0x4040000

#define FIRMWARE_INFO_FLASH             0x4440000

#define FIRMWARE_INFO_OFFSET            0x440000

#define GPIO_LED     BOARD_USER_LED_GPIO

#define GPIO_LED_PIN BOARD_USER_LED_GPIO_PIN

#endif /* _APP_H_ */