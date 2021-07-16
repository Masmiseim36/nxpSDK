/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pp_measure.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Pp measurement routine
 *
 * @param *sPpMeasFcn   input structure of type #mid_get_pp_t for passing
 *                      all necessary parameters.
 *
 * @return None
 */
void MID_getPp(mid_get_pp_t *sPpMeasFcn)
{
    /* Initialisation */
    if (sPpMeasFcn->bActive == FALSE)
    {
        sPpMeasFcn->bActive                   = TRUE;
        sPpMeasFcn->fltFreqElRamp                = 0.0F;
        sPpMeasFcn->ui16PpDetermined             = 0U;
        sPpMeasFcn->sFreqElRampParam.fltRampUp   = sPpMeasFcn->fltFreqElReq / MID_SPEED_RAMP_TIME / 10000.0F;
        sPpMeasFcn->sFreqElRampParam.fltRampDown = sPpMeasFcn->fltFreqElReq / MID_SPEED_RAMP_TIME / 10000.0F;
        sPpMeasFcn->sFreqIntegrator.a32Gain      = ACC32(1.0F * sPpMeasFcn->fltFreqMax / 10000.0F);
        sPpMeasFcn->sFreqIntegrator.f32IAccK_1   = FRAC32(0.0);
        GFLIB_IntegratorInit_F16(FRAC16(0.0), &sPpMeasFcn->sFreqIntegrator);
        GFLIB_RampInit_FLT(0.0F, &sPpMeasFcn->sFreqElRampParam);
    }

    /* Set Id required */
    *(sPpMeasFcn->pfltIdReq) = sPpMeasFcn->fltIdReqOpenLoop;

    /* Else start incrementing position */
    if (sPpMeasFcn->ui16WaitingSteady == 0U)
    {
        /* Ramp electrical speed */
        sPpMeasFcn->fltFreqElRamp = GFLIB_Ramp_FLT(sPpMeasFcn->fltFreqElReq, &sPpMeasFcn->sFreqElRampParam);
        /* Integrate electrical speed to get electrical position */
        *sPpMeasFcn->pf16PosEl = GFLIB_Integrator_F16(
            MLIB_ConvSc_F16ff(sPpMeasFcn->fltFreqElRamp, sPpMeasFcn->fltFreqMax), &sPpMeasFcn->sFreqIntegrator);
    }

    /* If position overflows, wait 2400ms in zero position */
    if (((*sPpMeasFcn->pf16PosEl < FRAC16(0.0)) && (sPpMeasFcn->f16PosElLast > FRAC16(0.0))) ||
        (sPpMeasFcn->ui16WaitingSteady == 1U))
    {
        *sPpMeasFcn->pf16PosEl = FRAC16(-1.0);

        /* Initialise waiting */
        if (sPpMeasFcn->ui16WaitingSteady == 0U)
        {
            sPpMeasFcn->ui16LoopCounter   = 0U;
            sPpMeasFcn->ui16WaitingSteady = 1U;
        }

        sPpMeasFcn->ui16LoopCounter++;

        /* Escape waiting in steady position after 2400 ms */
        if (sPpMeasFcn->ui16LoopCounter > (uint16_t)MID_TIME_2400MS)
        {
            *sPpMeasFcn->pf16PosEl        = FRAC16(0.0);
            sPpMeasFcn->f16PosElLast      = FRAC16(0.0);
            sPpMeasFcn->ui16WaitingSteady = 0U;
        }
    }

    /* Save last position */
    sPpMeasFcn->f16PosElLast    = sPpMeasFcn->f16PosElCurrent;
    sPpMeasFcn->f16PosElCurrent = *sPpMeasFcn->pf16PosEl;

    if (sPpMeasFcn->ui16PpDetermined > 0U)
    {
        /* When finished exit the function */
        sPpMeasFcn->bActive = FALSE;
    }
}
