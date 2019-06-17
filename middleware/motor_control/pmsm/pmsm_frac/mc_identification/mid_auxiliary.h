/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef MID_AUXILIARY_H_
#define MID_AUXILIARY_H_

#include "mlib_types.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief MID Align function input structure */
typedef struct
{
    uint16_t ui16Active; /* Inidicates whether Rs is being measured (true) or not (false) */
    uint16_t ui16LoopCounter; /* Serves for timing to determine e.g. 600ms */
    frac16_t* pf16IdReq; /* Pointer to required current Id (input to controllers) */
    frac16_t f16CurrentAlign; /* Alignment current */
    uint16_t ui16AlignDuration; /* Alignment duration */
}mid_align_a1_t;

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

extern void MID_alignment(mid_align_a1_t* sAlignmentFcn);

#ifdef __cplusplus
}
#endif

#endif /* MID_AUXILIARY_H_ */

