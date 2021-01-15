/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pwrstg_characteristic.h"
#include "mlib.h"
#include "gflib.h"
#include "mlib_types.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Power Stage Characterization routine
 *
 * @param *sTransferCharFcn input structure of type #mid_get_char_a1_t for passing
 *                          all necessary parameters.
 *
 * @return None
 */
void MID_GetTransferCharacteristic(mid_get_char_a1_t *sTransferCharFcn)
{
    frac16_t f16Rs_voltage_drop; /* Auxiliary variable for Rs voltage drop calculation */
    frac16_t f16UdReqFilt;       /* Filtered Ud required value */
    frac16_t f16IdfbckFilt;      /* Filtered Id feedback value */
    frac32_t f32Rs_voltage_drop; /* Auxiliary variable for Rs voltage drop calculation */
    frac32_t f32IdfbckFilt, f32Rs;

    /* Initialisation */
    if (sTransferCharFcn->ui16Active == 0U)
    {
        sTransferCharFcn->ui16Active             = 1U;
        sTransferCharFcn->ui16LoopCounter        = 0;
        sTransferCharFcn->f16IdReqActual         = MLIB_Neg_F16(sTransferCharFcn->f16IdCalib);
        *(sTransferCharFcn->pf16IdReq)           = sTransferCharFcn->f16IdReqActual;
        sTransferCharFcn->ui16LUTIndex           = 0;
        sTransferCharFcn->sUdReqMA32Filter.u16Sh = 6;
        GDFLIB_FilterMAInit_F16(FRAC16(0.0), &sTransferCharFcn->sUdReqMA32Filter);
        sTransferCharFcn->sIdfbckMA32Filter.u16Sh = 6;
        GDFLIB_FilterMAInit_F16(FRAC16(0.0), &sTransferCharFcn->sIdfbckMA32Filter);
    }

    /* LoopCounter for time keeping */
    sTransferCharFcn->ui16LoopCounter++;

    /* Filter required voltage and feedback current*/
    f16UdReqFilt  = GDFLIB_FilterMA_F16(*(sTransferCharFcn->pf16UdReq), &sTransferCharFcn->sUdReqMA32Filter);
    f16IdfbckFilt = GDFLIB_FilterMA_F16(*(sTransferCharFcn->pf16Idfbck), &sTransferCharFcn->sIdfbckMA32Filter);

    /* After 300ms settling of Id start calculation */
    if (sTransferCharFcn->ui16LoopCounter >= M1_TIME_600MS)
    {
        /* Faults */
        /* Check if Rs is low enough to reach 2A */
        if ((MLIB_Abs_F16(*(sTransferCharFcn->pf16Idfbck)) < (sTransferCharFcn->f16IdCalib - M1_K_I_50MA)) &&
            (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            ui16FaultMID |= MID_FAULT_TOO_HIGH_RS;
            sTransferCharFcn->ui16Active   = 0U;
            *(sTransferCharFcn->pf16IdReq) = FRAC16(0.0);
        }
        /* Check if motor is connected */
        if ((MLIB_Abs_F16(*(sTransferCharFcn->pf16Idfbck)) < M1_K_I_50MA) && (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            ui16FaultMID |= MID_FAULT_NO_MOTOR;
            sTransferCharFcn->ui16Active   = 0U;
            *(sTransferCharFcn->pf16IdReq) = FRAC16(0.0);
        }

        /* Calculate voltage drop from Rs and Id */
        /* float eq. V_Rs = Rs * Idfbck */
        /* frac  eq. f16Rs_voltage_drop = (f16Rs * f16Idfbck) << K_RS_MAX_SHIFT */
        f32IdfbckFilt      = MLIB_Conv_F32s(f16IdfbckFilt);
        f32Rs              = MLIB_Conv_F32s(sTransferCharFcn->f16Rs);
        f32Rs_voltage_drop = MLIB_ShLBi_F32(MLIB_Mul_F32(f32Rs, f32IdfbckFilt), sTransferCharFcn->i16ShiftRsMax);
        f16Rs_voltage_drop = MLIB_Conv_F16l(f32Rs_voltage_drop);

        /* Calculate Error voltage and store it to f16ErrorLookUp */
        /* float eq. Error voltage = Required voltage - Rs voltage drop */
        sTransferCharFcn->f16UdErrorLookUp[sTransferCharFcn->ui16LUTIndex] =
            MLIB_Sub_F16(f16UdReqFilt, f16Rs_voltage_drop);

        /* Prepare for next point measurement */
        sTransferCharFcn->ui16LUTIndex++;
        sTransferCharFcn->f16IdReqActual =
            MLIB_Add_F16(sTransferCharFcn->f16IdReqActual, sTransferCharFcn->f16IdIncrement);
        *(sTransferCharFcn->pf16IdReq)    = sTransferCharFcn->f16IdReqActual;
        sTransferCharFcn->ui16LoopCounter = 0;

        /* End after last current was measured */
        if (sTransferCharFcn->ui16LUTIndex >= M1_CHAR_CURRENT_POINT_NUMBERS)
        {
            sTransferCharFcn->ui16Active   = 0U;
            *(sTransferCharFcn->pf16IdReq) = FRAC16(0.0);
        }
    }
}
