/*
 *  Copyright 2020-2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

/* Wi-Fi boards configuration */
#if defined(WIFI_BOARD_PAN9026_SDIO)
#define SD8977
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_CLOCK_MAX (25000000U)
#elif defined(WIFI_BOARD_AW_AM281SM)
#define SD8977
#elif defined(WIFI_BOARD_AW_NM191)
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#elif defined(WIFI_BOARD_AW_AM457)
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#elif defined(WIFI_BOARD_AW_CM358)
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#else
#error "Please define macro related to wifi board"
#endif

#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 1)

#define CONFIG_MAX_AP_ENTRIES 10

#if defined(SD8977) || defined(SD8978) || defined(SD8987)
#define CONFIG_5GHz_SUPPORT 1
#endif

#if defined(SD8987)
#define CONFIG_11AC
#undef CONFIG_WMM
#endif

/* Logs */
#define CONFIG_ENABLE_ERROR_LOGS   1
#define CONFIG_ENABLE_WARNING_LOGS 1

/* WLCMGR debug */
#undef CONFIG_WLCMGR_DEBUG

/*
 * Wifi extra debug options
 */
#undef CONFIG_WIFI_EXTRA_DEBUG
#undef CONFIG_WIFI_EVENTS_DEBUG
#undef CONFIG_WIFI_CMD_RESP_DEBUG
#undef CONFIG_WIFI_SCAN_DEBUG
#undef CONFIG_WIFI_IO_INFO_DUMP
#undef CONFIG_WIFI_IO_DEBUG
#undef CONFIG_WIFI_IO_DUMP
#undef CONFIG_WIFI_MEM_DEBUG
#undef CONFIG_WIFI_AMPDU_DEBUG
#undef CONFIG_WIFI_TIMER_DEBUG
#undef CONFIG_WIFI_SDIO_DEBUG
#undef CONFIG_WIFI_FW_DEBUG

#endif /* _WIFI_CONFIG_H_ */
