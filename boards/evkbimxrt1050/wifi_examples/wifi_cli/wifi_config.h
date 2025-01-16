/*
 *  Copyright 2020-2022 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _WIFI_CONFIG_H_
#define _WIFI_CONFIG_H_

#include "app_config.h"
#include "wifi_bt_module_config.h"

#define CONFIG_WIFI_MAX_PRIO (configMAX_PRIORITIES - 1)
#define CONFIG_IPV6 1
#define CONFIG_MAX_IPV6_ADDRESSES 3

#define CONFIG_MAX_AP_ENTRIES 10

#define CONFIG_MAX_RESCAN_LIMIT 30

#if defined(SD8978) || defined(SD8987) || defined(RW610) || defined(SD9177)
#define CONFIG_5GHz_SUPPORT 1
#endif

#define CONFIG_SDIO_MULTI_PORT_RX_AGGR 1

#if defined(SD8987) || defined(RW610) || defined(SD9177)
#define CONFIG_11AC 1
#define CONFIG_WMM  0
#endif

#if defined(SD9177)
#define CONFIG_WMM 0
#define CONFIG_SDIO_MULTI_PORT_TX_AGGR 0
#define CONFIG_COMPRESS_TX_PWTBL 1
#define CONFIG_COMPRESS_RU_TX_PWTBL 1
#if CONFIG_11AC
#define CONFIG_11AX 1
#endif
#endif

#define PRINTF_FLOAT_ENABLE 1

#define CONFIG_IPV6               1
#define CONFIG_MAX_IPV6_ADDRESSES 3

#if defined(SD8978) || defined(SD8987) || defined(SD8801) || defined(SD9177)
#define CONFIG_WIFI_CAPA        1
#define CONFIG_ROAMING          1
#define CONFIG_CLOUD_KEEP_ALIVE 1
#define CONFIG_TURBO_MODE       1
#if defined(SD8978) || defined(SD8987)
#define CONFIG_AUTO_RECONNECT 1
#define CONFIG_WIFI_IND_DNLD 0
#define CONFIG_WIFI_IND_RESET 0
#endif

#if !defined(SD8801)
#define CONFIG_EXT_SCAN_SUPPORT 1
#define CONFIG_WIFI_EU_CRYPTO   1
#if !defined(SD9177)
#define CONFIG_11R 1
#endif
#define CONFIG_OWE 1
#endif

#define CONFIG_HOST_SLEEP 0
#define CONFIG_FIPS 0

#define CONFIG_11K 1
#define CONFIG_11V 1

#endif

/* Logs */
#define CONFIG_ENABLE_ERROR_LOGS   1
#define CONFIG_ENABLE_WARNING_LOGS 1

/* WLCMGR debug */
#define CONFIG_WLCMGR_DEBUG 0

/*
 * Wifi extra debug options
 */
#define CONFIG_WIFI_EXTRA_DEBUG 0
#define CONFIG_WIFI_EVENTS_DEBUG 0
#define CONFIG_WIFI_CMD_RESP_DEBUG 0
#define CONFIG_WIFI_PS_DEBUG 0
#define CONFIG_WIFI_PKT_DEBUG 0
#define CONFIG_WIFI_SCAN_DEBUG 0
#define CONFIG_WIFI_IO_INFO_DUMP 0
#define CONFIG_WIFI_IO_DEBUG 0
#define CONFIG_WIFI_IO_DUMP 0
#define CONFIG_WIFI_MEM_DEBUG 0
#define CONFIG_WIFI_AMPDU_DEBUG 0
#define CONFIG_WIFI_TIMER_DEBUG 0
#define CONFIG_WIFI_FW_DEBUG 0
#define CONFIG_WIFI_UAP_DEBUG 0
#define CONFIG_WPS_DEBUG 0
#define CONFIG_FW_VDLL_DEBUG 0
#define CONFIG_DHCP_SERVER_DEBUG 0
#define CONFIG_WIFI_SDIO_DEBUG 0
#define CONFIG_FWDNLD_IO_DEBUG 0

/*
 * Heap debug options
 */
#define CONFIG_HEAP_DEBUG 0
#define CONFIG_HEAP_STAT 0

#endif /* _WIFI_CONFIG_H_ */