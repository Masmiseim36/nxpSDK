/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ls_measure.h"
#include "mlib.h"
#include "gflib.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Ls measurement routine
 *
 * @param *sLsMeasFcn input structure of type #mid_get_ls_a1_t for passing
 *                      all necessary parameters.
 *
 * @return None
 */
void MID_getLs(mid_get_ls_a1_t* sLsMeasFcn)
{
    frac32_t f32UdAmplitude, f32UdAmplitudeShifted;
    frac32_t f32XRs;
    frac32_t f32Ztotal;
    frac32_t f32XLd, f32XLd_aux1, f32XLd_aux2;
    frac32_t f32Ld, f32XLdShifted;

    /* Initialisation */
    if(sLsMeasFcn->ui16Active == FALSE)
    {
        sLsMeasFcn->ui16Active = TRUE;
        sLsMeasFcn->ui16LoopCounter = 0;
        sLsMeasFcn->i16AmplitudeOK = FALSE;
        sLsMeasFcn->i16FrequencyOK = FALSE;
        sLsMeasFcn->f16AngleIncrement = FRAC16(0.0);
        sLsMeasFcn->f16Angle = FRAC16(0.0);
        sLsMeasFcn->f16IdAmplitude = FRAC16(0.0);
        sLsMeasFcn->f16UdAmplitude = FRAC16(0.0);
        sLsMeasFcn->f16Ls = FRAC16(0.0);
        sLsMeasFcn->f16FreqActual = sLsMeasFcn->f16FreqStart;
        sLsMeasFcn->f16AngleIncrementConst = M1_K_ANGLE_INCREMENT;
    }

    /* Sine angle increment */
    sLsMeasFcn->f16AngleIncrement = MLIB_Mul_F16(sLsMeasFcn->f16AngleIncrementConst, 
                                                 sLsMeasFcn->f16FreqActual);
    sLsMeasFcn->f16Angle = MLIB_Add_F16(sLsMeasFcn->f16Angle,
                                        sLsMeasFcn->f16AngleIncrement);

    /* For time keeping */
    sLsMeasFcn->ui16LoopCounter++;

    /* Measuring voltage amplitude adjusting */
    if(sLsMeasFcn->i16AmplitudeOK != TRUE)
    {
        /* Apply sine voltage (start with 10V/1kHz) for 300ms */
        if(sLsMeasFcn->ui16LoopCounter < M1_TIME_300MS)
            *(sLsMeasFcn->pf16UdReq) = MLIB_Mul_F16(sLsMeasFcn->f16UdAmplitude,
                                                    GFLIB_Sin_F16(sLsMeasFcn->f16Angle));

        /* If current f16Idfbck > f16MeasCurrentAmp, proceed to frequency adjusting */
        if((sLsMeasFcn->ui16LoopCounter > M1_TIME_100MS) && (*(sLsMeasFcn->pf16Idfbck) > sLsMeasFcn->f16IdAmplitudeReq))
        {
            sLsMeasFcn->i16AmplitudeOK = TRUE;
            *(sLsMeasFcn->pf16UdReq) = FRAC16(0.0);
            sLsMeasFcn->ui16LoopCounter = 0;
        }

        /* After 300ms */
        if(sLsMeasFcn->ui16LoopCounter >= M1_TIME_300MS)
        {
            /* Increase voltage amplitude by f16Ls_Volt_Increment */
            sLsMeasFcn->f16UdAmplitude += sLsMeasFcn->f16UdIncrement;

            /* If voltage amplitude was greater than f16UdMax, proceed to frequency adjusting */
            if(sLsMeasFcn->f16UdAmplitude > sLsMeasFcn->f16UdMax)
            {
                sLsMeasFcn->f16UdAmplitude = sLsMeasFcn->f16UdMax;
                sLsMeasFcn->i16AmplitudeOK = TRUE;
                *(sLsMeasFcn->pf16UdReq) = FRAC16(0.0);
            }

            sLsMeasFcn->ui16LoopCounter = 0;
        }
    }

    /* Measuring frequency adjusting */
    if(sLsMeasFcn->i16AmplitudeOK == TRUE && sLsMeasFcn->i16FrequencyOK != TRUE)
    {
        /* Apply voltage with frequency for 300ms */
        if(sLsMeasFcn->ui16LoopCounter < M1_TIME_300MS)
            *(sLsMeasFcn->pf16UdReq) = MLIB_Mul_F16(sLsMeasFcn->f16UdAmplitude,
                                                    GFLIB_Sin_F16(sLsMeasFcn->f16Angle));

        /* If current f16Idfbck > f16MeasCurrentAmp, proceed to measurement */
        if((sLsMeasFcn->ui16LoopCounter > M1_TIME_100MS) && (*(sLsMeasFcn->pf16Idfbck) > sLsMeasFcn->f16IdAmplitudeReq))
        {
            sLsMeasFcn->i16FrequencyOK = TRUE;
            *(sLsMeasFcn->pf16UdReq) = FRAC16(0.0);
            sLsMeasFcn->ui16LoopCounter = 0;
        }

        /* After 300ms */
        if(sLsMeasFcn->ui16LoopCounter >= M1_TIME_300MS)
        {
            /* Decrease frequency by f16Ls_Freq_Decrement */
            sLsMeasFcn->f16FreqActual -= sLsMeasFcn->f16FreqDecrement;

            /* If frequency was lower than f16FreqMin, proceed to measurement */
            if(sLsMeasFcn->f16FreqActual < sLsMeasFcn->f16FreqMin)
            {
                sLsMeasFcn->f16FreqActual = sLsMeasFcn->f16FreqMin;
                sLsMeasFcn->i16FrequencyOK = TRUE;
                *(sLsMeasFcn->pf16UdReq) = FRAC16(0.0);
            }

            sLsMeasFcn->ui16LoopCounter = 0;
        }
    }

    /* When amplitude and frequency adjusted, start Ld measurement */
    if(sLsMeasFcn->i16AmplitudeOK == TRUE && sLsMeasFcn->i16FrequencyOK == TRUE)
    {
        /* Apply measuring signal */
        if(sLsMeasFcn->ui16LoopCounter <= M1_TIME_300MS)
        {
            /* Sine to d_axis */
            *(sLsMeasFcn->pf16UdReq) = MLIB_Mul_F16(sLsMeasFcn->f16UdAmplitude,
                                                    GFLIB_Sin_F16(sLsMeasFcn->f16Angle));

            /* Current amplitudes after 100ms delay */
            if((sLsMeasFcn->ui16LoopCounter > M1_TIME_100MS) && (*(sLsMeasFcn->pf16Idfbck) > sLsMeasFcn->f16IdAmplitude))
                sLsMeasFcn->f16IdAmplitude = *(sLsMeasFcn->pf16Idfbck);
        }

        /* Inductance calculation */
        if(sLsMeasFcn->ui16LoopCounter > M1_TIME_300MS)
        {
            /* Total circuit reactance */
            /* float eq. Z = Ud / Id */
            /* frac  eq. f32Rs_voltage_drop = (f16Ud_max1  << i16K_Z_Max_Shift) / f16Id_max1 */
            f32UdAmplitude = MLIB_Conv_F32s(sLsMeasFcn->f16UdAmplitude);

            /* Set the i16ShiftZsMax to -16 */
            sLsMeasFcn->i16ShiftZsMax = -16;

            /* Divide U/I and always increase i16ShiftZsMax until the Ztotal result is positive 
               and f32UdAmplitudeShifted is not saturated */
            do
            {
                sLsMeasFcn->i16ShiftZsMax++;
                f32UdAmplitudeShifted = MLIB_ShLBiSat_F32(f32UdAmplitude, 
                                                          -sLsMeasFcn->i16ShiftZsMax);
                f32Ztotal = MLIB_DivSat_F32(f32UdAmplitudeShifted,
                                            MLIB_Conv_F32s(sLsMeasFcn->f16IdAmplitude));

            }while(f32Ztotal >= FRAC32(1.0) || f32UdAmplitudeShifted >= FRAC32(1.0));

            /* Rs re-scaling from FM_RS_SCALE => FM_Z_SCALE */
            f32XRs = MLIB_ShLBi_F32(MLIB_Conv_F32s(sLsMeasFcn->f16Rs), 
                                    -(sLsMeasFcn->i16ShiftZsMax - sLsMeasFcn->i16ShiftRsMax));

            /* Pure inductive reactance */
            /* float eq. XL = sqrt(Z^2 - R^2) */
            /* frac  eq. f32XLd = sqrt((f32Ztotal*f32Ztotal)-(f32XRs*f32XRs)) */
            f32XLd_aux1 = MLIB_Mul_F32(f32Ztotal, f32Ztotal);
            f32XLd_aux2 = MLIB_Mul_F32(f32XRs, f32XRs);
            f32XLd = MLIB_Conv_F32s(GFLIB_Sqrt_F16l(MLIB_Sub_F32(f32XLd_aux1, f32XLd_aux2)));

            /* Inductance */
            /* float eq. L = XL / (2*pi*f) */
            /* frac  eq. f32Ld = (f32XLd  << i16K_L_Max_Shift) / f16MeasFreq */

            /* Set the i16ShiftLMax to -16 */
            sLsMeasFcn->i16ShiftLsMax = -16;

            /* Divide U/I and always increase i16ShiftZMax until the Ztotal result is positive and f32UdAmplitudeShifted is not saturated */
            do
            {
                sLsMeasFcn->i16ShiftLsMax++;
                f32XLdShifted = MLIB_ShLBiSat_F32(f32XLd, -sLsMeasFcn->i16ShiftLsMax);
                f32Ld = MLIB_DivSat_F32(f32XLdShifted, MLIB_Conv_F32s(sLsMeasFcn->f16FreqActual));

            }while(f32Ld >= FRAC32(1.0) || f32XLdShifted >= FRAC32(1.0));

            /* Return frac16_t inductance */
            sLsMeasFcn->f16Ls = MLIB_Conv_F16l(f32Ld);

            sLsMeasFcn->ui16Active = FALSE;
            *(sLsMeasFcn->pf16UdReq) = FRAC16(0.0);

            /* Check Faults */
            /* Check if f16MeasCurrentAmp was reached (95% of the f16MeasCurrentAmp) */
            if(sLsMeasFcn->f16IdAmplitude < MLIB_Mul_F16(sLsMeasFcn->f16IdAmplitudeReq, FRAC16(0.95)))
                ui16FaultMID |= MID_FAULT_AC_CUR_NOT_REACHED;

            /* Check negative result or saturation of Z*/
            if(f32Ztotal < FRAC32(0.0) || f32Ztotal == FRAC32(1.0))
                ui16FaultMID |= MID_FAULT_Z_OUT_OF_RANGE;

                        /* Check negative result or saturation of Ls*/
            if(sLsMeasFcn->f16Ls < FRAC16(0.0) || sLsMeasFcn->f16Ls == FRAC16(1.0))
                ui16FaultMID |= MID_FAULT_LS_OUT_OF_RANGE;

            /* Check if motor is connected */
            if(sLsMeasFcn->f16IdAmplitude < M1_K_I_50MA)
                ui16FaultMID |= MID_FAULT_NO_MOTOR;
        }
    }
}