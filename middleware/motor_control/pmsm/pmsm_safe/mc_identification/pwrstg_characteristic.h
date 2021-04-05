/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef PWRSTG_CHARASTERISTIC_H_
#define PWRSTG_CHARASTERISTIC_H_

#include "mid_mc_api_connector.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* MID dead-time compensation table. Defined in mid_sm_states.c */
extern float_t g_pfltMIDUdErrorLookUp[MID_CHAR_CURRENT_POINT_NUMBERS];

/*******************************************************************************
 * API
 ******************************************************************************/
extern void MID_GetTransferCharacteristic(mid_get_char_t* sTransferCharFcn);

#ifdef __cplusplus
}
#endif

#endif /* PWRSTG_CHARASTERISTIC_H_ */
