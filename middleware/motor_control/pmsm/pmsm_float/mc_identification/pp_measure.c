/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pp_measure.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MID_RAMP_COEFF_MULT (MID_FAST_LOOP_TS / MID_SPEED_RAMP_TIME)

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
void MID_getPp(mid_get_pp_t* sPpMeasFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;

    /* Initialisation */
    if(sPpMeasFcn->bActive == FALSE)
    {
        sPpMeasFcn->bActive                      = TRUE;
        sPpMeasFcn->fltFreqElRamp                   = 0.0F;
        sPpMeasFcn->ui16PpDetermined                = 0U;
        sPpMeasFcn->sFreqElRampParam.fltRampUp      = sPpMeasFcn->fltFreqElReq * MID_RAMP_COEFF_MULT;
        sPpMeasFcn->sFreqElRampParam.fltRampDown    = sPpMeasFcn->fltFreqElReq * MID_RAMP_COEFF_MULT;
        sPpMeasFcn->fltFreqMax                      = sPpMeasFcn->fltFreqElReq;
        sPpMeasFcn->sFreqIntegrator.a32Gain         = MLIB_Conv_A32f(sPpMeasFcn->fltFreqMax * MID_FAST_LOOP_TS * 2.0F);
        sPpMeasFcn->sFreqIntegrator.f32IAccK_1      = FRAC32(0.0F);
        GFLIB_IntegratorInit_F16(0, &sPpMeasFcn->sFreqIntegrator);
        GFLIB_RampInit_FLT(0.0F, &sPpMeasFcn->sFreqElRampParam);
    }

    /* Set Id required */
    sIDQReq.fltD = sPpMeasFcn->fltIdReqOpenLoop;
    sIDQReq.fltQ = 0.0F;
    MID_MC_SetIDQReq(sIDQReq);

    /* Else start incrementing position */
    if(sPpMeasFcn->ui16WaitingSteady == 0U)
    {
        /* Ramp electrical speed */
        sPpMeasFcn->fltFreqElRamp = GFLIB_Ramp_FLT(sPpMeasFcn->fltFreqElReq, &sPpMeasFcn->sFreqElRampParam);
        /* Integrate electrical speed to get electrical position */
        MID_MC_SetExternalPosEl(GFLIB_Integrator_F16(MLIB_ConvSc_F16ff(sPpMeasFcn->fltFreqElRamp, sPpMeasFcn->fltFreqMax), &sPpMeasFcn->sFreqIntegrator));
    }

    /* If position overflows, wait 2400ms in zero position */
    if(((MID_MC_GetExternalPosEl() < FRAC16(0.0F)) && (sPpMeasFcn->f16PosElLast > FRAC16(0.0F))) || (sPpMeasFcn->ui16WaitingSteady == 1U))
    {
        MID_MC_SetExternalPosEl(FRAC16(-1.0F));

        /* Initialise waiting */
        if(sPpMeasFcn->ui16WaitingSteady == 0U)
        {
            sPpMeasFcn->ui16LoopCounter = 0U;
            sPpMeasFcn->ui16WaitingSteady = 1U;
        }

        sPpMeasFcn->ui16LoopCounter++;

        /* Escape waiting in steady position after 2400 ms */
        if(sPpMeasFcn->ui16LoopCounter > MID_TIME_2400MS)
        {
            MID_MC_SetExternalPosEl(FRAC16(0.0F));
            sPpMeasFcn->f16PosElLast = FRAC16(0.0F);
            sPpMeasFcn->ui16WaitingSteady = 0U;
        }
    }

    /* Save last position */
    sPpMeasFcn->f16PosElLast = sPpMeasFcn->f16PosElCurrent;
    sPpMeasFcn->f16PosElCurrent = MID_MC_GetExternalPosEl();

    if(sPpMeasFcn->ui16PpDetermined > 0U)
    {
        /* When finished exit the function */
        sPpMeasFcn->bActive = FALSE;
    }
}

