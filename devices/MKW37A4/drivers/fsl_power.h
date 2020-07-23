/*
 * Copyright 2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _FSL_POWER_H_
#define _FSL_POWER_H_

#include "fsl_common.h"

/*! @addtogroup power */
/*! @{ */

/*! @file */

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @name Driver version */
/*@{*/
/*! @brief POWER driver version 2.0.0. */
#define FSL_POWER_DRIVER_VERSION (MAKE_VERSION(2, 0, 0))
/*@}*/

/* Power mode definition used in application. */
typedef enum _power_mode
{
    kPWR_PowerModeMin = 'A' - 1,
    kPWR_PowerModeRun,  /* Normal RUN mode */
    kPWR_PowerModeWait, /* WAIT mode. */
    kPWR_PowerModeStop, /* STOP mode. */
    kPWR_PowerModeVlpr, /* VLPR mode. */
    kPWR_PowerModeVlpw, /* VLPW mode. */
    kPWR_PowerModeVlps, /* VLPS mode. */
#if (defined(FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_LOW_LEAKAGE_STOP_MODE)
    kPWR_PowerModeLls, /* LLS mode. */
#endif
#if (defined(FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE) && FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE)
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE0) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE0)
    kPWR_PowerModeVlls0, /* VLLS0 mode. */
#endif
    kPWR_PowerModeVlls1, /* VLLS1 mode. */
#if (defined(FSL_FEATURE_SMC_HAS_STOP_SUBMODE2) && FSL_FEATURE_SMC_HAS_STOP_SUBMODE2)
    kPWR_PowerModeVlls2, /* VLLS2 mode. */
#endif
    kPWR_PowerModeVlls3, /* VLLS3 mode. */
#endif                   /* FSL_FEATURE_SMC_HAS_VERY_LOW_LEAKAGE_STOP_MODE */
#if (defined(FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE) && FSL_FEATURE_SMC_HAS_HIGH_SPEED_RUN_MODE)
    kPWR_PowerModeHsrun, /* HighSpeed RUN mode */
#endif
    kPWR_PowerModeMax
} power_mode_t;

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/*!
 * @brief Pre enter low power mode function hook.
 *
 * This weak function allows application to call specific settings before enter low power mode
 * Please note that global interrupt is disabled when calling this function, so do not wait
 * for an interrupt in an infinite loop in this function.
 */
void POWER_PreEnterLowpowerModeHook(void);

/*!
 * @brief Post exit low power mode function hook.
 *
 * This weak function allows application to call specific settings after exit low power mode
 * Please note that global interrupt is disabled when calling this function, so do not wait
 * for an interrupt in an infinite loop in this function.
 */
void POWER_PostExitLowpowerModeHook(void);

/*
 * @brief POWER_EnterPowerMode.
 */
void POWER_EnterPowerMode(power_mode_t powerMode);

#ifdef SUPPORT_WARMBOOT
/*
 * @brief WarmbootRestore.
 */
void WarmbootRestore(void);
#endif

#if defined(__cplusplus)
}
#endif /* __cplusplus */

/*! @} */

#endif /* _FSL_POWER_H_ */
