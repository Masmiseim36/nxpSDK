/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "acim_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void MCS_DTComp(GMCLIB_2COOR_ALBE_T_FLT *sUAlBeDTComp, GMCLIB_3COOR_T_FLT *sIABC, float_t fltUDcBusFilt);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* dead-time compensation voltage table */
const float_t pfltUDtComp[DTCOMP_TABLE_SIZE] = DTCOMP_TABLE_DATA;

/* dead-time compensation look-up table */
GFLIB_LUT1D_T_FLT sLUTUDtComp;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief ACIM Scalar Open Loop Control
 *
 * This function is used to ACIM Open Loop scalar mode
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 * @param psScalarACIM The pointer of the ACIM scalar parameters structure
 *
 * @return None
 */
void MCS_ACIMOpenLoopScalarCtrlA1(mcs_acim_foc_a1_t *psFocACIM, mcs_scalar_a1_t *psScalarACIM)
{
    /* required speed ramp */
    psScalarACIM->fltSpdMeRamp = GFLIB_Ramp_FLT(psScalarACIM->fltSpdMeReq, &psScalarACIM->sRampParSpdMe);

    /* frequency generation by integrating required speed */
    psScalarACIM->fltPosEl += psScalarACIM->fltGainSpdMe2PosEl * psScalarACIM->fltSpdMeRamp;

    /* limit angle to -pi to pi range */
    if (psScalarACIM->fltPosEl > FLOAT_PI)
        psScalarACIM->fltPosEl -= (2.0F * FLOAT_PI);
    if (psScalarACIM->fltPosEl < -FLOAT_PI)
        psScalarACIM->fltPosEl += (2.0F * FLOAT_PI);

    /* calculate required stator voltage */
    psFocACIM->sUDQReq.fltD = 0.0F;
    psFocACIM->sUDQReq.fltQ =
        GFLIB_Limit_FLT(MLIB_Mul_FLT(psScalarACIM->fltGainRpm2Volt, MLIB_Abs_FLT(psScalarACIM->fltSpdMeRamp)),
                        psScalarACIM->fltUMin, MLIB_Mul_FLT(psFocACIM->fltDutyLim, psFocACIM->fltUDcBus));

    /* calculate sine & cosine for Park's transformation */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLT(psScalarACIM->fltPosEl);
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLT(psScalarACIM->fltPosEl);

    /* call voltage control to calculate PWM duty cycles */
    MCS_ACIMFocCtrlVoltageA1(psFocACIM);
}

/*!
 * @brief ACIM open loop (scalar control).
 *
 * This function is used for motor parameter identification purpose.
 * Required speed is set in Hz and stator voltage is set directly.
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 * @param psScalarACIM The pointer of the ACIM scalar parameters structure
 *
 * @return None
 */
void MCS_ACIMOpenLoopScalarCtrlA2(mcs_acim_foc_a1_t *psFocACIM, mcs_scalar_a1_t *psScalarACIM)
{
    /* frequency generation by integrating required speed */
    psScalarACIM->fltPosEl += psScalarACIM->fltGainHz2PosEl * psScalarACIM->fltSpdMeReq;

    /* limit angle to -pi to pi range */
    if (psScalarACIM->fltPosEl > FLOAT_PI)
        psScalarACIM->fltPosEl -= (2 * FLOAT_PI);
    if (psScalarACIM->fltPosEl < -FLOAT_PI)
        psScalarACIM->fltPosEl += (2 * FLOAT_PI);

    /* limit required voltage */
    psFocACIM->sUDQReq.fltD =
        GFLIB_UpperLimit_FLT(psFocACIM->sUDQReq.fltD, MLIB_Mul_FLT(psFocACIM->fltDutyLim, psFocACIM->fltUDcBus));
    psFocACIM->sUDQReq.fltQ = 0.0F;

    /* calculate sine & cosine for Park's transformation */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLT(psScalarACIM->fltPosEl);
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLT(psScalarACIM->fltPosEl);

    /* call voltage control to calculate PWM duty cycles */
    MCS_ACIMFocCtrlVoltageA1(psFocACIM);
}

/*!
 * @brief ACIM field oriented voltage control
 *
 * This function is used to perform ACIM field oriented voltage control
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlVoltageA1(mcs_acim_foc_a1_t *psFocACIM)
{
    GMCLIB_2COOR_SINCOS_T_FLT sSCFOC;

    /* Clarke's transformation of stator currents */
    GMCLIB_Clark_FLT(&psFocACIM->sIABC, &psFocACIM->sIAlBe);

    /* run rotor flux observer on background */
    AMCLIB_ACIMRotFluxObsrv_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sUAlBe, &psFocACIM->sRFO);

    /* run MRAS speed estimator on background */
    AMCLIB_ACIMSpeedMRAS_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sRFO.sPsiRotSAlBe, psFocACIM->sSpdObs.a32RotPos,
                                &psFocACIM->sSpdObs);

    /* calculate sine & cosine of rotor flux position */
    sSCFOC.fltSin = GFLIB_Sin_FLTa(psFocACIM->sRFO.a32RotFluxPos);
    sSCFOC.fltCos = GFLIB_Cos_FLTa(psFocACIM->sRFO.a32RotFluxPos);

    /* Park's transformation of stator currents */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, &sSCFOC, &psFocACIM->sIDQ);

    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, &psFocACIM->sSCFOC, &psFocACIM->sUAlBe);

    GMCLIB_Park_FLT(&psFocACIM->sUAlBe, &sSCFOC, &psFocACIM->sUDQReqScalar);

    /* dead-time compensation */
    psFocACIM->sUAlBeDTComp = psFocACIM->sUAlBe;
    if (psFocACIM->bFlagDTComp)
    {
        MCS_DTComp(&psFocACIM->sUAlBeDTComp, &psFocACIM->sIABC, psFocACIM->fltUDcBusFilt);
    }

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, &psFocACIM->sUAlBeDTComp, &psFocACIM->sUAlBeCmpFrac);

    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeCmpFrac, &psFocACIM->sDutyABC);
}

/*!
 * @brief ACIM field oriented voltage control
 *
 * This function is used to perform ACIM field oriented voltage control with
 * preset sin and cos of rotor flux position.
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlVoltageA2(mcs_acim_foc_a1_t *psFocACIM)
{
    /* Clarke's transformation of stator currents */
    GMCLIB_Clark_FLT(&psFocACIM->sIABC, &psFocACIM->sIAlBe);

    /* run rotor flux observer on background */
    AMCLIB_ACIMRotFluxObsrv_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sUAlBe, &psFocACIM->sRFO);

    /* run MRAS speed estimator on background */
    AMCLIB_ACIMSpeedMRAS_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sRFO.sPsiRotSAlBe, psFocACIM->sSpdObs.a32RotPos,
                                &psFocACIM->sSpdObs);

    /* calculate sine & cosine of rotor flux position */
    psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLTa(psFocACIM->sRFO.a32RotFluxPos);
    psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLTa(psFocACIM->sRFO.a32RotFluxPos);

    /* Park's transformation of stator currents */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, &psFocACIM->sSCFOC, &psFocACIM->sIDQ);

    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, &psFocACIM->sSCFOC, &psFocACIM->sUAlBe);

    /* dead-time compensation */
    psFocACIM->sUAlBeDTComp = psFocACIM->sUAlBe;
    if (psFocACIM->bFlagDTComp)
    {
        MCS_DTComp(&psFocACIM->sUAlBeDTComp, &psFocACIM->sIABC, psFocACIM->fltUDcBusFilt);
    }

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, &psFocACIM->sUAlBeDTComp, &psFocACIM->sUAlBeCmpFrac);

    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeCmpFrac, &psFocACIM->sDutyABC);
}
/*!
 * @brief ACIM field oriented current control
 *
 * This function is used to perform ACIM field oriented current control
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMFocCtrlCurrentA1(mcs_acim_foc_a1_t *psFocACIM)
{
    /* transform stator phase currents into stator reference frame */
    GMCLIB_Clark_FLT(&(psFocACIM->sIABC), &(psFocACIM->sIAlBe));

    /* do not estimate position if in startup*/
    if (!psFocACIM->bFlagSpdStart)
    {
        /* run rotor flux observer on background */
        AMCLIB_ACIMRotFluxObsrv_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sUAlBe, &psFocACIM->sRFO);

        /* run MRAS speed estimator on background */
        AMCLIB_ACIMSpeedMRAS_FLT_FC(&psFocACIM->sIAlBe, &psFocACIM->sRFO.sPsiRotSAlBe, psFocACIM->sSpdObs.a32RotPos,
                                    &psFocACIM->sSpdObs);

        /* calculate sine & cosine of rotor flux position */
        psFocACIM->sSCFOC.fltSin = GFLIB_Sin_FLTa(psFocACIM->sRFO.a32RotFluxPos);
        psFocACIM->sSCFOC.fltCos = GFLIB_Cos_FLTa(psFocACIM->sRFO.a32RotFluxPos);
    }
    else
    {
        /* force rotor flux position to zero */
        psFocACIM->sSCFOC.fltSin = 0.0F;
        psFocACIM->sSCFOC.fltCos = 1.0F;
    }

    /* transform stator currents in stator ref. frame into rotating rotor
       flux ref. frame */
    GMCLIB_Park_FLT(&psFocACIM->sIAlBe, &psFocACIM->sSCFOC, &psFocACIM->sIDQ);

    /* calculate d-axis voltage limitation */
    psFocACIM->sPIParId.fltLowerLim = MLIB_MulNeg_FLT(psFocACIM->fltDutyLim, psFocACIM->fltUDcBus);
    psFocACIM->sPIParId.fltUpperLim = MLIB_Mul_FLT(psFocACIM->fltDutyLim, psFocACIM->fltUDcBus);

    /* calculate d-axis current control error */
    psFocACIM->sIDQErr.fltD = MLIB_Sub_FLT(psFocACIM->sIDQReq.fltD, psFocACIM->sIDQ.fltD);

    /* calculate d-axis required voltage */
    psFocACIM->sUDQReq.fltD =
        GFLIB_CtrlPIpAW_FLT(psFocACIM->sIDQErr.fltD, &psFocACIM->bFlagPIIdStopInt, &psFocACIM->sPIParId);

    /* calculate q-axis voltage limitation */
    psFocACIM->sPIParIq.fltUpperLim = GFLIB_Sqrt_FLT(psFocACIM->sPIParId.fltUpperLim * psFocACIM->sPIParId.fltUpperLim -
                                                     psFocACIM->sUDQReq.fltD * psFocACIM->sUDQReq.fltD);
    psFocACIM->sPIParIq.fltLowerLim = -psFocACIM->sPIParIq.fltUpperLim;

    /* calculate q-axis current control error */
    psFocACIM->sIDQErr.fltQ = MLIB_Sub_FLT(psFocACIM->sIDQReq.fltQ, psFocACIM->sIDQ.fltQ);

    /* calculate q-axis required voltage */
    psFocACIM->sUDQReq.fltQ =
        GFLIB_CtrlPIpAW_FLT(psFocACIM->sIDQErr.fltQ, &psFocACIM->bFlagPIIqStopInt, &psFocACIM->sPIParIq);

    /* transform required voltage into stator reference frame */
    GMCLIB_ParkInv_FLT(&psFocACIM->sUDQReq, &psFocACIM->sSCFOC, &psFocACIM->sUAlBe);

    /* dead-time compensation */
    psFocACIM->sUAlBeDTComp = psFocACIM->sUAlBe;
    if (psFocACIM->bFlagDTComp)
    {
        MCS_DTComp(&psFocACIM->sUAlBeDTComp, &psFocACIM->sIABC, psFocACIM->fltUDcBusFilt);
    }

    /* DC-bus voltage ripple elimination */
    GMCLIB_ElimDcBusRipFOC_F16ff(psFocACIM->fltUDcBus, &psFocACIM->sUAlBeDTComp, &psFocACIM->sUAlBeCmpFrac);

    /* space vector modulation */
    psFocACIM->ui16SectorSVM = GMCLIB_SvmStd_F16(&psFocACIM->sUAlBeCmpFrac, &psFocACIM->sDutyABC);
}

/*!
 * @brief ACIM FOC with speed and flux control
 *
 * This function is used to perform flux and speed control of ACIM
 *
 * @param psFocACIM The pointer of the ACIM FOC parameters structure
 *
 * @return None
 */
void MCS_ACIMSpeedFluxCtrlA1(mcs_acim_foc_a1_t *psFocACIM, mcs_speed_flux_a1_t *psSpdFlux)
{
    float_t fltIdTmp;

    /* check whether the startup is done */
    if (!psSpdFlux->bStartupDone)
    {
        /* still in startup - keep maximal rotor flux */
        psFocACIM->sIDQReq.fltD = psSpdFlux->fltIdStart;

        /* check for end of startup */
        if (MLIB_Abs_FLT(psSpdFlux->fltSpdMeFilt) > psSpdFlux->fltSpdMeReqMin)
            psSpdFlux->bStartupDone = TRUE;
    }
    else
    {
        /* startup done - employ MTPA and FW */
        /* field weakening */
        fltIdTmp = AMCLIB_CtrlFluxWkng_FLT(psFocACIM->sIDQErr.fltQ, psFocACIM->sUDQReq.fltQ,
                                           psFocACIM->sPIParIq.fltUpperLim, &psSpdFlux->sFluxWkng);

        /* max torque per ampere calculation including exponential filter */
        psSpdFlux->fltIdMTPA = MLIB_Mac_FLT(psSpdFlux->fltIdMTPA, psSpdFlux->fltIdMTPAFiltCoef,
                                            MLIB_Sub_FLT(MLIB_Abs_FLT(psFocACIM->sIDQ.fltQ), psSpdFlux->fltIdMTPA));

        /* limit required MTPA current */
        psSpdFlux->fltIdMTPA = GFLIB_Limit_FLT(psSpdFlux->fltIdMTPA, psSpdFlux->fltIdReqMin, psSpdFlux->fltIdReqMax);

        /* limit d-axis current in case of flux-weakening */
        psFocACIM->sIDQReq.fltD = GFLIB_UpperLimit_FLT(psSpdFlux->fltIdMTPA, fltIdTmp);
    }

    /* required speed ramp */
    if (psSpdFlux->fltSpdMeReq != psSpdFlux->sRampParSpdMe.fltTarget)
    {
        /* recalculate new S-ramp */
        GFLIB_FlexSRampCalcIncr_FLT(psSpdFlux->fltSpdMeReq, psSpdFlux->fltSRampDuration, &psSpdFlux->sRampParSpdMe);
    }
    psSpdFlux->fltSpdMeRamp = GFLIB_FlexSRamp_FLT(&psSpdFlux->sRampParSpdMe);

    /* limit speed controller integrator in case the speed cannot be reached */
    psSpdFlux->bFlagPISpdMeStopInt = (psFocACIM->sPIParId.bLimFlag || psFocACIM->sPIParIq.bLimFlag) &&
                                     (MLIB_Abs_FLT(psSpdFlux->fltSpdMeRamp) > MLIB_Abs_FLT(psSpdFlux->fltSpdMeFilt));

    /* calculate control error */
    psSpdFlux->fltSpdMeErr = MLIB_Sub_FLT(psSpdFlux->fltSpdMeRamp, psSpdFlux->fltSpdMeFilt);

    /* compensation for changing d-axis current */
    fltIdTmp = 1.0F / psFocACIM->sIDQReq.fltD;

    /* speed controller */
    psFocACIM->sIDQReq.fltQ =
        GFLIB_CtrlBetaIPpAW_FLT(psSpdFlux->fltSpdMeRamp * fltIdTmp, psSpdFlux->fltSpdMeFilt * fltIdTmp,
                                &psSpdFlux->bFlagPISpdMeStopInt, &psSpdFlux->sPIParSpdMe);
}

/*!
 * @brief Dead-time compensation
 *
 * This function is used to perform DT compensation using LUT with interpolation
 *
 * @param sUAlBeDTComp The pointer of output compensation voltages
 *        sIABC The pointer to three motor currents
 *        fltUDcBusFilt Filtered DC-Bus voltage
 * @return None
 */
static void MCS_DTComp(GMCLIB_2COOR_ALBE_T_FLT *sUAlBeDTComp, GMCLIB_3COOR_T_FLT *sIABC, float_t fltUDcBusFilt)
{
    register GMCLIB_3COOR_T_FLT sUABCErr;
    register float_t fltUerrMax;
    register int16_t i16CurrSign;

    /* maximal error voltage */
    fltUerrMax = *pfltUDtComp;

    /* compensate phase A */
    i16CurrSign = (sIABC->fltA > DTCOMP_I_RANGE) - (sIABC->fltA < -DTCOMP_I_RANGE);
    if (!i16CurrSign)
        sUABCErr.fltA = GFLIB_Lut1D_FLT(sIABC->fltA, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltA = i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltA) - DTCOMP_I_RANGE) * DTCOMP_LINCOEFF - fltUerrMax);

    /* compensate phase B */
    i16CurrSign = (sIABC->fltB > DTCOMP_I_RANGE) - (sIABC->fltB < -DTCOMP_I_RANGE);
    if (!i16CurrSign)
        sUABCErr.fltB = GFLIB_Lut1D_FLT(sIABC->fltB, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltB = i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltB) - DTCOMP_I_RANGE) * DTCOMP_LINCOEFF - fltUerrMax);

    /* compensate phase C */
    i16CurrSign = (sIABC->fltC > DTCOMP_I_RANGE) - (sIABC->fltC < -DTCOMP_I_RANGE);
    if (!i16CurrSign)
        sUABCErr.fltC = GFLIB_Lut1D_FLT(sIABC->fltC, pfltUDtComp, &sLUTUDtComp);
    else
        sUABCErr.fltC = i16CurrSign * ((MLIB_Abs_FLT(sIABC->fltC) - DTCOMP_I_RANGE) * DTCOMP_LINCOEFF - fltUerrMax);

    /* add compensation voltages */
    sUAlBeDTComp->fltAlpha +=
        (0.333333333333F) * fltUDcBusFilt * (sUABCErr.fltA + sUABCErr.fltA - sUABCErr.fltB - sUABCErr.fltC);

    sUAlBeDTComp->fltBeta += (0.5773502691896F) * fltUDcBusFilt * (sUABCErr.fltB - sUABCErr.fltC);
}
