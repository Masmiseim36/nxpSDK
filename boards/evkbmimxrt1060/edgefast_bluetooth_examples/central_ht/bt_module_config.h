/*
 *  Copyright 2021 NXP
 *  All rights reserved.
 *
 *  SPDX-License-Identifier: BSD-3-Clause
 */

/* Wi-Fi boards configuration */
#if (defined(WIFI_IW416_BOARD_AW_AM457_USD))
#define SD8978
#define SDMMCHOST_OPERATION_VOLTAGE_3V3
/* #define SDMMCHOST_OPERATION_VOLTAGE_1V8 */
#define SD_CLOCK_MAX (25000000U)
#elif (defined(WIFI_88W8987_BOARD_AW_CM358_USD) || defined(WIFI_88W8987_BOARD_AW_CM358MA))
#define SD8987
/*#define SDMMCHOST_OPERATION_VOLTAGE_3V3*/
#define SDMMCHOST_OPERATION_VOLTAGE_1V8
#define SD_TIMING_MAX kSD_TimingDDR50Mode /* For 1V8 only */
#elif defined(K32W061_TRANSCEIVER)
/*
 * Wifi functions are not used with K32W061 but wifi files require to
 * be built, so stub macro are defined. Wifi functions won't be used at
 * link stage for k32w061 transceiver
 *
 */
#define SD8987
#else
#error "Please define macro related to wifi board"
#endif
