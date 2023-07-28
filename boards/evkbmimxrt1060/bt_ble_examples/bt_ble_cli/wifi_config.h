/*
 * Copyright 2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef _WIFI_CONFIG_H__
#define _WIFI_CONFIG_H__

#include "app_config.h"

/* Wi-Fi boards configuration */
#if defined(WIFI_88W8977_BOARD_PAN9026_SDIO)
#define SD8977
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
/* #define SDMMCHOST_OPERATION_VOLTAGE_1V8 */
#define SD_CLOCK_MAX (25000000U)
#elif defined(WIFI_BOARD_AW_AM281SM)
#define SD8977
#elif defined(WIFI_88W8801_BOARD_AW_NM191MA)
#define SD8801
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#elif (defined(WIFI_IW416_BOARD_AW_AM457_USD) || defined(WIFI_IW416_BOARD_AW_AM457MA))
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_CLOCK_MAX (25000000U)
#elif (defined(WIFI_IW416_BOARD_MURATA_1XK_USD))
#define SD8978
#define WIFI_BT_USE_USD_INTERFACE
#define WIFI_BT_TX_PWR_LIMITS "wlan_txpwrlimit_cfg_murata_1XK_WW.h"
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x0, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }

#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_TIMING_MAX kSD_TimingSDR25HighSpeedMode

#elif (defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA))
#define SD8987
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define SD_CLOCK_MAX (25000000U)
#define CONFIG_BT_HCI_BAUDRATE (3000000U)
#elif (defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD))
#define SD8987
#define WIFI_BT_USE_USD_INTERFACE
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_CLOCK_MAX (25000000U)
#define CONFIG_BT_HCI_BAUDRATE (3000000U)
#elif defined(WIFI_IW61x_BOARD_RD_USD)
#define IW61x
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#elif defined(WIFI_IW61x_BOARD_MURATA_2EL_USD)
#define IW61x
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#define WIFI_BT_USE_USD_INTERFACE
#define WLAN_ED_MAC_CTRL                                                               \
    {                                                                                  \
        .ed_ctrl_2g = 0x1, .ed_offset_2g = 0x6, .ed_ctrl_5g = 0x1, .ed_offset_5g = 0x6 \
    }
#elif defined(WIFI_IW61x_BOARD_MURATA_2EL_M2)
#define IW61x
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define WIFI_BT_USE_M2_INTERFACE
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#else
//#error "Please define macro related to wifi board"
#endif

#define CONFIG_MAX_AP_ENTRIES 10
#define CONFIG_UAP_AMPDU_TX   1
#define CONFIG_UAP_AMPDU_RX   1
#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 1)


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

#endif /* __WIFI_CONFIG_H__ */  
