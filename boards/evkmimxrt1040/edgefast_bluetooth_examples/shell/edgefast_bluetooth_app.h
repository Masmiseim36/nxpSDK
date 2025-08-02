/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#ifdef WIFI_IW610_BOARD_MURATA_2LL_M2
#undef CONFIG_BT_CLASSIC
#undef CONFIG_BT_IND_RESET
#undef CONFIG_BT_IND_RESET_OOB
#endif /* #ifdef WIFI_IW610_BOARD_MURATA_2LL_M2 */

#ifndef CONFIG_BT_CLASSIC
#undef CONFIG_BT_A2DP
#undef CONFIG_BT_A2DP_CP_SERVICE
#undef CONFIG_BT_A2DP_DR_SERVICE
#undef CONFIG_BT_A2DP_SINK
#undef CONFIG_BT_A2DP_SOURCE
#undef CONFIG_BT_AVRCP
#undef CONFIG_BT_AVRCP_BROWSING
#undef CONFIG_BT_AVRCP_COVER_ART
#undef CONFIG_BT_AVRCP_COVER_ART_INITIATOR
#undef CONFIG_BT_AVRCP_COVER_ART_RESPONDER
#undef CONFIG_BT_AVRCP_CT
#undef CONFIG_BT_AVRCP_TG
#undef CONFIG_BT_HFP_HF
#undef CONFIG_BT_MAP
#undef CONFIG_BT_MAP_MCE
#undef CONFIG_BT_MAP_MSE
#undef CONFIG_BT_PBAP
#undef CONFIG_BT_PBAP_PCE
#undef CONFIG_BT_PBAP_PSE
#undef CONFIG_BT_RFCOMM
#endif /* #ifndef CONFIG_BT_CLASSIC */

#if 0
#undef CONFIG_BT_EXT_ADV
#define CONFIG_BT_EXT_ADV 1
#undef CONFIG_BT_PER_ADV
#define CONFIG_BT_PER_ADV 1
#endif

#if 0
#define CONFIG_BT_SMP_SELFTEST 1

#undef CONFIG_BT_DEBUG
#define CONFIG_BT_DEBUG 1

#define CONFIG_NET_BUF_LOG        1
#define CONFIG_NET_BUF_POOL_USAGE 1
#endif

#ifdef K32W061_TRANSCEIVER
#undef CONFIG_BT_CLASSIC
#endif

#define SHELL_BUFFER_SIZE 512
#define SHELL_MAX_ARGS    20

