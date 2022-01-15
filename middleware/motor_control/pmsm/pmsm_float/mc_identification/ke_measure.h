/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef KE_MEASURE_H_
#define KE_MEASURE_H_

#include "mid_mc_api_connector.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/
extern void MID_getKe(mid_get_ke_t* sKeMeasFcn);

#ifdef __cplusplus
}
#endif

#endif /* KE_MEASURE_H_ */
