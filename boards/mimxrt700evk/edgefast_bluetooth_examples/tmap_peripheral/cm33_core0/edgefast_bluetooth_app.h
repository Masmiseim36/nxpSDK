/*
 *  Copyright 2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_audio_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* Controller config
 * Supported controller list,
 * WIFI_IW612_BOARD_MURATA_2EL_M2
 *
 * If Murata Type 2EL module(Rev-A1 M2 only) used , define macro WIFI_IW612_BOARD_MURATA_2EL_M2 in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW612_BOARD_MURATA_2EL_M2
/* @END_TEST_ANCHOR */
/*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

#if CONFIG_BT_CSIP_SET_MEMBER
    #define CONFIG_TMAP_PERIPHERAL_DUO  0 /* Duo headset */
#endif

#if defined(CONFIG_TMAP_PERIPHERAL_DUO) && (CONFIG_TMAP_PERIPHERAL_DUO > 0)
    #define CONFIG_TMAP_PERIPHERAL_SET_RANK 1
#endif

#define CONFIG_TMAP_PERIPHERAL_LEFT  0
#define CONFIG_TMAP_PERIPHERAL_RIGHT 1

#if CONFIG_BT_BAP_UNICAST_SERVER
#undef CONFIG_BT_BAP_UNICAST
#define CONFIG_BT_BAP_UNICAST        1
#undef CONFIG_BT_ISO_UNICAST
#define CONFIG_BT_ISO_UNICAST        1
#undef CONFIG_BT_ISO_PERIPHERAL
#define CONFIG_BT_ISO_PERIPHERAL     1
#undef CONFIG_BT_GATT_CACHING
#define CONFIG_BT_GATT_CACHING       1
#undef CONFIG_BT_ASCS
#define CONFIG_BT_ASCS               1
#endif

