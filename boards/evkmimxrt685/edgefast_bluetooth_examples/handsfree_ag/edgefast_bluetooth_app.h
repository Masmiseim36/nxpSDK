/*
 *  Copyright 2020-2021, 2024  NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

#include "edgefast_bluetooth_config_Gen.h"
#include "edgefast_bluetooth_debug_config_Gen.h"
#include "edgefast_bluetooth_extension_config_Gen.h"

/* Controller config
 * Supported controller list,
 * WIFI_IW416_BOARD_AW_AM510_USD
 * WIFI_IW416_BOARD_AW_AM457_USD
 * WIFI_88W8987_BOARD_AW_CM358_USD
 * WIFI_IW416_BOARD_MURATA_1XK_USD
 * WIFI_88W8987_BOARD_MURATA_1ZM_USD
 * WIFI_IW612_BOARD_MURATA_2EL_USD
 *
 * If aw am510 uSD used, define marco WIFI_IW416_BOARD_AW_AM510_USD in following.
 * If aw am457 uSD used, define marco WIFI_IW416_BOARD_AW_AM457_USD in following.
 * If aw cm358 uSD used, define marco WIFI_88W8987_BOARD_AW_CM358_USD in following.
 * If Murata Type 1XK module with uSD-M2 adapter used, define macro WIFI_IW416_BOARD_MURATA_1XK_USD in following.
 * If Murata Type 1ZM module with uSD-M2 adapter used, define macro WIFI_88W8987_BOARD_MURATA_1ZM_USD in following.
 * If Murata Type 2EL module with uSD-M2 adapter used, define macro WIFI_IW612_BOARD_MURATA_2EL_USD in following.
 */

/* @TEST_ANCHOR */
#define WIFI_IW416_BOARD_MURATA_1XK_USD
/* @END_TEST_ANCHOR */
/*#define WIFI_IW416_BOARD_AW_AM457_USD*/
/*#define WIFI_88W8987_BOARD_AW_CM358_USD*/
/*#define WIFI_IW416_BOARD_AW_AM510_USD*/
/*#define WIFI_IW416_BOARD_MURATA_1XK_USD*/
/*#define WIFI_88W8987_BOARD_MURATA_1ZM_USD*/
/*#define WIFI_IW612_BOARD_MURATA_2EL_USD*/

#if defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM457_USD) ||     \
    defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || \
    defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) || defined(WIFI_IW612_BOARD_MURATA_2EL_USD)
#include "wifi_bt_module_config.h"
#include "wifi_config.h"
#else
#error The transceiver module is unsupported
#endif

#if (defined(WIFI_IW416_BOARD_MURATA_1XK_USD) || defined(WIFI_88W8987_BOARD_MURATA_1ZM_USD) )
#undef SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingSDR25HighSpeedMode
#endif
     
#if defined(WIFI_IW612_BOARD_MURATA_2EL_USD)
#undef SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#undef SD_TIMING_MAX
#define SD_TIMING_MAX kSD_TimingDDR50Mode
#endif
      
#if (defined(WIFI_IW416_BOARD_AW_AM510_USD) || defined(WIFI_IW416_BOARD_AW_AM457_USD))
#undef SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
#endif

