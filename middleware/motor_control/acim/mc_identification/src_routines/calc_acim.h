/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _CALC_ACIM_H_
#define _CALC_ACIM_H_

#include "mid_def.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MID_CALC_RAD2RPM (9.549296585514F)
#define MID_CALC_RPM2RAD (0.10471975512F)

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

extern void MID_CalcElPar(void);
extern void MID_CalcMechPar(void);

#ifdef __cplusplus
}
#endif

#endif /* _CALC_ACIM_H_ */
