/*
 *  Copyright 2020-2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_audio_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

#define FLASH_ADAPTER_SIZE 0x10000

/* Controller config
 * Supported controller list,
 * BT_THIRD_PARTY_TRANSCEIVER
 * WIFI_IW612_BOARD_RD_USD
 * WIFI_IW612_BOARD_MURATA_2EL_M2
 *
 * If third party controller used , define macro BT_THIRD_PARTY_TRANSCEIVER in following.
 * If IW612 is used, define the macro WIFI_IW612_BOARD_RD_USD in following.
 * If Murata Type 2EL module(M2 only) used , define macro WIFI_IW612_BOARD_MURATA_2EL_M2 in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW612_BOARD_MURATA_2EL_M2
/* @END_TEST_ANCHOR */

#if defined(BT_THIRD_PARTY_TRANSCEIVER) || defined(WIFI_IW612_BOARD_RD_USD) || defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

/* BIS */

/* Even we only have 1 bis stream in sink side, but the BASE have 2 bis_data.
So we have to set CONFIG_BT_BAP_BROADCAST_SNK_STREAM_COUNT to 2 or bigger to support decode BASE data.
Same as CONFIG_BT_BAP_BROADCAST_SNK_SUBGROUP_COUNT should also be same or bigger than the source. */

/* LE Audio Sync Enable. */

