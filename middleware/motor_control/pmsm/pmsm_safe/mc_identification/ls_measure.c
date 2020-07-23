/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ls_measure.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Ls measurement routine
 *
 * @param *sLsMeasFcn   input structure of type #mid_get_ls_t for passing
 *                      all necessary parameters.
 *
 * @return None
 */
void MID_getLs(mid_get_ls_t* sLsMeasFcn)
{
    GMCLIB_2COOR_DQ_T_FLT   sUDQReq;
    GMCLIB_2COOR_DQ_T_FLT   sIDQfbck;
    float_t                 fltZtotal, fltRs_square, fltZtotal_square, fltXLd;

    /* Initialisation */
    if(sLsMeasFcn->ui16Active == FALSE)
    {
        sLsMeasFcn->ui16Active                  = TRUE;
        sLsMeasFcn->ui16LoopCounter             = 0U;
        sLsMeasFcn->bAmplitudeOK                = FALSE;
        sLsMeasFcn->bFrequencyOK                = FALSE;
        sLsMeasFcn->f16Angle                    = FRAC16(0.0F);
        sLsMeasFcn->fltIdAmplitude              = 0.0F;
        sLsMeasFcn->fltUdAmplitude              = 0.0F;
        sLsMeasFcn->fltLs                       = 0.0F;
        sLsMeasFcn->fltFreqActual               = sLsMeasFcn->fltFreqStart;
        sLsMeasFcn->fltFreqMax                  = MID_FREQ_MAX;
        sLsMeasFcn->sFreqIntegrator.a32Gain     = MLIB_Conv_A32f(sLsMeasFcn->fltFreqMax * MID_FAST_LOOP_TS * 2.0F);
        sLsMeasFcn->sFreqIntegrator.f32IAccK_1  = FRAC32(0.0F);
        GFLIB_IntegratorInit_F16(0, &sLsMeasFcn->sFreqIntegrator);
    }

    /* Integrate electrical frequency to get angle */
    sLsMeasFcn->f16Angle = GFLIB_Integrator_F16(MLIB_ConvSc_F16ff(sLsMeasFcn->fltFreqActual, sLsMeasFcn->fltFreqMax), &sLsMeasFcn->sFreqIntegrator);

    /* For time keeping */
    sLsMeasFcn->ui16LoopCounter++;

    /* Read IDQfbck */
    MID_MC_GetIDQfbck(&sIDQfbck);

    /* Measuring voltage amplitude adjusting */
    if(sLsMeasFcn->bAmplitudeOK != TRUE)
    {
        /* Apply sine voltage (start with 10V/1kHz) for 300ms */
        if(sLsMeasFcn->ui16LoopCounter < MID_TIME_300MS)
        {
            sUDQReq.fltD = MLIB_Mul_FLT(sLsMeasFcn->fltUdAmplitude, GFLIB_Sin_FLTa((acc32_t)sLsMeasFcn->f16Angle));
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);
        }

        /* If current f16Idfbck > f16MeasCurrentAmp, proceed to frequency adjusting */
        if((sLsMeasFcn->ui16LoopCounter > MID_TIME_100MS) && (sIDQfbck.fltD > sLsMeasFcn->fltIdAmplitudeReq))
        {
            sLsMeasFcn->bAmplitudeOK = TRUE;
            sUDQReq.fltD = 0.0F;
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);
            sLsMeasFcn->ui16LoopCounter = 0U;
        }

        /* After 300ms */
        if(sLsMeasFcn->ui16LoopCounter >= MID_TIME_300MS)
        {
            /* Increase voltage amplitude by f16Ls_Volt_Increment */
            sLsMeasFcn->fltUdAmplitude += sLsMeasFcn->fltUdIncrement;

            /* If voltage amplitude was greater than f16UdMax, proceed to frequency adjusting */
            if(sLsMeasFcn->fltUdAmplitude > sLsMeasFcn->fltUdMax)
            {
                sLsMeasFcn->fltUdAmplitude = sLsMeasFcn->fltUdMax;
                sLsMeasFcn->bAmplitudeOK = TRUE;
                sUDQReq.fltD = 0.0F;
                sUDQReq.fltQ = 0.0F;
                MID_MC_SetUDQReq(sUDQReq);
            }

            sLsMeasFcn->ui16LoopCounter = 0U;
        }
    }

    /* Measuring frequency adjusting */
    if((sLsMeasFcn->bAmplitudeOK == TRUE) && (sLsMeasFcn->bFrequencyOK != TRUE))
    {
        /* Apply voltage with frequency for 300ms */
        if(sLsMeasFcn->ui16LoopCounter < MID_TIME_300MS)
        {
            sUDQReq.fltD = MLIB_Mul_FLT(sLsMeasFcn->fltUdAmplitude, GFLIB_Sin_FLTa((acc32_t)sLsMeasFcn->f16Angle));
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);
        }

        /* If current f16Idfbck > f16MeasCurrentAmp, proceed to measurement */
        if((sLsMeasFcn->ui16LoopCounter > MID_TIME_100MS) && (sIDQfbck.fltD > sLsMeasFcn->fltIdAmplitudeReq))
        {
            sLsMeasFcn->bFrequencyOK = TRUE;
            sUDQReq.fltD = 0.0F;
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);
            sLsMeasFcn->ui16LoopCounter = 0U;
        }

        /* After 300ms */
        if(sLsMeasFcn->ui16LoopCounter >= MID_TIME_300MS)
        {
            /* Decrease frequency by f16Ls_Freq_Decrement */
            sLsMeasFcn->fltFreqActual -= sLsMeasFcn->fltFreqDecrement;

            /* If frequency was lower than f16FreqMin, proceed to measurement */
            if(sLsMeasFcn->fltFreqActual < sLsMeasFcn->fltFreqMin)
            {
                sLsMeasFcn->fltFreqActual = sLsMeasFcn->fltFreqMin;
                sLsMeasFcn->bFrequencyOK = TRUE;
                sUDQReq.fltD = 0.0F;
                sUDQReq.fltQ = 0.0F;
                MID_MC_SetUDQReq(sUDQReq);
            }

            sLsMeasFcn->ui16LoopCounter = 0U;
        }
    }

    /* When amplitude and frequency adjusted, start Ld measurement */
    if((sLsMeasFcn->bAmplitudeOK == TRUE) && (sLsMeasFcn->bFrequencyOK == TRUE))
    {
        /* Apply measuring signal */
        if(sLsMeasFcn->ui16LoopCounter <= MID_TIME_300MS)
        {
            /* Sine to d_axis */
            sUDQReq.fltD = MLIB_Mul_FLT(sLsMeasFcn->fltUdAmplitude, GFLIB_Sin_FLTa((acc32_t)sLsMeasFcn->f16Angle));
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);

            /* Current amplitudes after 100ms delay */
            if((sLsMeasFcn->ui16LoopCounter > MID_TIME_100MS) && (sIDQfbck.fltD > sLsMeasFcn->fltIdAmplitude))
            {
                sLsMeasFcn->fltIdAmplitude = sIDQfbck.fltD;
            }
        }

        /* Inductance calculation */
        if(sLsMeasFcn->ui16LoopCounter > MID_TIME_300MS)
        {
            /* Total circuit reactance */
            /* float eq. Z = Ud / Id */
            fltZtotal = MLIB_Div_FLT(sLsMeasFcn->fltUdAmplitude,  sLsMeasFcn->fltIdAmplitude);

            /* Pure inductive reactance */
            /* float eq. XL = sqrt(Z^2 - R^2) */
            fltZtotal_square = MLIB_Mul_FLT(fltZtotal, fltZtotal);
            fltRs_square = MLIB_Mul_FLT(sLsMeasFcn->fltRs, sLsMeasFcn->fltRs);
            fltXLd = GFLIB_Sqrt_FLT(MLIB_Sub_FLT(fltZtotal_square, fltRs_square));

            /* Inductance */
            /* float eq. L = XL / (2*pi*f) */
            sLsMeasFcn->fltLs = MLIB_Div_FLT(fltXLd, MLIB_Mul_FLT(2.0F, MLIB_Mul_FLT(FLOAT_PI, sLsMeasFcn->fltFreqActual)));

            sLsMeasFcn->ui16Active = FALSE;
            sUDQReq.fltD = 0.0F;
            sUDQReq.fltQ = 0.0F;
            MID_MC_SetUDQReq(sUDQReq);

            /* Check Faults */
            /* Check if f16MeasCurrentAmp was reached (95% of the f16MeasCurrentAmp) */
            if(sLsMeasFcn->fltIdAmplitude < MLIB_Mul_FLT(sLsMeasFcn->fltIdAmplitudeReq, 0.95F))
            {
                g_sMID.ui16WarnMID |= MID_WARN_AC_CUR_NOT_REACHED;
            }

            /* Check negative result or saturation of Z*/
            if(fltZtotal < 0.0F)
            {
                g_sMID.ui16WarnMID |= MID_WARN_LS_OUT_OF_RANGE;
            }

            /* Check negative result or saturation of Ls*/
            if(sLsMeasFcn->fltLs < 0.0F)
            {
                g_sMID.ui16WarnMID |= MID_WARN_LS_OUT_OF_RANGE;
            }

            /* Check if motor is connected */
            if(sLsMeasFcn->fltIdAmplitude < MID_K_I_50MA)
            {
                g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;
            }
        }
    }
}

