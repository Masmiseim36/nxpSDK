/*
 * Copyright 2020-2023 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#define USB_HOST_CONFIG_IP3516HS (1U)
#define CONTROLLER_ID            kUSB_ControllerIp3516Hs0

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM510MA
 * WIFI_88W8987_BOARD_AW_CM358MA
 * WIFI_IW416_BOARD_MURATA_1XK_M2
 * WIFI_88W8987_BOARD_MURATA_1ZM_M2
 * K32W061_TRANSCEIVER
 *
 * If WIFI_IW416_BOARD_AW_AM510MA used, define marco WIFI_IW416_BOARD_AW_AM510MA in following.
 * If WIFI_88W8987_BOARD_AW_CM358MA used, define marco WIFI_88W8987_BOARD_AW_CM358MA in following.
 * If Murata Type 1XK module(EAR00385 M2 only) used, define macro WIFI_IW416_BOARD_MURATA_1XK_M2 in following.
 * If Murata Type 1ZM module(EAR00364 M2 only) used , define macro WIFI_88W8987_BOARD_MURATA_1ZM_M2 in following.
 * If transceiver k32w061 is used, the macro K32W061_TRANSCEIVER should be defined
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_MURATA_1XK_M2
/* @END_TEST_ANCHOR */
/*#define WIFI_88W8987_BOARD_AW_CM358MA*/
/*#define WIFI_IW416_BOARD_AW_AM510MA*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_M2*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_M2*/
/*#define K32W061_TRANSCEIVER*/

#if defined(WIFI_IW416_BOARD_AW_AM510MA) || defined(WIFI_88W8987_BOARD_AW_CM358MA) || defined(K32W061_TRANSCEIVER) || \
    defined(WIFI_IW416_BOARD_MURATA_1XK_M2) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_M2)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#define CONFIG_BT_DEVICE_NAME           "central_ht"
#define CONFIG_BT_PHY_UPDATE            1
#define CONFIG_BT_AUTO_PHY_UPDATE       1
#define CONFIG_BT_DATA_LEN_UPDATE       1
#define CONFIG_BT_AUTO_DATA_LEN_UPDATE  1
#define CONFIG_BT_SMP                   1
#define CONFIG_BT_SETTINGS              1
#define CONFIG_BT_HOST_CRYPTO           1
#define CONFIG_BT_KEYS_OVERWRITE_OLDEST 1
#define CONFIG_BT_RX_STACK_SIZE         2200

/*******************************************************************************
 * Low power feature config
 ******************************************************************************/

/* List of controllers supporting Low power feature,
 * WIFI_IW416_BOARD_MURATA_1XK_M2 */
#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
/* Enable/Disable low power entry on tickless idle. */
#define APP_LOWPOWER_ENABLED            1
#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/* If low power is enabled, use BT UART to download controller firmware. */
#define CONFIG_BT_IND_DNLD              1
/* To achieve a better low power performance, optimize MEM_1V8 power domain. */
#define APP_MEM_POWER_OPT               1
/* If low power is enabled, force tickless idle enable in FreeRTOS. */
#define configUSE_TICKLESS_IDLE         1
#endif
#endif

#if defined(APP_LOWPOWER_ENABLED) && (APP_LOWPOWER_ENABLED > 0)
/* Controller_selection */
#if defined(WIFI_IW416_BOARD_MURATA_1XK_M2)
/* External Wakeup schemems - 6 wires
 * Define additional Wakeup lines */
/* C2H for controller to wakeup host */
#define APP_C2H_GPIO         GPIO
#define APP_C2H_PORT         (0U)
#define APP_C2H_PIN          (17U)
#define APP_C2H_IRQ          GPIO_INTA_IRQn
/* Default BT_C2H_GPIO is for 1XK ver A.
 * If 1XK ver PA2 is used, please change it to (14U). */
#define BT_C2H_GPIO          (4U)

/* H2C for host to wakeup controller */
#define APP_H2C_GPIO         GPIO
#define APP_H2C_PORT         (1U)
#define APP_H2C_PIN          (3U)
#define BT_H2C_GPIO          (12U)
#endif /* Controller_selection */
#endif /* APP_LOWPOWER_ENABLED */

#include "edgefast_bluetooth_config.h"
#include "edgefast_bluetooth_debug_config.h"