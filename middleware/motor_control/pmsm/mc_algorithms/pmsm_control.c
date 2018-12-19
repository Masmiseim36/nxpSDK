/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "pmsm_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief PMSM field oriented current control.
 *
 * This function is used to compute PMSM field oriented current control.
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 *
 * @return None
 */
void MCS_PMSMFocCtrl(mcs_pmsm_foc_t *psFocPMSM)
{
    /* pass electrical position from outside function if enabled else estimated
     * electrical position is selected */
    if (psFocPMSM->bPosExtOn)
        psFocPMSM->f16PosEl = psFocPMSM->f16PosElExt;
    else
        psFocPMSM->f16PosEl = psFocPMSM->f16PosElEst;

    /* Position angle of the last PWM update */
    psFocPMSM->sAnglePosEl.f16Sin = GFLIB_Sin_F16(psFocPMSM->f16PosElEst);
    psFocPMSM->sAnglePosEl.f16Cos = GFLIB_Cos_F16(psFocPMSM->f16PosElEst);

    /* 3-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_Clark_F16(&psFocPMSM->sIABC, &psFocPMSM->sIAlBe);

    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GMCLIB_Park_F16(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    GMCLIB_Park_F16(&psFocPMSM->sUAlBeReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUDQEst);

    /* BEMF observer in DQ system */
    psFocPMSM->acc32BemfErr = AMCLIB_PMSMBemfObsrvDQ_F16(&psFocPMSM->sIDQ, 
                                                            &psFocPMSM->sUDQEst, 
                                                            psFocPMSM->f16SpeedElEst, 
                                                            &psFocPMSM->sBemfObsrv);
    
    /* tracking observer calculating speed and position */
    psFocPMSM->f16PosElEst = (frac16_t)AMCLIB_TrackObsrv_F16(psFocPMSM->acc32BemfErr, &psFocPMSM->sTo);

    /* Speed estimation filter  */
    psFocPMSM->f16SpeedElEst =
        GDFLIB_FilterIIR1_F16(MLIB_Conv_F16l(psFocPMSM->sTo.f32Speed), &psFocPMSM->sSpeedElEstFilt);

    /* for open loop control enabled parallel running of observer and FOC
     * open loop electrical position passed to rest of FOC */
    if (psFocPMSM->bOpenLoop || psFocPMSM->bPosExtOn)
    {
        psFocPMSM->sAnglePosEl.f16Sin = GFLIB_Sin_F16(psFocPMSM->f16PosEl);
        psFocPMSM->sAnglePosEl.f16Cos = GFLIB_Cos_F16(psFocPMSM->f16PosEl);
        GMCLIB_Park_F16(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    }

    /* perform current control loop if enabled */
    if (psFocPMSM->bCurrentLoopOn)
    {
        /* D current error calculation */
        psFocPMSM->sIDQError.f16D = MLIB_SubSat_F16(psFocPMSM->sIDQReq.f16D, psFocPMSM->sIDQ.f16D);

        /* Q current error calculation */
        psFocPMSM->sIDQError.f16Q = MLIB_SubSat_F16(psFocPMSM->sIDQReq.f16Q, psFocPMSM->sIDQ.f16Q);

        /*** D - controller limitation calculation ***/
        psFocPMSM->sIdPiParams.f16UpperLim = MLIB_Mul_F16(psFocPMSM->f16DutyCycleLimit, psFocPMSM->f16UDcBusFilt);
        psFocPMSM->sIdPiParams.f16LowerLim = MLIB_Neg_F16(psFocPMSM->sIdPiParams.f16UpperLim);

        /* D current PI controller */
        psFocPMSM->sUDQReq.f16D =
            GFLIB_CtrlPIpAW_F16(psFocPMSM->sIDQError.f16D, &psFocPMSM->bIdPiStopInteg, &psFocPMSM->sIdPiParams);

        /*** Q - controller limitation calculation ***/
        psFocPMSM->sIqPiParams.f16UpperLim = GFLIB_Sqrt_F16(
            MLIB_Sub_F16(MLIB_Mul_F16(psFocPMSM->sIdPiParams.f16UpperLim, psFocPMSM->sIdPiParams.f16UpperLim),
                         MLIB_Mul_F16(psFocPMSM->sUDQReq.f16D, psFocPMSM->sUDQReq.f16D)));
        psFocPMSM->sIqPiParams.f16LowerLim = MLIB_Neg_F16(psFocPMSM->sIqPiParams.f16UpperLim);

        /* Q current PI controller */
        psFocPMSM->sUDQReq.f16Q =
            GFLIB_CtrlPIpAW_F16(psFocPMSM->sIDQError.f16Q, &psFocPMSM->bIqPiStopInteg, &psFocPMSM->sIqPiParams);
    }

    /* 2-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_ParkInv_F16(&psFocPMSM->sUDQReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUAlBeReq);

    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16(psFocPMSM->f16UDcBusFilt, &psFocPMSM->sUAlBeReq, &psFocPMSM->sUAlBeComp);

    /* space vector modulation */
    psFocPMSM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocPMSM->sUAlBeComp, &psFocPMSM->sDutyABC);

    /* End - voltage control */
}

/*!
 * @brief PMSM field oriented speed control.
 *
 * This function is used to compute PMSM field oriented speed control.
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 * @param psSpeed       The pointer of the PMSM speed structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlSpeed(mcs_speed_t *psSpeed)
{
    /* Speed saturation flag given by the Q current controller saturation flag and speed controller saturation flag */
    psSpeed->bSpeedPiStopInteg = (psSpeed->sSpeedPiParams.bLimFlag | psSpeed->bIqPiLimFlag) &
                                 (MLIB_AbsSat_F16(psSpeed->f16SpeedCmd) >= MLIB_AbsSat_F16(psSpeed->f16SpeedFilt));

    /* Speed ramp generation */
    psSpeed->f16SpeedRamp =
        MLIB_Conv_F16l(GFLIB_Ramp_F32(MLIB_Conv_F32s(psSpeed->f16SpeedCmd), &psSpeed->sSpeedRampParams));

    /* Speed error calculation */
    psSpeed->f16SpeedError = MLIB_SubSat_F16(psSpeed->f16SpeedRamp, psSpeed->f16SpeedFilt);

    /* Desired current by the speed PI controller */
    psSpeed->f16IqReq =  GFLIB_CtrlPIpAW_F16(psSpeed->f16SpeedError, 
                                             &psSpeed->bSpeedPiStopInteg, 
                                             &psSpeed->sSpeedPiParams);
}

/*!
 * @brief PMSM 2-step rotor alignment - 120deg in first step and 0deg in second.
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 * @param psAlignment   The pointer of the motor control alignment structure
 *
 * @return None
 */
void MCS_PMSMAlignment(mcs_alignment_t *psAlignment)
{
    /* first half duration time is position set to 120 degree */
    if (psAlignment->ui16TimeHalf > 0)
    {
        if(psAlignment->bDirection) 
            psAlignment->f16PosAlign = psAlignment->f16PosStep1;
        else
            psAlignment->f16PosAlign = psAlignment->f16PosStep2;
        psAlignment->ui16TimeHalf--;
    }
    else
    {
        psAlignment->f16PosAlign = FRAC16(0.0);
    }
}

/*!
 * @brief PMSM Open Loop Start-up
 *
 * This function is used to PMSM Open Loop Start-up
 *
 * @param psStartUp     The pointer of the PMSM open loop start up parameters structure
 *
 * @return None
 */
void MCS_PMSMOpenLoopStartUp(mcs_pmsm_startup_t *psStartUp)
{
    /* Open loop startup speed ramp */
    psStartUp->f16SpeedRampOpenLoop = MLIB_Conv_F16l(GFLIB_Ramp_F32(MLIB_Conv_F32s(psStartUp->f16SpeedReq),
                                                     &psStartUp->sSpeedRampOpenLoopParams));

    /* generation of open loop position from the required speed */
    psStartUp->f16PosGen = GFLIB_Integrator_F16(psStartUp->f16SpeedRampOpenLoop, &psStartUp->sSpeedIntegrator);

    /* position merging starts above merging speed threshold*/
    if (MLIB_AbsSat_F16(psStartUp->f16SpeedRampOpenLoop) >= psStartUp->f16SpeedCatchUp)
    {
        /* increment position merging coefficient */
        psStartUp->f16RatioMerging = MLIB_AddSat_F16(psStartUp->f16RatioMerging, psStartUp->f16CoeffMerging);

        /* merging equation */
        psStartUp->f16PosMerged = MLIB_Add_F16(
            psStartUp->f16PosGen,
            MLIB_Mul_F16(MLIB_Sub_F16(psStartUp->f16PosEst, psStartUp->f16PosGen), psStartUp->f16RatioMerging));
    }
    else
    {
        psStartUp->f16PosMerged = psStartUp->f16PosGen;
    }

    /* clear open loop flag */
    if (psStartUp->f16RatioMerging == FRAC16(1.0))
        psStartUp->bOpenLoop = FALSE;
}

/*!
 * @brief PMSM scalar control, voltage is set based on required speed
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psFocPMSM      The pointer of the PMSM FOC structure
 * @param psScalarPMSM   The pointer of the PMSM scalar control structure
 *
 * @return None
 */
void MCS_PMSMScalarCtrl(mcs_pmsm_scalar_ctrl_t *psScalarPMSM)
{
    /* this part of code is executed when scalar control is turned-on */
    /* frequency ramp */
    psScalarPMSM->f16FreqRamp =
        MLIB_Conv_F16l(GFLIB_Ramp_F32(MLIB_Conv_F32s(psScalarPMSM->f16FreqCmd), &psScalarPMSM->sFreqRampParams));

    /* voltage calculation */
    psScalarPMSM->sUDQReq.f16Q = (frac16_t)(MLIB_ShLSat_F16(
        MLIB_Mul_F16(psScalarPMSM->f16VHzGain, psScalarPMSM->f16FreqRamp), psScalarPMSM->f16VHzGainShift));
    psScalarPMSM->sUDQReq.f16D = 0;

    /* stator voltage angle , used the same integrator as for the open-loop start up*/
    psScalarPMSM->f16PosElScalar = GFLIB_Integrator_F16(psScalarPMSM->f16FreqRamp, &psScalarPMSM->sFreqIntegrator);

}

