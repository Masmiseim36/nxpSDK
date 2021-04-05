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
void MID_GetTransferCharacteristic(mid_get_char_t* sTransferCharFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sUDQReq;
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;
    GMCLIB_2COOR_DQ_T_FLT sIDQfbck;
    float_t fltRs_voltage_drop;      /* Auxiliary variable for Rs voltage drop calculation */
    float_t fltUdReqFilt;            /* Filtered Ud required value */
    float_t fltIdfbckFilt;           /* Filtered Id feedback value */


    /* Initialisation */
    if(sTransferCharFcn->ui16Active == 0U)
    {
        sTransferCharFcn->ui16Active      = TRUE;
        sTransferCharFcn->ui16LoopCounter = 0U;
        sTransferCharFcn->fltIdReqActual  = MLIB_Neg_FLT(sTransferCharFcn->fltIdCalib);
        sIDQReq.fltD = sTransferCharFcn->fltIdReqActual;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);
        sTransferCharFcn->ui16LUTIndex    = 0U;
        sTransferCharFcn->sUdReqMA32Filter.fltLambda = MID_RS_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sTransferCharFcn->sUdReqMA32Filter);
        sTransferCharFcn->sIdfbckMA32Filter.fltLambda = MID_RS_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sTransferCharFcn->sIdfbckMA32Filter);
        sTransferCharFcn->fltIdIncrement = sTransferCharFcn->fltIdCalib * 2.0F / (float_t)MID_CHAR_CURRENT_POINT_NUMBERS;
    }

    /* LoopCounter for time keeping */
    sTransferCharFcn->ui16LoopCounter++;

    /* Read UDQReq and IDQfbck */
    MID_MC_GetUDQReq(&sUDQReq);
    MID_MC_GetIDQfbck(&sIDQfbck);

    /* Filter required voltage and feedback current*/
    fltUdReqFilt       = GDFLIB_FilterMA_FLT(sUDQReq.fltD, &sTransferCharFcn->sUdReqMA32Filter);
    fltIdfbckFilt      = GDFLIB_FilterMA_FLT(sIDQfbck.fltD, &sTransferCharFcn->sIdfbckMA32Filter);

    /* After 600ms settling of Id start calculation */
    if(sTransferCharFcn->ui16LoopCounter >= MID_TIME_600MS)
    {
        /* Faults */
        /* Check if Rs is low enough to reach 2A */
        if((MLIB_Abs_FLT(sIDQfbck.fltD) < (sTransferCharFcn->fltIdCalib - MID_K_I_50MA)) && (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_TOO_HIGH_RS;
            sTransferCharFcn->ui16Active   = FALSE;
            sIDQReq.fltD = 0.0F;
            sIDQReq.fltQ = 0.0F;
            MID_MC_SetIDQReq(sIDQReq);
        }
        /* Check if motor is connected */
        if((MLIB_Abs_FLT(sIDQfbck.fltD) < MID_K_I_50MA) && (sTransferCharFcn->ui16LUTIndex == 0U))
        {
            g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;
            sTransferCharFcn->ui16Active   = FALSE;
            sIDQReq.fltD = 0.0F;
            sIDQReq.fltQ = 0.0F;
            MID_MC_SetIDQReq(sIDQReq);
        }

        /* Calculate voltage drop from Rs and Id */
        /* float eq. V_Rs = Rs * Idfbck */
        fltRs_voltage_drop = MLIB_Mul_FLT(sTransferCharFcn->fltRs, fltIdfbckFilt);

        /* Calculate Error voltage and store it to f16ErrorLookUp */
        /* float eq. Error voltage = (Required voltage - Rs voltage drop) / DCbus */
        g_pfltMIDUdErrorLookUp[sTransferCharFcn->ui16LUTIndex] = MLIB_Div_FLT(MLIB_Sub_FLT(fltUdReqFilt, fltRs_voltage_drop), MID_MC_GetUDCbusFilt());

        /* Prepare for next point measurement */
        sTransferCharFcn->ui16LUTIndex++;
        sTransferCharFcn->fltIdReqActual = MLIB_Add_FLT(sTransferCharFcn->fltIdReqActual, sTransferCharFcn->fltIdIncrement);
        sIDQReq.fltD = sTransferCharFcn->fltIdReqActual;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);
        sTransferCharFcn->ui16LoopCounter = 0U;

        /* End after last current was measured */
        if(sTransferCharFcn->ui16LUTIndex >= MID_CHAR_CURRENT_POINT_NUMBERS)
        {
            sTransferCharFcn->ui16Active   = FALSE;
            sIDQReq.fltD = 0.0F;
            sIDQReq.fltQ = 0.0F;
            MID_MC_SetIDQReq(sIDQReq);
        }
    }
}

