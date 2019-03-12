/*
 * Copyright 2018-2019 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _SPECIFIC_H_
#define _SPECIFIC_H_

#include "fsl_common.h"
#include "fsl_clock.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define ARM_CORE_CURRENT_FREQ (396) /*  ARM is at 396MHz at this point */
#define Flash_Type FLEXSPI
#define HAS_SEMC_MODULE (0U)
#define HAS_WAKEUP_PIN (0U)

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

AT_QUICKACCESS_SECTION_CODE(void SwitchFlexspi2Clock(void));
AT_QUICKACCESS_SECTION_CODE(void RestoreFlexspi2Clock(void));

void SwitchToRunModePLLs(void);

void SwitchToFullRunPLLs(void);

void SwitchToLowSpeedRunPLLs(void);

void SwitchToLowPowerRunPLLs(void);

void SwitchToSystemIdlPLLs(void);

void SwitchToLPIdlPLLs(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _SPECIFIC_H_ */
