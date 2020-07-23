/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mid_auxiliary.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief MID one-step Current alignment
 *
 * @param *sMIDRs_align
 *
 * @return None
 */
void MID_alignment(mid_align_t *sAlignmentFcn)
{
    /* if alignment hasn't started, set the duration of the alignment process */
    if (sAlignmentFcn->ui16Active == FALSE)
    {
        sAlignmentFcn->ui16LoopCounter = sAlignmentFcn->ui16AlignDuration;
        sAlignmentFcn->ui16Active      = TRUE;
    }

    /* decrement alignment timer/counter */
    sAlignmentFcn->ui16LoopCounter--;

    /* single position alignment */
    if (sAlignmentFcn->ui16LoopCounter > 0)
    {
        /* require d-axis voltage for an alignment */
        *(sAlignmentFcn->pfltIdReq) = sAlignmentFcn->fltCurrentAlign;
    }
    else
    {
        /* after defined time period set required d-axis current to zero */
        *(sAlignmentFcn->pfltIdReq) = 0.0;
        sAlignmentFcn->ui16Active   = FALSE;
    }
}
