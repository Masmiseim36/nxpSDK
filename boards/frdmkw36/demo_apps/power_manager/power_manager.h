/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * 
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _POWER_MANAGER_H_
#define _POWER_MANAGER_H_

#include "fsl_common.h"
#include "fsl_notifier.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/* Power mode definition used in application. */
typedef enum _app_power_mode
{
    kAPP_PowerModeMin = 'A' - 1,
    kAPP_PowerModeRun,  /*!< Run mode. All Kinetis chips. */
    kAPP_PowerModeWait, /*!< Wait mode. All Kinetis chips. */
    kAPP_PowerModeStop, /*!< Stop mode. All Kinetis chips. */
    kAPP_PowerModeVlpr, /*!< Very low power run mode. All Kinetis chips. */
    kAPP_PowerModeVlpw, /*!< Very low power wait mode. All Kinetis chips. */
    kAPP_PowerModeVlps, /*!< Very low power stop mode. All Kinetis chips. */
#if defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE
    kAPP_PowerModeLls, /*!< Low leakage stop mode. All Kinetis chips. */
#endif                 /* FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE */
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0
    kAPP_PowerModeVlls0, /*!< Very low leakage stop 0 mode. Chip-specific. */
#endif                   /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE0 */
    kAPP_PowerModeVlls1, /*!< Very low leakage stop 1 mode. All Kinetis chips. */
#if defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2
    kAPP_PowerModeVlls2, /*!< Very low leakage stop 2 mode. All Kinetis chips. */
#endif                   /* FSL_FEATURE_SMC_HAS_STOP_SUBMODE2 */
    kAPP_PowerModeVlls3, /*!< Very low leakage stop 3 mode. All Kinetis chips. */
#endif /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#if defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE
    kAPP_PowerModeHsrun, /*!< High-speed run mode. Chip-specific. */
#endif                   /* FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE */
    kAPP_PowerModeMax
} app_power_mode_t;

/*!
 * @brief Power mode user configuration structure.
 *
 * This structure defines Kinetis power mode with additional power options and specifies
 * transition to and out of this mode. Application may define multiple power modes and
 * switch between them.
 * List of power mode configuration structure members depends on power options available
 * for specific chip. Complete list contains:
 *  mode - Kinetis power mode. List of available modes is chip-specific. See power_manager_modes_t
 *   list of modes. This item is common for all Kinetis chips.
 *  enableLowPowerWakeUpOnInterrupt - When set to true, system exits to Run mode when any interrupt occurs while in
 *   Very low power run, Very low power wait or Very low power stop mode. This item is chip-specific.
 *  enablePowerOnResetDetection - When set to true, power on reset detection circuit is disabled in
 *   Very low leakage stop 0 mode. When set to false, circuit is enabled. This item is chip-specific.
 *  enableRAM2Powered - When set to true, RAM2 partition content is retained through Very low
 *   leakage stop 2 mode. When set to false, RAM2 partition power is disabled and memory content lost.
 *   This item is chip-specific.
 *  enableLowPowerOscillator - When set to true, the 1 kHz Low power oscillator is disabled in any
 *   Low leakage or Very low leakage stop mode. When set to false, oscillator is enabled in these modes.
 *   This item is chip-specific.
 */
typedef struct power_user_config
{
    app_power_mode_t mode; /*!< Power mode. */

#if defined(FSL_FEATURE_SMC_HAS_LPWUI) && FSL_FEATURE_SMC_HAS_LPWUI
    bool enableLowPowerWakeUpOnInterrupt; /*!< true - system exits to Run mode on interrupt from Very low power run,
                                             Very low power wait or Very low power stop mode, false - Wake-up on
                                             interrupt disabled for these modes. */
#endif                                    /* FSL_FEATURE_SMC_HAS_LPWUI */

#if defined(FSL_FEATURE_SMC_HAS_PORPO) && FSL_FEATURE_SMC_HAS_PORPO
    bool enablePorDetectInVlls0; /*!< true - Power on reset detection circuit is enabled in Very low leakage stop 0
                                    mode, false - Power on reset detection circuit is disabled. */
#endif                           /* FSL_FEATURE_SMC_HAS_PORPO */

#if defined(FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION) && FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION
    bool enableRam2InVlls2; /*!< true - RAM2 partition content is retained through Very low leakage stop 2 mode, false -
                               RAM2 partition power is disabled and memory content lost. */
#endif                      /* FSL_FEATURE_SMC_HAS_RAM2_POWER_OPTION */

#if defined(FSL_FEATURE_SMC_HAS_LPOPO) && FSL_FEATURE_SMC_HAS_LPOPO
    bool enableLpoClock; /*!< true - The 1 kHz Low power oscillator is enabled in any Low leakage or Very low leakage
                            stop mode - false, oscillator is disabled in these modes. */
#endif                   /* FSL_FEATURE_SMC_HAS_LPOPO */
} power_user_config_t;

/* callback data type which is used for power manager user callback */
typedef struct
{
    uint32_t beforeNotificationCounter; /*!< Callback before notification counter */
    uint32_t afterNotificationCounter;  /*!< Callback after notification counter */
    smc_power_state_t originPowerState; /*!< Origin power state before switch */
} user_callback_data_t;

typedef enum _app_wakeup_source
{
    kAPP_WakeupSourceLptmr, /*!< Wakeup by LPTMR.        */
    kAPP_WakeupSourcePin    /*!< Wakeup by external pin. */
} app_wakeup_source_t;

#endif /* _POWER_MANAGER_H_ */
