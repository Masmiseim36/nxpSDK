/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rs_measure.h"
#include "mlib.h"
#include "gflib.h"

/*******************************************************************************
 * Variables
 ******************************************************************************/
/*Default transfer characteristics LookUp for HVP PowerStage and 325V DCbus*/
frac16_t f16TransferCharError[] = {
    -1859, -1857, -1869, -1882, -1883, -1887, -1878, -1868, -1865, -1870, -1873, -1865, -1869,
    -1862, -1860, -1846, -1847, -1835, -1823, -1816, -1816, -1796, -1783, -1758, -1736, -1713,
    -1709, -1663, -1594, -1494, -1373, -1063, -302,  275,   1106,  1459,  1547,  1635,  1705,
    1722,  1731,  1744,  1783,  1804,  1823,  1821,  1835,  1836,  1847,  1865,  1869,  1875,
    1866,  1864,  1869,  1878,  1899,  1899,  1904,  1891,  1886,  1890,  1896,  1908,  1909,
};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Rs measurement routine
 *
 * @param *sRsMeasFcn input structure of type #mid_get_rs_a1_t for passing
 *                          all necessary parameters.
 *
 * @return None
 */
void MID_getRs(mid_get_rs_a1_t *sRsMeasFcn)
{
    frac16_t f16Ud, f16Id;
    frac16_t f16UdReqFilt, f16IdfbckFilt;
    frac16_t f16UdCorrect;
    frac32_t f32UdCorrect, f32UdCorrectShifted;
    frac32_t f32Rs;
    frac16_t f16Id_rescaled;

    /* Initialization */
    if (sRsMeasFcn->ui16Active == 0U)
    {
        sRsMeasFcn->ui16Active             = 1U;
        sRsMeasFcn->ui16LoopCounter        = 0;
        sRsMeasFcn->f16Rs                  = FRAC16(0.0);
        sRsMeasFcn->sUdReqMA32Filter.u16Sh = 6;
        GDFLIB_FilterMAInit_F16(FRAC16(0.0), &sRsMeasFcn->sUdReqMA32Filter);
        sRsMeasFcn->sIdfbckMA32Filter.u16Sh = 6;
        GDFLIB_FilterMAInit_F16(FRAC16(0.0), &sRsMeasFcn->sIdfbckMA32Filter);

        /* Set the measuring current Id_meas*/
        *(sRsMeasFcn->pf16IdReq) = sRsMeasFcn->f16IdMeas;
    }

    /* LoopCounter for time keeping */
    sRsMeasFcn->ui16LoopCounter++;

    /* Filter required voltage and feedback current*/
    f16UdReqFilt  = GDFLIB_FilterMA_F16(*(sRsMeasFcn->pf16UdReq), &sRsMeasFcn->sUdReqMA32Filter);
    f16IdfbckFilt = GDFLIB_FilterMA_F16(*(sRsMeasFcn->pf16Idfbck), &sRsMeasFcn->sIdfbckMA32Filter);

    /* After 600ms start calculation */
    if (sRsMeasFcn->ui16LoopCounter == M1_TIME_1200MS)
    {
        /* store actual measured current and required voltage for Rs calculation */
        f16Id = f16IdfbckFilt;
        f16Ud = f16UdReqFilt;

        /* Set required current to zero */
        *(sRsMeasFcn->pf16IdReq) = FRAC16(0.0);

        /* Rescale Id to frac <-0.5;0.5) */
        f16Id_rescaled =
            MLIB_ShLBi_F16(MLIB_Mul_F16(sRsMeasFcn->f16RescaleIdLUTGain, f16Id), sRsMeasFcn->i16RescaleIdLUTShift);

        /* Calculate corrected voltage as a subtraction of the required voltage and the compensation voltage from the
         * LUT */
        f16UdCorrect = MLIB_Sub_F16(
            f16Ud, GFLIB_Lut1D_F16(f16Id_rescaled, sRsMeasFcn->pf16UdErrorLookUp, M1_CHAR_NUMBER_OF_POINTS_BASE));

        /* Calculate Rs from Ud_correct and Id */
        /* float eq. Rs = UdCorrect / Idfbck */
        /* frac  eq. f16Rs = (f16UdCorrect >> K_RS_MAX_SHIFT) / f16Idfbck  */
        f32UdCorrect = MLIB_Conv_F32s(f16UdCorrect);

        /* Set the i16ShiftRsMax to -16 */
        sRsMeasFcn->i16ShiftRsMax = -16;

        /* Divide U/I and always increase i16ShiftRsMax until the Rs result is positive and f32UdCorrectShifted is not
         * saturated */
        do
        {
            sRsMeasFcn->i16ShiftRsMax++;
            f32UdCorrectShifted = MLIB_ShLBiSat_F32(f32UdCorrect, -sRsMeasFcn->i16ShiftRsMax);
            f32Rs               = MLIB_DivSat_F32(f32UdCorrectShifted, MLIB_Conv_F32s(f16Id));

        } while ((f32Rs >= FRAC32(1.0)) || (f32UdCorrectShifted >= FRAC32(1.0)));

        /* Return frac16_t inductance */
        sRsMeasFcn->f16Rs = MLIB_Conv_F16l(f32Rs);

        /* Set Id_req to zero */
        *(sRsMeasFcn->pf16IdReq) = FRAC16(0.0);

        /* Check Faults */
        /* Check if motor is connected */
        if (MLIB_Abs_F16(*(sRsMeasFcn->pf16Idfbck)) < M1_K_I_50MA)
        {
            ui16FaultMID |= MID_FAULT_NO_MOTOR;
        }

        /* Check if Rs is negative or saturated*/
        if (sRsMeasFcn->f16Rs < FRAC16(0.0) || sRsMeasFcn->f16Rs == FRAC16(1.0))
        {
            ui16FaultMID |= MID_FAULT_RS_OUT_OF_RANGE;
        }

        /* Check if measuring current was reached */
        if (*(sRsMeasFcn->pf16Idfbck) < MLIB_Sub_F16(sRsMeasFcn->f16IdMeas, M1_K_I_50MA))
        {
            ui16FaultMID |= MID_FAULT_DC_CUR_NOT_REACHED;
        }
    }

    /* Wait additional 600ms to stabilize Id at 0A */
    /* Exit the function after 1200ms */
    if (sRsMeasFcn->ui16LoopCounter > M1_TIME_2400MS)
    {
        sRsMeasFcn->ui16Active = 0U;
    }
}
