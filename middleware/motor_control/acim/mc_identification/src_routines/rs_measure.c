/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rs_measure.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Rs estimation function
 *
 * @param None
 *
 * @return None
 */
void MID_getRs(void)
{
    /* initialization */
    if (g_sMID.sRs.ui16Active == FALSE)
    {
        g_sMID.sRs.ui16Active = TRUE;
        g_sMID.ui32LoopCntr   = 0;
        g_sMID.sPar.fltRs     = 0.0F;
        g_sMID.sRs.fltILPF    = 0.0F;

        /* set the measuring d-axis current */
        *g_sMID.sIO.pfltIdReq = g_sMID.sRs.fltIMeas;
        *g_sMID.sIO.pfltIqReq = 0.0F;
    }

    /* loop counter for time keeping */
    g_sMID.ui32LoopCntr += 1;

    /* filter measured current at 50Hz */
    g_sMID.sRs.fltILPF += (0.031F * ((*g_sMID.sIO.pfltId) - g_sMID.sRs.fltILPF));

    /* start calculation after measurement time passed */
    if (g_sMID.ui32LoopCntr == g_sMID.sRs.ui32TimeMeas)
    {
        /* set required d-axis current to zero */
        *g_sMID.sIO.pfltIdReq = 0.0F;

        /* calculate stator resistance */
        g_sMID.sPar.fltRs = *g_sMID.sIO.pfltUdReq / (g_sMID.sRs.fltILPF);

        /* check if motor is connected */
        if (MLIB_Abs_FLT(g_sMID.sRs.fltILPF) < MID_RS_MIN_CURR)
            g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;

        /* check if measuring current was reached */
        if (*g_sMID.sIO.pfltId < (g_sMID.sRs.fltIMeas - MID_RS_MIN_CURR))
            g_sMID.ui16WarningMID |= MID_WARN_DC_CUR_NOT_REACHED;

        /* check whether the precision of measurement was not too low */
        if ((*g_sMID.sIO.pfltUdReq < (M1_U_MAX * MID_RS_MEAS_MIN_LVL)) ||
            (g_sMID.sRs.fltILPF < (M1_I_MAX * MID_RS_MEAS_MIN_LVL)))
            g_sMID.ui16WarningMID |= MID_WARN_TOO_LOW_PREC;
    }

    /* wait additional time to settle d-axis current at zero and exit */
    if (g_sMID.ui32LoopCntr > g_sMID.sRs.ui32TimeSettle)
        g_sMID.sRs.ui16Active = FALSE;
}
