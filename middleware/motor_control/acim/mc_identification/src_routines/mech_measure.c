/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mech_measure.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Mechanical parameter estimation function
 *
 * @param None
 *
 * @return None
 */
void MID_getMech(void)
{
    /* initialization */
    if (g_sMID.sMech.ui16Active == FALSE)
    {
        g_sMID.sMech.ui16Active = TRUE;
        g_sMID.sMech.eState     = kMID_MechInit;
        g_sMID.ui32LoopCntr     = 0;

        g_sMID.sMech.fltTrqLPF   = 0.0F;
        g_sMID.sMech.fltSpdLPF   = 0.0F;
        g_sMID.sMech.fltLPFCoeff = 0.1180886217F; /* 200 Hz filter */
        g_sMID.sMech.fltTrqInteg = 0.0F;
        g_sMID.sMech.fltSpdInteg = 0.0F;

        /* calculate required current values for PI controllers */
        *g_sMID.sIO.pfltIdReq = 0.0F;
        *g_sMID.sIO.pfltIqReq = 0.0F;

        return;
    }

    /* wait until rotor is unlocked (reset by MCAT) */
    if (g_sMID.bRotBlocked)
        return;

    /* increment loop counter */
    g_sMID.ui32LoopCntr += 1;

    /* filter speed */
    g_sMID.sMech.fltSpdLPF += g_sMID.sMech.fltLPFCoeff * ((*g_sMID.sIO.pfltSpeedEl) - g_sMID.sMech.fltSpdLPF);

    /* filter torque */
    g_sMID.sMech.fltTrqLPF += g_sMID.sMech.fltLPFCoeff * ((*g_sMID.sIO.pfltTorque) - g_sMID.sMech.fltTrqLPF);

    /* measurement state machine */
    switch (g_sMID.sMech.eState)
    {
        case kMID_MechInit:
        case kMID_MechStartUp:
            *g_sMID.sIO.pfltIdReq = g_sMID.sAlgNew.fltIdStart;

            /* wait until we reach minimal d-axis current */
            if ((*g_sMID.sIO.pfltId) > (g_sMID.sAlgNew.fltIdStart * 0.9F))
            {
                g_sMID.sMech.eState   = kMID_MechStartUp;
                *g_sMID.sIO.pfltIqReq = g_sMID.sMech.fltIqAccl;

                /* check whether we reached minimal speed  */
                if (g_sMID.sMech.fltSpdLPF > g_sMID.sMech.fltSpdElStart)
                {
                    /* go to acceleration state */
                    g_sMID.sMech.eState = kMID_MechAccelerate;
                }
            }

            /* check the timer */
            if (g_sMID.ui32LoopCntr > g_sMID.sMech.ui32TimeMeasMax)
            {
                /* assign fault code */
                g_sMID.ui16FaultMID = MID_FAULT_MECH_TIMEOUT;
                /* end measurement */
                g_sMID.sMech.eState = kMID_MechEndOfMeas;
            }
            break;

        case kMID_MechAccelerate:
            /* integrate torque for moment of inertia calculation */
            g_sMID.sMech.fltTrqInteg += M1_FAST_LOOP_TS * g_sMID.sMech.fltTrqLPF;
            g_sMID.sMech.fltSpdInteg += M1_FAST_LOOP_TS * g_sMID.sMech.fltSpdLPF;

            /* check whether we reached maximal measurement speed */
            if (g_sMID.sMech.fltSpdLPF > g_sMID.sMech.fltSpdElEnd)
            {
                /* required speed reached - produce no torque */
                *g_sMID.sIO.pfltIqReq = 0.0f;
                /* reset loop counter */
                g_sMID.ui32LoopCntr = 0;
                /* end measurement */
                g_sMID.sMech.eState = kMID_MechDecelerate;
            }

            /* check the timer */
            if (g_sMID.ui32LoopCntr > g_sMID.sMech.ui32TimeMeasMax)
            {
                /* assign fault code */
                g_sMID.ui16FaultMID = MID_FAULT_MECH_TIMEOUT;
                /* end measurement */
                g_sMID.sMech.eState = kMID_MechEndOfMeas;
            }
            break;

        case kMID_MechDecelerate:
            /* check the timer */
            if (g_sMID.ui32LoopCntr < (0.05F * M1_MID_TIME_ONESEC))
            {
                *g_sMID.sIO.pfltIdReq    = MID_MECH_ID_DECEL_PCT * g_sMID.sAlgNew.fltIdStart;
                g_sMID.sMech.fltSpdElMax = g_sMID.sMech.fltSpdLPF;
                break;
            }
            else if (g_sMID.ui32LoopCntr > g_sMID.sMech.ui32TimeSettleMax)
            {
                /* assign fault code */
                g_sMID.ui16FaultMID = MID_FAULT_MECH_TIMEOUT;
                /* end measurement */
                g_sMID.sMech.eState = kMID_MechEndOfMeas;
            }

            /* check whether the speed dropped to 63pct of maximum*/
            if (g_sMID.sMech.fltSpdLPF < (g_sMID.sMech.fltSpdElMax * 0.3678794412F))
            {
                g_sMID.sPar.fltTauMech =
                    ((float_t)(g_sMID.ui32LoopCntr - (0.05F * M1_MID_TIME_ONESEC))) * M1_FAST_LOOP_TS;
                /* end measurement */
                g_sMID.sMech.eState = kMID_MechEndOfMeas;
            }
            break;

        default:
            /* end-of-measurement state */
            /* no currents required */
            *g_sMID.sIO.pfltIdReq = 0.0F;
            *g_sMID.sIO.pfltIqReq = 0.0F;

            /* clear flag */
            g_sMID.sMech.ui16Active = FALSE;
            g_sMID.eCalcMechPar     = kMID_CalcWorking;
            break;
    }
}
