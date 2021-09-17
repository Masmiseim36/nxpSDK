/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_88W8987_BOARD_AW_CM358_USD
 *
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw cm358 uSD used, define marco WIFI_88W8987_BOARD_AW_CM358_USD in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_AW_AM457_USD
/* @END_TEST_ANCHOR */
/*#define WIFI_88W8987_BOARD_AW_CM358_USD*/

#if defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_88W8987_BOARD_AW_CM358_USD)
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

#if defined(WIFI_88W8987_BOARD_AW_CM358_USD)
#if defined(SDMMCHOST_OPERATION_VOLTAGE_3V3)
#elif defined(SDMMCHOST_OPERATION_VOLTAGE_1V8)
#undef SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#endif
#endif

#define CONFIG_BT_RFCOMM         1
#define CONFIG_BT_HFP_HF         1
#define CONFIG_BT_DEBUG          0
#define CONFIG_BT_DEBUG_HCI_CORE 0
#define CONFIG_BT_DEBUG_CONN     0
#define CONFIG_BT_DEBUG_GATT     0
#define CONFIG_BT_DEBUG_ATT      0
#define CONFIG_BT_DEBUG_SMP      0
#define CONFIG_BT_DEBUG_KEYS     0
#define CONFIG_BT_DEBUG_L2CAP    0
#define CONFIG_BT_DEBUG_SERVICE  0
#define CONFIG_BT_DEBUG_HFP_HF   0

#include "edgefast_bluetooth_config.h"
