/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI (2U)
#define CONTROLLER_ID        kUSB_ControllerEhci0

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM510MA
 * WIFI_88W8987_BOARD_AW_CM358MA
 * WIFI_IW416_BOARD_AW_AM510_USD
 * WIFI_88W8987_BOARD_AW_CM358_USD
 * WIFI_IW416_BOARD_MURATA_1XK_USD
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_88W8987_BOARD_MURATA_1ZM_USD
 * WIFI_88W8987_BOARD_MURATA_1ZM_M2
 * K32W061_TRANSCEIVER
 *
 * If aw am510 ma used, define marco WIFI_IW416_BOARD_AW_AM510MA in following.
 * If aw cm358 ma used, define marco WIFI_88W8987_BOARD_AW_CM358MA in following.
 * If aw am510 uSD used, define marco WIFI_IW416_BOARD_AW_AM510_USD in following.
 * If aw cm358 uSD used, define marco WIFI_88W8987_BOARD_AW_CM358_USD in following.
 * If Embedded Artists Type 1XK with uSD-M2 adapter used, define macro WIFI_IW416_BOARD_MURATA_1XK_USD in following.
 * If Embedded Artists Type 1XK(EAR00385 M2 only) used, define macro WIFI_IW416_BOARD_MURATA_1XK_M2 in following.
 * If Embedded Artists Type 1ZM with uSD-M2 adapter used, define macro WIFI_88W8987_BOARD_MURATA_1ZM_USD in following.
 * If Embedded Artists Type 1ZM(EAR00364 M2 only) used , define macro WIFI_88W8987_BOARD_MURATA_1ZM_M2 in following.
 * If transceiver k32w061 is used, define macro K32W061_TRANSCEIVER in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_MURATA_1XK_USD
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_AW_AM510_USD*/
/*#define WIFI_88W8987_BOARD_AW_CM358_USD*/
/*#define WIFI_IW416_BOARD_AW_AM510MA*/
/*#define WIFI_88W8987_BOARD_AW_CM358MA*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_USD*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_M2*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_USD*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_M2*/
/*#define K32W061_TRANSCEIVER*/

#if defined(WIFI_IW416_BOARD_AW_AM510MA) || defined(WIFI_88W8987_BOARD_AW_CM358MA) || defined(K32W061_TRANSCEIVER) || \
    defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_88W8987_BOARD_AW_CM358_USD) ||                             \
    defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_M2) ||                            \
    defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

/* Select witch beacon application to start */
#define BEACON_APP  1
#define IBEACON_APP 0
#define EDDYSTONE   0

#define CONFIG_BT_PERIPHERAL 1
#if (defined EDDYSTONE) && (EDDYSTONE)
#define CONFIG_BT_SETTINGS              1
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 1
#endif
#if defined(IBEACON_APP) && (IBEACON_APP == 1)
#define CONFIG_BT_DEVICE_NAME "ibeacon"
#elif defined(EDDYSTONE) && (EDDYSTONE == 1)
#define CONFIG_BT_DEVICE_NAME "eddystone"
#elif defined(BEACON_APP) && (BEACON_APP == 1)
#define CONFIG_BT_DEVICE_NAME "beacon"
#endif
#include "edgefast_bluetooth_config.h"
