/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#define SERIAL_PORT_TYPE_BLE_WU            1

#define CONTROLLER_INIT_ESCAPE             1

#define FLASH_ADAPTER_SIZE                 0x10000
#define OSA_USED                           1
#define porting_errno                      1

#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO            (BOARD_INITPINSBUTTON_SW5_PORT)
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN        (BOARD_INITPINSBUTTON_SW5_PIN)
#endif
#define BOARD_USER_BUTTON_IRQ             GPIO00_IRQn
#define BOARD_USER_BUTTON_NAME            "SW5"
#define BUTTON_TASK_STACK_SIZE            (1500U)

#if (defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
     defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(WIFI_IW610_BOARD_MURATA_2LL_M2))
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

