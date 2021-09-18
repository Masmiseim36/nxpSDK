/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define BUTTON_SHORT_PRESS_THRESHOLD    500U
#define BUTTON_LONG_PRESS_THRESHOLD     800U
#define FLASH_ADAPTER_SIZE              0x4000
#define CONFIG_BT_GATT_CLIENT           1
#define BUTTON_COUNT                    1
#define CONFIG_BT_CENTRAL               1
#define CONFIG_BT_OBSERVER              1
#define DEBUG_CONSOLE_RX_ENABLE         0
#define OSA_USED                        1
#define CONFIG_BT_MAX_CONN              16
#define CONFIG_BT_L2CAP_TX_BUF_COUNT    8
#define CONFIG_BT_DIS_MODEL             "Wireless UART Demo"
#define CONFIG_BT_PERIPHERAL            1
#define CONFIG_BT_DEVICE_NAME           "NXP_WU"
#define CONFIG_BT_SMP                   1
#define CONFIG_BT_SETTINGS              1
#define CONFIG_BT_HOST_CRYPTO           1
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 1

#define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 4096

#define CONFIG_BT_SNOOP      0
#define CONFIG_BT_MAX_PAIRED 16
/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_88W8987_BOARD_AW_CM358MA
 *
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw cm358 M.2 used, define marco WIFI_88W8987_BOARD_AW_CM358MA in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_AW_AM457_USD
/* @END_TEST_ANCHOR */
/*#define WIFI_88W8987_BOARD_AW_CM358MA*/

#if defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA)
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#include "edgefast_bluetooth_config.h"
