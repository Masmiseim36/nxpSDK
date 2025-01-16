/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
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

