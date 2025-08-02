/*
 *  Copyright 2020-2024 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_audio_config.h"

#define FLASH_ADAPTER_SIZE 0x10000

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

