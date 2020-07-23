/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2020 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rs_measure.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Rs measurement routine
 *
 * @param *sRsMeasFcn input structure of type #mid_get_rs_t for passing
 *                          all necessary parameters.
 *
 * @return None
 */
void MID_getRs(mid_get_rs_t* sRsMeasFcn, mid_get_char_t* sTransferCharFcn)
{
    GMCLIB_2COOR_DQ_T_FLT sUDQReq;
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;
    GMCLIB_2COOR_DQ_T_FLT sIDQfbck;
    float_t               fltRsUdReqFilt, fltRsIdfbckFilt;

    /* Initialization */
    if(sRsMeasFcn->ui16Active == 0U)
    {
        sRsMeasFcn->ui16Active              = TRUE;
        sRsMeasFcn->ui16LoopCounter         = 0U;
        sRsMeasFcn->fltRs                   = 0.0F;
        sRsMeasFcn->sUdReqMA32Filter.fltLambda  = MID_RS_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sRsMeasFcn->sUdReqMA32Filter);
        sRsMeasFcn->sIdfbckMA32Filter.fltLambda = MID_RS_FILT_MA_LAMBDA;
        GDFLIB_FilterMAInit_FLT(0.0F, &sRsMeasFcn->sIdfbckMA32Filter);

        /* Set the measuring current Id_meas*/
        sIDQReq.fltD = sRsMeasFcn->fltIdMeas;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);
    }

    /* LoopCounter for time keeping */
    sRsMeasFcn->ui16LoopCounter++;

    /* Read UDQReq and IDQfbck */
    MID_MC_GetUDQReq(&sUDQReq);
    MID_MC_GetIDQfbck(&sIDQfbck);

    /* Filter required voltage and feedback current*/
    fltRsUdReqFilt  = GDFLIB_FilterMA_FLT(sUDQReq.fltD, &sRsMeasFcn->sUdReqMA32Filter);
    fltRsIdfbckFilt = GDFLIB_FilterMA_FLT(sIDQfbck.fltD, &sRsMeasFcn->sIdfbckMA32Filter);

    /* After 1200ms start calculation */
    if(sRsMeasFcn->ui16LoopCounter == MID_TIME_1200MS)
    {
        /* Check whether the measurement current is above HW characterization table range. */
        if(fltRsIdfbckFilt >= sTransferCharFcn->fltIdCalib)
        {
            /* Use the maximal correction voltage value. */
            sTransferCharFcn->ui16LUTIndex = MID_CHAR_CURRENT_POINT_NUMBERS - 1U;
        }
        /* Check whether the measurement current is below HW characterization table range. */
        else if(fltRsIdfbckFilt <= MLIB_Neg_FLT(sTransferCharFcn->fltIdCalib))
        {
            /* Use the minimal correction voltage value. */
            sTransferCharFcn->ui16LUTIndex = 0U;
        }
        else
        {
            /* Calculate the correction voltage table index. Reuse fltRs as temporary variable. */
            sRsMeasFcn->fltRs = ((float_t)(MID_CHAR_CURRENT_POINT_NUMBERS) * 0.5F) *
                MLIB_Div_FLT((fltRsIdfbckFilt + sTransferCharFcn->fltIdCalib),
                              sTransferCharFcn->fltIdCalib);
            sTransferCharFcn->ui16LUTIndex = (uint16_t)sRsMeasFcn->fltRs;
        }

        /* Correct the measurement voltage. */
        fltRsUdReqFilt -= (g_pfltMIDUdErrorLookUp[sTransferCharFcn->ui16LUTIndex] *
                           MID_MC_GetUDCbusFilt());

        /* Calculate Rs from Ud_correct and Id */
        /* float eq. Rs = UdCorrect / Idfbck */
        sRsMeasFcn->fltRs = MLIB_Div_FLT(fltRsUdReqFilt, fltRsIdfbckFilt);

        /* Set required current to zero */
        sIDQReq.fltD = 0.0F;
        sIDQReq.fltQ = 0.0F;
        MID_MC_SetIDQReq(sIDQReq);

        /* Check Faults */
        /* Check if motor is connected */
        if(MLIB_Abs_FLT(sIDQfbck.fltD) < MID_K_I_50MA)
        {
            g_sMID.ui16FaultMID |= MID_FAULT_NO_MOTOR;
        }

        /* Check if Rs is negative or saturated*/
        if(sRsMeasFcn->fltRs < 0.0F)
        {
            g_sMID.ui16WarnMID |= MID_WARN_RS_OUT_OF_RANGE;
        }

        /* Check if measuring current was reached */
        if(sIDQfbck.fltD < MLIB_Sub_FLT(sRsMeasFcn->fltIdMeas, MID_K_I_50MA))
        {
            g_sMID.ui16WarnMID |= MID_WARN_DC_CUR_NOT_REACHED;
        }
    }

    /* Wait additional 1200ms to stabilize Id at 0A */
    /* Exit the function after 2400ms */
    if(sRsMeasFcn->ui16LoopCounter > MID_TIME_2400MS)
    {
        sRsMeasFcn->ui16Active = FALSE;
    }
}

