/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI 2
#define CONTROLLER_ID        kUSB_ControllerEhci0

#define FLASH_ADAPTER_SIZE    0x4000
#define APP_FLEXSPI_AMBA_BASE FlexSPI1_ALIAS_BASE
/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_88W8987_BOARD_AW_CM358MA
 *
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw cm358 M.2 used, define marco WIFI_88W8987_BOARD_AW_CM358MA in following.
 */

#define WIFI_IW416_BOARD_AW_AM457_USD
/*#define WIFI_88W8987_BOARD_AW_CM358MA*/

#if defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA)
#include "bt_module_config.h"
#include "wifi_config.h"
#else
#error The Wi-Fi module is unsupported
#endif

#define CONFIG_BT_A2DP        1
#define CONFIG_BT_A2DP_SOURCE 1

#include "edgefast_bluetooth_config.h"
