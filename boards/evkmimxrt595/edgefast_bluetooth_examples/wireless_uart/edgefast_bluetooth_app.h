/*
 * Copyright 2020-2024 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config.h"

#define OSA_USED                          1

#ifndef BOARD_USER_BUTTON_GPIO
#define BOARD_USER_BUTTON_GPIO BOARD_SW1_GPIO_PORT
#endif
#ifndef BOARD_USER_BUTTON_GPIO_PIN
#define BOARD_USER_BUTTON_GPIO_PIN (BOARD_SW1_GPIO_PIN)
#endif
#define BOARD_USER_BUTTON_IRQ  GPIO_INTA_IRQn
#define BOARD_USER_BUTTON_NAME "SW1"

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

