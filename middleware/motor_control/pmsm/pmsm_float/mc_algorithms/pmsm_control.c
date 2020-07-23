/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
/* dead-time compensation voltage table */
float_t pfltUDtComp[DTCOMP_TABLE_SIZE] = DTCOMP_TABLE_DATA;

/* dead-time compensation look-up table */
GFLIB_LUT1D_T_FLT sLUTUDtComp;

static void MCS_DTComp(GMCLIB_2COOR_ALBE_T_FLT *sUAlBeDTComp,
                       GMCLIB_3COOR_T_FLT *sIABC,
                       float_t fltUDcBusFilt,
                       float_t fltPwrStgCharIRange,
                       float_t fltPwrStgCharLinCoeff);

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
    psFocPMSM->sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)psFocPMSM->f16PosElEst);
    psFocPMSM->sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)psFocPMSM->f16PosElEst);

    /* 3-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_Clark_FLT(&psFocPMSM->sIABC, &psFocPMSM->sIAlBe);

    /* 2-phase to 2-phase transformation to rotary ref. frame */
    GMCLIB_Park_FLT(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    GMCLIB_Park_FLT(&psFocPMSM->sUAlBeReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUDQEst);

    /* BEMF observer in DQ system */
    psFocPMSM->acc32BemfErr = AMCLIB_PMSMBemfObsrvDQ_A32fff(&psFocPMSM->sIDQ, &psFocPMSM->sUDQEst,
                                                            psFocPMSM->fltSpeedElEst, &psFocPMSM->sBemfObsrv);

    psFocPMSM->f16PosElEst = (frac16_t)AMCLIB_TrackObsrv_A32af(psFocPMSM->acc32BemfErr, &psFocPMSM->sTo);

    /* Speed estimation filter  */
    psFocPMSM->fltSpeedElEst = GDFLIB_FilterIIR1_FLT(psFocPMSM->sTo.fltSpeed, &psFocPMSM->sSpeedElEstFilt);

    /* for open loop control enabled parallel running of observer and FOC
     * open loop electrical position passed to rest of FOC */
    if (psFocPMSM->bOpenLoop || psFocPMSM->bPosExtOn)
    {
        psFocPMSM->sAnglePosEl.fltSin = GFLIB_Sin_FLTa((acc32_t)psFocPMSM->f16PosEl);
        psFocPMSM->sAnglePosEl.fltCos = GFLIB_Cos_FLTa((acc32_t)psFocPMSM->f16PosEl);
        GMCLIB_Park_FLT(&psFocPMSM->sIAlBe, &psFocPMSM->sAnglePosEl, &psFocPMSM->sIDQ);
    }

    /* perform current control loop if enabled */
    if (psFocPMSM->bCurrentLoopOn)
    {
        /* D current error calculation */
        psFocPMSM->sIDQError.fltD = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltD, psFocPMSM->sIDQ.fltD);

        /* Q current error calculation */
        psFocPMSM->sIDQError.fltQ = MLIB_Sub_FLT(psFocPMSM->sIDQReq.fltQ, psFocPMSM->sIDQ.fltQ);

        /*** D - controller limitation calculation ***/
        psFocPMSM->sIdPiParams.fltLowerLim = MLIB_MulNeg_FLT(psFocPMSM->fltDutyCycleLimit, psFocPMSM->fltUDcBusFilt);
        psFocPMSM->sIdPiParams.fltUpperLim = MLIB_Mul_FLT(psFocPMSM->fltDutyCycleLimit, psFocPMSM->fltUDcBusFilt);

        /* D current PI controller */
        psFocPMSM->sUDQReq.fltD =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltD, &psFocPMSM->bIdPiStopInteg, &psFocPMSM->sIdPiParams);

        /*** Q - controller limitation calculation ***/
        psFocPMSM->sIqPiParams.fltUpperLim =
            GFLIB_Sqrt_FLT(psFocPMSM->sIdPiParams.fltUpperLim * psFocPMSM->sIdPiParams.fltUpperLim -
                           psFocPMSM->sUDQReq.fltD * psFocPMSM->sUDQReq.fltD);
        psFocPMSM->sIqPiParams.fltLowerLim = MLIB_Neg_FLT(psFocPMSM->sIqPiParams.fltUpperLim);

        /* Q current PI controller */
        psFocPMSM->sUDQReq.fltQ =
            GFLIB_CtrlPIpAW_FLT(psFocPMSM->sIDQError.fltQ, &psFocPMSM->bIqPiStopInteg, &psFocPMSM->sIqPiParams);
    }

    /* 2-phase to 2-phase transformation to stationary ref. frame */
    GMCLIB_ParkInv_FLT(&psFocPMSM->sUDQReq, &psFocPMSM->sAnglePosEl, &psFocPMSM->sUAlBeReq);

    /* dead-time compensation */
    psFocPMSM->sUAlBeDTComp = psFocPMSM->sUAlBeReq;
    if (psFocPMSM->bFlagDTComp)
    {
        MCS_DTComp(&psFocPMSM->sUAlBeDTComp, &psFocPMSM->sIABC, psFocPMSM->fltUDcBusFilt,
                   psFocPMSM->fltPwrStgCharIRange, psFocPMSM->fltPwrStgCharLinCoeff);
    }

    /* DCBus ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocPMSM->fltUDcBusFilt, &psFocPMSM->sUAlBeDTComp, &psFocPMSM->sUAlBeCompFrac);

    /* space vector modulation */
    psFocPMSM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocPMSM->sUAlBeCompFrac, &psFocPMSM->sDutyABC);
}

/*!
 * @brief PMSM field oriented speed control.
 *
 * This function is used to compute PMSM field oriented speed control.
 *
 * @param psSpeed       The pointer of the PMSM speed structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlSpeed(mcs_speed_t *psSpeed)
{
    /* Speed saturation flag given by the Q current controller saturation flag and speed controller saturation flag */
    psSpeed->bSpeedPiStopInteg = (psSpeed->sSpeedPiParams.bLimFlag | psSpeed->bIqPiLimFlag) &
                                 (MLIB_Abs_FLT(psSpeed->fltSpeedCmd) >= MLIB_Abs_FLT(psSpeed->fltSpeedFilt));

    /* Speed ramp generation */
    psSpeed->fltSpeedRamp = GFLIB_Ramp_FLT(psSpeed->fltSpeedCmd, &psSpeed->sSpeedRampParams);

    /* Speed error calculation */
    psSpeed->fltSpeedError = MLIB_Sub_FLT(psSpeed->fltSpeedRamp, psSpeed->fltSpeedFilt);

    /* Desired current by the speed PI controller */
    psSpeed->fltIqReq =
        GFLIB_CtrlPIpAW_FLT(psSpeed->fltSpeedError, &psSpeed->bSpeedPiStopInteg, &psSpeed->sSpeedPiParams);
}

/*!
 * @brief PMSM field oriented position control.
 *
 * This function is used to compute PMSM field oriented position control.
 *
 * @param psSpeed       The pointer of the PMSM position structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlPosition(mcs_position_t *psPosition)
{
    /* Position error calculation */
    psPosition->a32PositionError = ((psPosition->a32PositionCmd) - (psPosition->a32Position));
    /* Position P controller output */
    psPosition->f16SpeedReq = MLIB_MulSat_F16as((psPosition->a32PositionError), (psPosition->f16PositionPGain));
}

/*!
 * @brief PMSM 2-step rotor alignment - 120deg in first step and 0deg in second.
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psAlignment   The pointer of the motor control alignment structure
 *
 * @return None
 */
void MCS_PMSMAlignment(mcs_alignment_t *psAlignment)
{
    /* first half duration time is position set to 120 degree */
    if (psAlignment->ui16TimeHalf > 0)
    {
        psAlignment->f16PosAlign = FRAC16(120.0 / 180.0);
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
    psStartUp->fltSpeedRampOpenLoop = GFLIB_Ramp_FLT(psStartUp->fltSpeedReq, &psStartUp->sSpeedRampOpenLoopParams);

    /* generation of open loop position from the required speed */
    psStartUp->f16PosGen = GFLIB_Integrator_F16(
        MLIB_ConvSc_F16ff(psStartUp->fltSpeedRampOpenLoop, psStartUp->fltSpeedMax), &psStartUp->sSpeedIntegrator);

    /* position merging starts above merging speed threshold*/
    if (MLIB_Abs_FLT(psStartUp->fltSpeedRampOpenLoop) >= psStartUp->fltSpeedCatchUp)
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
 * @param psScalarPMSM   The pointer of the PMSM scalar control structure
 *
 * @return None
 */
void MCS_PMSMScalarCtrl(mcs_pmsm_scalar_ctrl_t *psScalarPMSM)
{
    /* this part of code is executed when scalar control is turned-on */
    /* frequency ramp */
    psScalarPMSM->fltFreqRamp = GFLIB_Ramp_FLT(psScalarPMSM->fltFreqCmd, &psScalarPMSM->sFreqRampParams);

    /* voltage calculation */
    psScalarPMSM->sUDQReq.fltQ =
        GFLIB_LowerLimit_FLT((psScalarPMSM->fltVHzGain * psScalarPMSM->fltFreqRamp), psScalarPMSM->fltUqMin);
    psScalarPMSM->sUDQReq.fltD = 0.0F;

    /* stator voltage angle , used the same integrator as for the open-loop start up*/
    psScalarPMSM->f16PosElScalar = GFLIB_Integrator_F16(
        MLIB_ConvSc_F16ff(psScalarPMSM->fltFreqRamp, psScalarPMSM->fltFreqMax), &psScalarPMSM->sFreqIntegrator);
}

/******************************************************************************
@brief   Dead-time compensation using LUT wit interpolation

@param   N/A

@return  N/A
******************************************************************************/
static void MCS_DTComp(GMCLIB_2COOR_ALBE_T_FLT *sUAlBeDTComp,
                       GMCLIB_3COOR_T_FLT *sIABC,
                       float_t fltUDcBusFilt,
                       float_t fltPwrStgCharIRange,
                       float_t fltPwrStgCharLinCoeff)
{
    register GMCLIB_3COOR_T_FLT sUABCErr;
    register float_t fltUerrMax;
    register int16_t i16CurrSign;

    /* maximal error voltage */
    fltUerrMax = *pfltUDtComp;

    /* compensate phase A */
    i16CurrSign = (sIABC->fltA > fltPwrStgCharIRange) - (sIABC->fltA < -fltPwrStgCharIRange);
    if (!i16CurrSign)
        sUABCErr.fltA = GFLIB_Lut1D_FLT(sIABC->fltA, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltA =
            i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltA) - fltPwrStgCharIRange) * fltPwrStgCharLinCoeff - fltUerrMax);

    /* compensate phase B */
    i16CurrSign = (sIABC->fltB > fltPwrStgCharIRange) - (sIABC->fltB < -fltPwrStgCharIRange);
    if (!i16CurrSign)
        sUABCErr.fltB = GFLIB_Lut1D_FLT(sIABC->fltB, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltB =
            i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltB) - fltPwrStgCharIRange) * fltPwrStgCharLinCoeff - fltUerrMax);

    /* compensate phase C */
    i16CurrSign = (sIABC->fltC > fltPwrStgCharIRange) - (sIABC->fltC < -fltPwrStgCharIRange);
    if (!i16CurrSign)
        sUABCErr.fltC = GFLIB_Lut1D_FLT(sIABC->fltC, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltC =
            i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltC) - fltPwrStgCharIRange) * fltPwrStgCharLinCoeff - fltUerrMax);

    /* add compensation voltages */
    sUAlBeDTComp->fltAlpha +=
        (0.333333333333F) * fltUDcBusFilt * (sUABCErr.fltA + sUABCErr.fltA - sUABCErr.fltB - sUABCErr.fltC);

    sUAlBeDTComp->fltBeta += (0.5773502691896F) * fltUDcBusFilt * (sUABCErr.fltB - sUABCErr.fltC);
}
