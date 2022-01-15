/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
void MID_alignment(mid_align_t* sAlignmentFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;

    /* if alignment hasn't started, set the duration of the alignment process */
    if(sAlignmentFcn->bActive == FALSE)
    {
        sAlignmentFcn->ui16LoopCounter = sAlignmentFcn->ui16AlignDuration;
        sAlignmentFcn->bActive = TRUE;
    }

    /* decrement alignment timer/counter */
    sAlignmentFcn->ui16LoopCounter--;

    /* single position alignment */
    if(sAlignmentFcn->ui16LoopCounter > 0U)
    {
        /* require d-axis voltage for an alignment */
        sIDQReq.fltD = sAlignmentFcn->fltCurrentAlign;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);
    }
    else
    {
        /* after defined time period set required d-axis current to zero */
        sIDQReq.fltD = 0.0F;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);
        sAlignmentFcn->bActive = FALSE;
    }
}

