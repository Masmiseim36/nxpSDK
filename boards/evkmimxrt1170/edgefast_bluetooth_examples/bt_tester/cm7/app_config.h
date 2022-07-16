/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI (2U)
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define FLASH_ADAPTER_SIZE        0x4000
#define CONFIG_BT_PERIPHERAL      1
#define CONFIG_BT_CENTRAL         1
#define CONFIG_BT_GATT_CLIENT     1
#define CONFIG_BT_GATT_DYNAMIC_DB 1
#define CONFIG_BT_GATT_CACHING    1

#define CONFIG_BT_SMP                        1
#define CONFIG_BT_SMP_ALLOW_UNAUTH_OVERWRITE 1
#define CONFIG_BT_SIGNING                    1
#define CONFIG_BT_BONDABLE                   1
#define CONFIG_BT_SMP_APP_PAIRING_ACCEPT     1
#define CONFIG_BT_PRIVACY                    1
#define CONFIG_BT_HOST_CRYPTO                1

#define CONFIG_BT_DEVICE_NAME_MAX            32
#define CONFIG_BT_DEVICE_NAME_DYNAMIC        1
#define CONFIG_BT_DEVICE_NAME_GATT_WRITABLE  1
#define CONFIG_BT_DEVICE_NAME                "BT_TESTER"
#define CONFIG_BT_GAP_PERIPHERAL_PREF_PARAMS 1

#define CONFIG_BT_L2CAP_DYNAMIC_CHANNEL 1
#define CONFIG_BT_L2CAP_ECRED           1
#define CONFIG_BT_EATT_MAX              5

/*
#define CONFIG_BT_SMP_SC_ONLY 1
#define CONFIG_BT_SMP_SC_PAIR_ONLY 1
#define CONFIG_BT_RPA_TIMEOUT 30
#define CONFIG_BT_CTLR_CONN_PARAM_REQ
*/

/*
#define CONFIG_BT_ATT_PREPARE_COUNT       8
#define CONFIG_BT_EATT                    1
*/

#define CONFIG_WORK_QUEUE_TASK_STACK_SIZE 4096

#define DEBUG_CONSOLE_RX_ENABLE   0
#define BOARD_DEBUG_UART_BAUDRATE (9600U)
#define SDK_DEBUGCONSOLE          DEBUGCONSOLE_DISABLE

#define OSA_USED  1
#define BT_TESTER 1

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_IW416_BOARD_AW_AM510MA
 * WIFI_88W8987_BOARD_AW_CM358MA
 * WIFI_IW416_BOARD_MURATA_1XK_USD
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_88W8987_BOARD_MURATA_1ZM_USD
 * WIFI_88W8987_BOARD_MURATA_1ZM_M2
 *
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw am510 M.2 used, define marco WIFI_IW416_BOARD_AW_AM510MA in following.
 * If aw cm358 M.2 used, define marco WIFI_88W8987_BOARD_AW_CM358MA in following.
 * If Murata Type 1XK module with uSD-M2 adapter used, define macro WIFI_IW416_BOARD_MURATA_1XK_USD in following.
 * If Murata Type 1XK module(EAR00385 M2 only) used, define macro WIFI_IW416_BOARD_MURATA_1XK_M2 in following.
 * If Murata Type 1ZM module with uSD-M2 adapter used, define macro WIFI_88W8987_BOARD_MURATA_1ZM_USD in following.
 * If Murata Type 1ZM module(EAR00364 M2 only) used , define macro WIFI_88W8987_BOARD_MURATA_1ZM_M2 in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_AW_AM510MA
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_AW_AM457_USD*/
/*#define WIFI_88W8987_BOARD_AW_CM358MA*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_USD*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_M2*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_USD*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_M2*/

#if (defined(WIFI_IW416_BOARD_AW_AM510MA) || defined(WIFI_88W8987_BOARD_AW_CM358MA) ||          \
     defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2) ||     \
     defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
     defined(WIFI_IW416_BOARD_AW_AM457_USD))
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#include "edgefast_bluetooth_config.h"
