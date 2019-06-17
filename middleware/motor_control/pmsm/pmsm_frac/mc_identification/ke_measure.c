/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "ke_measure.h"
#include "mlib.h"
#include "gflib.h"
#include "mlib_types.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Ke measurement routine
 *
 * @param *sKeMeasFcn input structure of type #mid_get_ke_a1_t for passing
 *                    all necessary parameters.
 *
 * @return None
 */
void MID_getKe(mid_get_ke_a1_t* sKeMeasFcn)
{
    frac32_t f32EdFilt, f32EqFilt;
    frac32_t f32EdFiltSquare, f32EqFiltSquare;
    frac32_t f32Etotal, f32EtotalShifted, f32SpeedElReq, f32Ke;

    /* Initialisation */
    if(sKeMeasFcn->ui16Active == FALSE)
    {
        sKeMeasFcn->sSpeedElRampParam.f16RampUp = MID_SPEED_RAMP_UP;
        sKeMeasFcn->sSpeedElRampParam.f16RampDown = MID_SPEED_RAMP_DOWN;
        sKeMeasFcn->sSpeedIntegrator.a32Gain = M1_SCALAR_INTEG_GAIN;
        sKeMeasFcn->sSpeedIntegrator.f32IAccK_1 = FRAC32(0.0);
        GFLIB_IntegratorInit_F16(0, &sKeMeasFcn->sSpeedIntegrator);
        sKeMeasFcn->sEdMA32Filter.u16Sh = 10;
        sKeMeasFcn->sEdMA32Filter.a32Acc = FRAC32(0.0);
        sKeMeasFcn->sEqMA32Filter.u16Sh = 10;
        sKeMeasFcn->sEqMA32Filter.a32Acc = FRAC32(0.0);

        sKeMeasFcn->ui16Active = TRUE;
        sKeMeasFcn->ui16LoopCounter = 0;
        sKeMeasFcn->f16SpeedElRamp = FRAC16(0.0);
    }
    /* Set Id required */
    *(sKeMeasFcn->pf16IdReq) = sKeMeasFcn->f16IdReqOpenLoop;
    /* Ramp electrical speed */
    sKeMeasFcn->f16SpeedElRamp = GFLIB_Ramp_F16(sKeMeasFcn->f16SpeedElReq, 
                                                &sKeMeasFcn->sSpeedElRampParam);
    /* Integrate electrical speed to get electrical position */
    *sKeMeasFcn->pf16PosEl = GFLIB_Integrator_F16(sKeMeasFcn->f16SpeedElRamp, 
                                                  &sKeMeasFcn->sSpeedIntegrator);

    /* Bemf filtering */
    f32EdFilt = MLIB_Conv_F32s(GDFLIB_FilterMA_F16(MLIB_Conv_F16l(*(sKeMeasFcn->pf32Ed)),
                                                                  &sKeMeasFcn->sEdMA32Filter));
    f32EqFilt = MLIB_Conv_F32s(GDFLIB_FilterMA_F16(MLIB_Conv_F16l(*(sKeMeasFcn->pf32Eq)),
                                                                  &sKeMeasFcn->sEqMA32Filter));

    if(sKeMeasFcn->f16SpeedElRamp == sKeMeasFcn->f16SpeedElReq)
    {
        sKeMeasFcn->ui16LoopCounter++;

        if(sKeMeasFcn->ui16LoopCounter > M1_TIME_2400MS)
        {
            /* Total Bemf calculation */
            f32EdFiltSquare = MLIB_Mul_F32(f32EdFilt, f32EdFilt);
            f32EqFiltSquare = MLIB_Mul_F32(f32EqFilt, f32EqFilt);
            f32Etotal = MLIB_Conv_F32s(GFLIB_Sqrt_F16l(MLIB_Add_F32(f32EdFiltSquare,
                                                                    f32EqFiltSquare)));

            /* Ke calculation */
            f32Etotal = MLIB_ShR_F32(f32Etotal,
                                     sKeMeasFcn->i16ShiftKeMax);
            f32SpeedElReq = MLIB_Conv_F32s(sKeMeasFcn->f16SpeedElReq);

            /* Set the i16ShiftRsMax to -1 */
            sKeMeasFcn->i16ShiftKeMax = -1;

            /* Divide U/I and always increase i16ShiftRsMax until the Rs result is positive and f32UdCorrectShifted is not saturated */
            do
            {
                sKeMeasFcn->i16ShiftKeMax++;
                f32EtotalShifted = MLIB_ShLBiSat_F32(f32Etotal, 
                                                     -sKeMeasFcn->i16ShiftKeMax);
                f32Ke = MLIB_DivSat_F32(f32EtotalShifted,
                                        f32SpeedElReq);

            }while(f32Ke >= FRAC32(1.0) || f32EtotalShifted >= FRAC32(1.0));

            /* Return frac16_t inductance */
            sKeMeasFcn->f16Ke = MLIB_Conv_F16l(f32Ke);

            /* Check Faults */
            /* Check if Ke is negative or saturated*/
            if(sKeMeasFcn->f16Ke < FRAC16(0.0) || sKeMeasFcn->f16Ke == FRAC16(1.0))
                ui16FaultMID |= MID_FAULT_KE_OUT_OF_RANGE;

            /* When finished exit the function */
            sKeMeasFcn->ui16Active = FALSE;
        }
    }
}

