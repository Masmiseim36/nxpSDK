/*
 * Copyright 2020-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#if defined(WIFI_IW416_BOARD_AW_AM510MA) || defined(WIFI_88W8987_BOARD_AW_CM358MA) || defined(K32W061_TRANSCEIVER) || \
    defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2) || \
    defined(WIFI_IW612_BOARD_MURATA_2EL_M2) || defined(WIFI_IW610_BOARD_MURATA_2LL_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif /*#define WIFI_IW612_BOARD_MURATA_2EL_M2*/

/*******************************************************************************
 * Low power feature config
 ******************************************************************************/

/* Controller_selection
 * List of controllers supporting Low power feature,
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_IW612_BOARD_MURATA_2EL_M2
 */
#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_IW612_BOARD_MURATA_2EL_M2)
/* Enable/Disable low power entry on tickless idle. */
#define APP_LOWPOWER_ENABLED            1
#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/* Defines the low power mode of BLE host when scanning and connecting. */
#define APP_LOW_POWER_MODE              PWR_DeepSleep
/* If low power is enabled, use BT UART to download controller firmware. */
#define CONFIG_BT_IND_DNLD              1
/* To achieve a better low power performance, optimize MEM_1V8 power domain. */
#define APP_MEM_POWER_OPT               1
/* If low power is enabled, force tickless idle enable in FreeRTOS. */
#define configUSE_TICKLESS_IDLE         1
#endif /* APP_LOWPOWER_ENABLED */
#endif /* Controller_selection */

