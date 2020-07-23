/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pwrstg_characteristic.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Transfer characteristic measurement function
 *
 * @param None
 *
 * @return None
 */
void MID_GetTrfChar(void)
{
    /* initialization */
    if (g_sMID.sPwrStgCh.ui16Active == 0)
    {
        g_sMID.sPwrStgCh.ui16Active     = TRUE;
        g_sMID.ui32LoopCntr             = 0;
        g_sMID.sPwrStgCh.fltIReqAct     = -g_sMID.sPwrStgCh.fltICal;
        *g_sMID.sIO.pfltIdReq           = g_sMID.sPwrStgCh.fltIReqAct;
        *g_sMID.sIO.pfltIqReq           = 0.0F;
        g_sMID.sPwrStgCh.ui16LUTId      = 0;
        g_sMID.sPwrStgCh.pfltUErrLUT[0] = 0.0F;
    }

    /* increment loop counter */
    g_sMID.ui32LoopCntr += 1;

    /* calculate and filter error voltage */
    g_sMID.sPwrStgCh.pfltUErrLUT[g_sMID.sPwrStgCh.ui16LUTId] +=
        MID_PWRCHR_FILTCOEFF * (*g_sMID.sIO.pfltUdReq - (*g_sMID.sIO.pfltId) * g_sMID.sPwrStgCh.fltRs -
                                g_sMID.sPwrStgCh.pfltUErrLUT[g_sMID.sPwrStgCh.ui16LUTId]);

    /* after 300ms settling of Id start calculation */
    if (g_sMID.ui32LoopCntr >= (MID_PWRCHR_MEASTIME * M1_MID_TIME_ONESEC))
    {
        /* check if Rs is low enough to reach 2A */
        if ((MLIB_Abs_FLT(*g_sMID.sIO.pfltId) < (g_sMID.sPwrStgCh.fltICal - MID_RS_MIN_CURR)) &&
            (g_sMID.sPwrStgCh.ui16LUTId == 0))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_TOO_HIGH_RS;
            *g_sMID.sIO.pfltIdReq       = 0.0F;
            g_sMID.sPwrStgCh.ui16Active = FALSE;
        }

        /* check if motor is connected */
        if ((MLIB_Abs_FLT(*g_sMID.sIO.pfltId) < MID_RS_MIN_CURR) && (g_sMID.sPwrStgCh.ui16LUTId == 0))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;
            *g_sMID.sIO.pfltIdReq       = 0.0F;
            g_sMID.sPwrStgCh.ui16Active = FALSE;
        }

        /* divide error voltage by DC-bus voltage */
        g_sMID.sPwrStgCh.pfltUErrLUT[g_sMID.sPwrStgCh.ui16LUTId] /= (*g_sMID.sIO.pfltUDCbus);

        /* prepare for next point measurement */
        g_sMID.sPwrStgCh.ui16LUTId++;
        g_sMID.sPwrStgCh.fltIReqAct += g_sMID.sPwrStgCh.fltIInc;
        *g_sMID.sIO.pfltIdReq = g_sMID.sPwrStgCh.fltIReqAct;
        g_sMID.ui32LoopCntr   = 0;

        /* end after last current was measured */
        if (g_sMID.sPwrStgCh.ui16LUTId >= MID_PWRCHR_CURR_POINT_NUM)
        {
            g_sMID.sPwrStgCh.ui16Active = FALSE;
            *g_sMID.sIO.pfltIdReq       = 0.0F;
        }
        else
            g_sMID.sPwrStgCh.pfltUErrLUT[g_sMID.sPwrStgCh.ui16LUTId] = 0.0F;
    }
}
