/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRSTG_CHARASTERISTIC_H_
#define PWRSTG_CHARASTERISTIC_H_

#include "mid_def.h"

#ifdef __cplusplus
extern "C" {
#endif

extern float_t fltRs_voltage_drop; /* Auxiliary variable for Rs voltage drop calculation */
extern float_t fltUdReqFilt;       /* Filtered Ud required value */
extern float_t fltIdfbckFilt;      /* Filtered Id feedback value */

/*******************************************************************************
 * API
 ******************************************************************************/
extern void MID_GetTransferCharacteristic(mid_get_char_t *sTransferCharFcn);

#ifdef __cplusplus
}
#endif

#endif /* PWRSTG_CHARASTERISTIC_H_ */
