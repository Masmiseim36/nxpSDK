/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "blocked_test.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Blocked-rotor test function
 *
 * @param None
 *
 * @return None
 */
void MID_testBlocked(void)
{
    /* initialization */
    if (g_sMID.sBlocked.ui16Active == FALSE)
    {
        g_sMID.sBlocked.ui16Active             = TRUE;
        g_sMID.ui32LoopCntr                    = 0;
        g_sMID.sBlocked.fltFMeas               = g_sMID.sPar.fltFreqN;
        g_sMID.sBlocked.fltUMeas               = 0.0F;
        g_sMID.sBlocked.sIrmsPIPar.fltInErrK_1 = 0.0F;
        g_sMID.sBlocked.sIrmsPIPar.fltIAccK_1  = 0.0F;
        /* calculate power in slow loop */
        g_sMID.sCalcPwr.bCalcPwr = TRUE;
    }

    /* wait until rotor is locked (set by MCAT) */
    if (!g_sMID.bRotBlocked)
        return;

    /* pass speed to open loop scalar control */
    *g_sMID.sIO.pfltFreqReq = g_sMID.sBlocked.fltFMeas;

    /* pass required voltage to open loop scalar control */
    *g_sMID.sIO.pfltUdReq = g_sMID.sBlocked.fltUMeas;

    /* increment loop counter */
    g_sMID.ui32LoopCntr += 1;

    /* finish blocked-rotor test after 4.8 s */
    if (g_sMID.ui32LoopCntr >= g_sMID.sBlocked.ui32TimeMeas)
    {
        /* read measured power */
        MID_ReadPwr(&g_sMID.sCalcPwr);

        /* store measured results */
        g_sMID.sBlocked.fltUrms = g_sMID.sCalcPwr.fltUrmsAvg;
        g_sMID.sBlocked.fltIrms = g_sMID.sCalcPwr.fltIrmsAvg;
        g_sMID.sBlocked.fltP    = g_sMID.sCalcPwr.fltPAvg;
        g_sMID.sBlocked.fltQ    = g_sMID.sCalcPwr.fltQAvg;

        /* check if 90% of nominal rms current was reached */
        if (g_sMID.sBlocked.fltIrms < (0.9F * g_sMID.sBlocked.fltIrmsMeas))
            g_sMID.ui16WarningMID |= MID_WARN_I_RMS_NOT_REACHED;

        g_sMID.sBlocked.fltUMeas   = 0.0F;
        g_sMID.sBlocked.fltFMeas   = 0.0F;
        g_sMID.sBlocked.ui16Active = FALSE;
        g_sMID.sCalcPwr.bCalcPwr   = FALSE;
        g_sMID.eCalcElPar          = kMID_CalcWorking;
    }
}
