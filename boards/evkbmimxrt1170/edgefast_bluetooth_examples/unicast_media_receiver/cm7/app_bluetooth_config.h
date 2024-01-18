/*
 * Copyright 2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_EHCI (2U)
#define CONTROLLER_ID        kUSB_ControllerEhci0

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

#define CONFIG_BT_PERIPHERAL            1
#define CONFIG_BT_DEVICE_NAME           "unicast_media_receiver"
#define CONFIG_BT_DEVICE_NAME_DYNAMIC   1
#define CONFIG_BT_SMP                   1
#define CONFIG_BT_SETTINGS              0
#define CONFIG_BT_HOST_CRYPTO           1
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 0
#define CONFIG_BT_RX_STACK_SIZE         2200

#define CONFIG_BT_SMP_SC_PAIR_ONLY 1

#define CONFIG_BT_PERIPHERAL 1
#define CONFIG_BT_EXT_ADV 1

#define CONFIG_BT_ISO_UNICAST       1
#define CONFIG_BT_ISO_PERIPHERAL    1
#define CONFIG_BT_ISO_MAX_CHAN      1

#define CONFIG_BT_GATT_DYNAMIC_DB        1
#define CONFIG_BT_GATT_CACHING           1
#define CONFIG_BT_GATT_AUTO_DISCOVER_CCC 1
#define CONFIG_BT_PACS                   1
#define CONFIG_BT_ASCS                   1

/* CIS */
#define CONFIG_BT_AUDIO                1
#define CONFIG_BT_AUDIO_RX             1
#define CONFIG_BT_BAP_UNICAST          1
#define CONFIG_BT_BAP_UNICAST_SERVER   1
#define CONFIG_BT_PAC_SNK 1
#define CONFIG_BT_ASCS_ASE_SNK_COUNT   1
#define CONFIG_BT_PAC_SRC 0
#define CONFIG_BT_ASCS_ASE_SRC_COUNT   0

/* VCP */
#define CONFIG_BT_VCP_VOL_REND 1

/* MCC */
#define CONFIG_BT_MCC 1

/* Mandatory to support at least 1 for ASCS */
#define CONFIG_BT_ATT_PREPARE_COUNT    1

/* LE Audio Sync Enable. */
#define LE_AUDIO_SYNC_ENABLE 1
/* LE Audio Sync Test. */
#define LE_AUDIO_SYNC_TEST 0

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_audio_config.h"
#include "edgefast_bluetooth_debug_config.h"