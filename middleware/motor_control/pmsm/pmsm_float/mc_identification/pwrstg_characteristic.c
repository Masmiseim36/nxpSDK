/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pwrstg_characteristic.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Power Stage Characterization routine
 *
 * @param *sTransferCharFcn input structure of type #mid_get_char_t for passing
 *                          all necessary parameters.
 *
 * @return None
 */
float_t fltRs_voltage_drop; /* Auxiliary variable for Rs voltage drop calculation */
float_t fltUdReqFilt;       /* Filtered Ud required value */
float_t fltIdfbckFilt;      /* Filtered Id feedback value */

void MID_GetTransferCharacteristic(mid_get_char_t *sTransferCharFcn)
{
    /* Initialisation */
    if (sTransferCharFcn->bActive == FALSE)
    {
        sTransferCharFcn->bActive                 = TRUE;
        sTransferCharFcn->ui16LoopCounter            = 0U;
        sTransferCharFcn->fltIdReqActual             = MLIB_Neg_FLT(sTransferCharFcn->fltIdCalib);
        *(sTransferCharFcn->pfltIdReq)               = sTransferCharFcn->fltIdReqActual;
        sTransferCharFcn->ui16LUTIndex               = 0U;
        sTransferCharFcn->sUdReqMA32Filter.fltLambda = 1.0F / 20.0F;
        GDFLIB_FilterMAInit_FLT(0.0, &sTransferCharFcn->sUdReqMA32Filter);
        sTransferCharFcn->sIdfbckMA32Filter.fltLambda = 1.0F / 20.0F;
        GDFLIB_FilterMAInit_FLT(0.0, &sTransferCharFcn->sIdfbckMA32Filter);
    }

    /* LoopCounter for time keeping */
    sTransferCharFcn->ui16LoopCounter++;

    /* Filter required voltage and feedback current*/
    fltUdReqFilt  = GDFLIB_FilterMA_FLT(*(sTransferCharFcn->pfltUdReq), &sTransferCharFcn->sUdReqMA32Filter);
    fltIdfbckFilt = GDFLIB_FilterMA_FLT(*(sTransferCharFcn->pfltIdfbck), &sTransferCharFcn->sIdfbckMA32Filter);

    /* After 600ms settling of Id start calculation */
    if (sTransferCharFcn->ui16LoopCounter >= (uint16_t)MID_TIME_600MS)
    {
        /* Faults */
        /* Check if Rs is low enough to reach 2A */
        if ((MLIB_Abs_FLT(*(sTransferCharFcn->pfltIdfbck)) < (sTransferCharFcn->fltIdCalib - MID_K_I_50MA)) &&
            (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_TOO_HIGH_RS;
            sTransferCharFcn->bActive   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }
        /* Check if motor is connected */
        if ((MLIB_Abs_FLT(*(sTransferCharFcn->pfltIdfbck)) < MID_K_I_50MA) && (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;
            sTransferCharFcn->bActive   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }

        /* Calculate voltage drop from Rs and Id */
        /* float eq. V_Rs = Rs * Idfbck */
        fltRs_voltage_drop = MLIB_Mul_FLT(sTransferCharFcn->fltRs, fltIdfbckFilt);

        /* Calculate Error voltage and store it to f16ErrorLookUp */
        /* float eq. Error voltage = (Required voltage - Rs voltage drop) / DCbus */
        sTransferCharFcn->fltUdErrorLookUp[sTransferCharFcn->ui16LUTIndex] =
            MLIB_Div_FLT(MLIB_Sub_FLT(fltUdReqFilt, fltRs_voltage_drop), *(sTransferCharFcn->pfltUDCbus));

        /* Prepare for next point measurement */
        sTransferCharFcn->ui16LUTIndex++;
        sTransferCharFcn->fltIdReqActual =
            MLIB_Add_FLT(sTransferCharFcn->fltIdReqActual, sTransferCharFcn->fltIdIncrement);
        *(sTransferCharFcn->pfltIdReq)    = sTransferCharFcn->fltIdReqActual;
        sTransferCharFcn->ui16LoopCounter = 0;

        /* End after last current was measured */
        if (sTransferCharFcn->ui16LUTIndex >= MID_CHAR_CURRENT_POINT_NUMBERS)
        {
            sTransferCharFcn->bActive   = FALSE;
            *(sTransferCharFcn->pfltIdReq) = 0.0F;
        }
    }
}
