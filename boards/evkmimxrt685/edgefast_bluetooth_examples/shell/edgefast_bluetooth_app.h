/*
 *  Copyright 2020-2021, 2024  NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#if defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM457_USD) ||     \
    defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || \
    defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined(WIFI_IW612_BOARD_MURATA_2EL_USD)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_USD)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif

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

