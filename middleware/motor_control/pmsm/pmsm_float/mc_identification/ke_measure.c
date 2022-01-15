/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ke_measure.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MID_RAMP_COEFF_MULT (MID_FAST_LOOP_TS / MID_SPEED_RAMP_TIME)

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Ke measurement routine
 *
 * @param *sKeMeasFcn   input structure of type #mid_get_ke_t for passing
 *                      all necessary parameters.
 *
 * @return None
 */
void MID_getKe(mid_get_ke_t* sKeMeasFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;
    GMCLIB_2COOR_DQ_T_FLT sEDQObsrv;
    float_t fltEdFilt, fltEqFilt;
    float_t fltEdFiltSquare, fltEqFiltSquare;
    float_t fltEtotal;
    frac16_t f16PosOL;

    /* Initialisation */
    if(sKeMeasFcn->bActive == FALSE)
    {
        sKeMeasFcn->bActive                      = TRUE;
        sKeMeasFcn->ui16LoopCounter                 = 0U;
        sKeMeasFcn->fltFreqElRamp                   = 0.0F;
        sKeMeasFcn->sFreqElRampParam.fltRampUp      = sKeMeasFcn->fltFreqElReq * MID_RAMP_COEFF_MULT;
        sKeMeasFcn->sFreqElRampParam.fltRampDown    = sKeMeasFcn->fltFreqElReq * MID_RAMP_COEFF_MULT;
        sKeMeasFcn->sEdMA32Filter.fltLambda         = MID_KE_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sKeMeasFcn->sEdMA32Filter);
        sKeMeasFcn->sEqMA32Filter.fltLambda         = MID_KE_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sKeMeasFcn->sEqMA32Filter);
        sKeMeasFcn->fltFreqMax                      = sKeMeasFcn->fltFreqElReq;
        sKeMeasFcn->sFreqIntegrator.a32Gain         = MLIB_Conv_A32f(sKeMeasFcn->fltFreqMax * MID_FAST_LOOP_TS * (float_t)2.0F);
        GFLIB_IntegratorInit_F16(0, &sKeMeasFcn->sFreqIntegrator);
        GFLIB_RampInit_FLT(0.0F, &sKeMeasFcn->sFreqElRampParam);
    }

    /* Set Id required */
    sIDQReq.fltD = sKeMeasFcn->fltIdReqOpenLoop;
    sIDQReq.fltQ = 0.0F;
    MID_MC_SetIDQReq(sIDQReq);
    /* Ramp electrical speed */
    sKeMeasFcn->fltFreqElRamp = GFLIB_Ramp_FLT(sKeMeasFcn->fltFreqElReq, &sKeMeasFcn->sFreqElRampParam);
    /* Integrate electrical speed to get electrical position */
    f16PosOL = GFLIB_Integrator_F16(MLIB_ConvSc_F16ff(sKeMeasFcn->fltFreqElRamp, sKeMeasFcn->fltFreqMax), &sKeMeasFcn->sFreqIntegrator);
    MID_MC_SetExternalPosEl(f16PosOL);

    /* Read Bemf observed voltages */
    MID_MC_GetEDQObsrv(&sEDQObsrv);

    /* Bemf filtering */
    fltEdFilt = GDFLIB_FilterMA_FLT(sEDQObsrv.fltD, &sKeMeasFcn->sEdMA32Filter);
    fltEqFilt = GDFLIB_FilterMA_FLT(sEDQObsrv.fltQ, &sKeMeasFcn->sEqMA32Filter);

    if(sKeMeasFcn->fltFreqElRamp >= sKeMeasFcn->fltFreqElReq)
    {
        sKeMeasFcn->ui16LoopCounter++;

        if(sKeMeasFcn->ui16LoopCounter > MID_TIME_2400MS)
        {
            /* Total Bemf calculation */
            fltEdFiltSquare = MLIB_Mul_FLT(fltEdFilt, fltEdFilt);
            fltEqFiltSquare = MLIB_Mul_FLT(fltEqFilt, fltEqFilt);
            fltEtotal = GFLIB_Sqrt_FLT(MLIB_Add_FLT(fltEdFiltSquare, fltEqFiltSquare));

            /* Ke calculation */
            sKeMeasFcn->fltKe = MLIB_Div_FLT(fltEtotal, sKeMeasFcn->fltFreqElReq);

            /* Check Faults */
            /* Check if Ke is negative or saturated*/
            if(sKeMeasFcn->fltKe < 0.0F)
            {
                g_sMID.ui16WarnMID |= MID_WARN_KE_OUT_OF_RANGE;
            }

            /* When finished exit the function */
            sKeMeasFcn->bActive = FALSE;
        }
    }
}

