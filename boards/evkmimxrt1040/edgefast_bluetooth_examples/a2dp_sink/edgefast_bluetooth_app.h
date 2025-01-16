/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_88W8987_BOARD_MURATA_1ZM_M2
 *
 * If Murata Type 1XK module used, define macro WIFI_IW416_BOARD_MURATA_1XK_M2 in following.
 * If Murata Type 1ZM module used, define macro WIFI_88W8987_BOARD_MURATA_1ZM_M2 in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_MURATA_1XK_M2
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_MURATA_1XK_M2*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_M2*/

#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

