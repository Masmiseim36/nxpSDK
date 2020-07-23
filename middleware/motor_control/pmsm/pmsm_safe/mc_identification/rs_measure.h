/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RS_MEASURE_H_
#define RS_MEASURE_H_

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
extern void MID_getRs(mid_get_rs_t* sRsMeasFcn, mid_get_char_t* sTransferCharFcn);

#ifdef __cplusplus
}
#endif

#endif /* RS_MEASURE_H_ */
