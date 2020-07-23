/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "noload_test.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief No-load test function
 *
 * @param None
 *
 * @return None
 */
void MID_testNoLoad(void)
{
    /* initialization */
    if (g_sMID.sNoLoad.ui16Active == FALSE)
    {
        g_sMID.sNoLoad.ui16Active            = TRUE;
        g_sMID.ui32LoopCntr                  = 0;
        g_sMID.sNoLoad.fltFMeas              = g_sMID.sPar.fltFreqN;
        g_sMID.sNoLoad.sUMeasRmpPar.fltState = 0.0F;

        /* pass speed to open loop scalar control */
        *g_sMID.sIO.pfltFreqReq = g_sMID.sNoLoad.fltFMeas;
    }

    /* wait until rotor is unlocked (set by MCAT) */
    if (g_sMID.bRotBlocked)
        return;

    /* increment loop counter */
    g_sMID.ui32LoopCntr += 1;

    /* wait for preset time */
    if (g_sMID.ui32LoopCntr < g_sMID.sNoLoad.ui32TimeMeas)
    {
        /* get actual measurement stator voltage
          (limited in scalar control fcn) and filter it at 50Hz */
        g_sMID.sNoLoad.fltUMeasMax += 0.031F * (*g_sMID.sIO.pfltUdReq - g_sMID.sNoLoad.fltUMeasMax);

        *g_sMID.sIO.pfltUdReq = GFLIB_Ramp_FLT(g_sMID.sNoLoad.fltUMeas, &g_sMID.sNoLoad.sUMeasRmpPar);

        /* start measuring the power */
        g_sMID.sCalcPwr.bCalcPwr = TRUE;
    }
    else if (g_sMID.ui32LoopCntr == g_sMID.sNoLoad.ui32TimeMeas)
    {
        /* read average measured power */
        MID_ReadPwr(&g_sMID.sCalcPwr);

        /* read measured results */
        g_sMID.sNoLoad.fltUrms = g_sMID.sCalcPwr.fltUrmsAvg;
        g_sMID.sNoLoad.fltIrms = g_sMID.sCalcPwr.fltIrmsAvg;
        g_sMID.sNoLoad.fltP    = g_sMID.sCalcPwr.fltPAvg;
        g_sMID.sNoLoad.fltQ    = g_sMID.sCalcPwr.fltQAvg;
    }
    else
    {
        *g_sMID.sIO.pfltUdReq = GFLIB_Ramp_FLT(0.0F, &g_sMID.sNoLoad.sUMeasRmpPar);
    }

    /* let the motor stop */
    if (g_sMID.ui32LoopCntr >= g_sMID.sNoLoad.ui32TimeSettle)
    {
        g_sMID.sNoLoad.ui16Active = FALSE;
        g_sMID.sCalcPwr.bCalcPwr  = FALSE;
        g_sMID.ui32LoopCntr       = 0;

        /* check if 90% of nominal voltage was reached */
        if (g_sMID.sNoLoad.fltUrms < (0.9F * g_sMID.sNoLoad.fltUMeas * 1.414213562F))
            g_sMID.ui16WarningMID |= MID_WARN_U_RMS_NOT_REACHED;
    }
}
