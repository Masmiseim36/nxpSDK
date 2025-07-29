/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

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

/* Select witch beacon application to start */
#define BEACON_APP                      1
#define IBEACON_APP                     0
#define EDDYSTONE                       0

#if (defined EDDYSTONE) && (EDDYSTONE)
#undef CONFIG_BT_SETTINGS
#define CONFIG_BT_SETTINGS              1
#undef CONFIG_BT_KEYS_OVERWRITE_OLDEST
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 1
#endif
#if defined(IBEACON_APP) && (IBEACON_APP == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "ibeacon"
#elif defined(EDDYSTONE) && (EDDYSTONE == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "eddystone"
#elif defined(BEACON_APP) && (BEACON_APP == 1)
#undef CONFIG_BT_DEVICE_NAME
#define CONFIG_BT_DEVICE_NAME "beacon"
#endif
