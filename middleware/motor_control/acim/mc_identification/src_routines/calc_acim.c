/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "calc_acim.h"

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Calculate ACIM electrical parameters
 *
 * @param None
 *
 * @return None
 */
void MID_CalcElPar(void)
{
    float_t fltR;
    float_t fltXBlocked;
    float_t fltXNoLoad;
    float_t fltX1;
    float_t fltX2;
    float_t fltLl1;
    float_t fltLl2;
    float_t fltXm;
    float_t fltLeak;
    float_t fltTauRot;
    float_t fltTmp;

    /* auxiliary calculations */
    fltTmp      = 1.0F / (g_sMID.sBlocked.fltIrms * g_sMID.sBlocked.fltIrms);
    fltR        = g_sMID.sBlocked.fltP * fltTmp;
    fltXBlocked = g_sMID.sBlocked.fltQ * fltTmp;
    fltXNoLoad  = g_sMID.sNoLoad.fltQ / (g_sMID.sNoLoad.fltIrms * g_sMID.sNoLoad.fltIrms);
    fltX1       = fltXBlocked * 0.5F;
    fltX2       = fltXBlocked - fltX1;
    fltTmp      = 1.0F / (2.0F * FLOAT_PI * g_sMID.sPar.fltFreqN);
    fltLl1      = fltX1 * fltTmp;
    fltLl2      = fltX2 * fltTmp;
    fltXm       = fltXNoLoad - fltX1;

    /* rotor resistance */
    g_sMID.sPar.fltRr = (fltR - g_sMID.sPar.fltRs) * (fltX2 + fltXm) * (fltX2 + fltXm) / (fltXm * fltXm);

    /* mutual inductance */
    g_sMID.sPar.fltLm = fltXm * fltTmp;

    /* stator inductance */
    g_sMID.sPar.fltLs = g_sMID.sPar.fltLm + fltLl1;

    /* rotor inductance */
    g_sMID.sPar.fltLr = g_sMID.sPar.fltLm + fltLl2;

    /* leakage inductance */
    fltLeak = 1.0F - (g_sMID.sPar.fltLm * g_sMID.sPar.fltLm) / (g_sMID.sPar.fltLs * g_sMID.sPar.fltLr);

    /* calculate current controllers parameters */
    g_sMID.sAlgNew.fltIdPIKp = 4.0F * FLOAT_PI * g_sMID.fltFreqCLoop * g_sMID.sPar.fltLs * fltLeak - g_sMID.sPar.fltRs;
    g_sMID.sAlgNew.fltIdPIKi = 4.0F * FLOAT_PI * FLOAT_PI * M1_FAST_LOOP_TS * g_sMID.fltFreqCLoop *
                               g_sMID.fltFreqCLoop * g_sMID.sPar.fltLs * fltLeak;
    g_sMID.sAlgNew.fltIqPIKp = g_sMID.sAlgNew.fltIdPIKp;
    g_sMID.sAlgNew.fltIqPIKi = g_sMID.sAlgNew.fltIdPIKi;

    /* calculate rotor flux observer parameters */
    fltTauRot                         = g_sMID.sPar.fltLr / g_sMID.sPar.fltRr;
    fltTmp                            = 1.0F / (fltTauRot + M1_FAST_LOOP_TS);
    g_sMID.sAlgNew.fltPsiRA1Gain      = fltTauRot * fltTmp;
    g_sMID.sAlgNew.fltPsiRB1Gain      = g_sMID.sPar.fltLm * M1_FAST_LOOP_TS * fltTmp;
    g_sMID.sAlgNew.fltPsiSA1Gain      = (0.1591549431F) / (0.1591549431F + M1_FAST_LOOP_TS);
    g_sMID.sAlgNew.fltPsiSA2Gain      = g_sMID.sAlgNew.fltPsiSA1Gain * M1_FAST_LOOP_TS;
    g_sMID.sAlgNew.fltKrInvGain       = g_sMID.sPar.fltLr / g_sMID.sPar.fltLm;
    g_sMID.sAlgNew.fltKrLsTotLeakGain = g_sMID.sAlgNew.fltKrInvGain * g_sMID.sPar.fltLs * fltLeak;
    g_sMID.sAlgNew.fltRsEst           = g_sMID.sPar.fltRs;
    g_sMID.sAlgNew.fltGainTrq         = (1.5F * g_sMID.sPar.fltPp * g_sMID.sPar.fltLm) / g_sMID.sPar.fltLr;

    /* calculate MRAS speed observer parameters */
    g_sMID.sAlgNew.fltMRASPsiRA1Gain = g_sMID.sAlgNew.fltPsiRA1Gain;
    g_sMID.sAlgNew.fltMRASPsiRB1Gain = g_sMID.sAlgNew.fltPsiRB1Gain;
    g_sMID.sAlgNew.fltGainSpd        = MID_CALC_RAD2RPM / g_sMID.sPar.fltPp;

    /* calculate current limits for flux and speed control */
    g_sMID.sAlgNew.fltIdMax = (g_sMID.sNoLoad.fltUMeasMax * 0.1591549F) / /* set maximal d-axis current */
                              (g_sMID.sPar.fltLm * g_sMID.sPar.fltFreqN);
    if (g_sMID.sAlgNew.fltIdMax > g_sMID.sPar.fltIphN) /* if calculated value is bigger than nominal, limit it */
        g_sMID.sAlgNew.fltIdMax = g_sMID.sPar.fltIphN;
    g_sMID.sAlgNew.fltIdMin   = MID_CALC_ID_MIN2MAX_RATIO * g_sMID.sAlgNew.fltIdMax; /* set minimal d-axis current */
    g_sMID.sAlgNew.fltIdStart = g_sMID.sAlgNew.fltIdMax; /* set startup d-axis current equal to maximal value */
    g_sMID.sAlgNew.fltIMax =
        GFLIB_Sqrt_FLT(2.0F * g_sMID.sPar.fltIphN * g_sMID.sPar.fltIphN -
                       g_sMID.sAlgNew.fltIdMax * g_sMID.sAlgNew.fltIdMax); /* set maximal q-axis stator current*/

    /* calculate flux-weakening PI controller gains */
    fltTmp                   = 2.0F * FLOAT_PI * MID_CALC_FW_BANDWIDTH / (g_sMID.sPar.fltLm * g_sMID.sPar.fltRr);
    g_sMID.sAlgNew.fltFWPIKp = g_sMID.sPar.fltLr * fltTmp; /* calculate flux-weakening controller proportional gain */
    g_sMID.sAlgNew.fltFWPIKi =
        M1_SLOW_LOOP_TS * g_sMID.sPar.fltRr * fltTmp; /* calculate flux-weakening controller integral gain */

    /* prepare mechanical parameters measurement */
    g_sMID.sMech.fltGainTrq = g_sMID.sAlgNew.fltGainTrq * g_sMID.sPar.fltLm * g_sMID.sAlgNew.fltIdMax;
    g_sMID.sMech.fltIqAccl  = g_sMID.sMech.fltTrqAccl / g_sMID.sMech.fltGainTrq;

    /* enable dead-time compensation */
    g_sMID.sAlgNew.bFlagDTComp = TRUE;

    /* electrical parameter measurement done */
    g_sMID.eCalcElPar = kMID_CalcDone;
}

/*!
 * @brief Calculate ACIM mechanical parameters
 *
 * @param None
 *
 * @return None
 */
void MID_CalcMechPar(void)
{
    float_t fltTmp;

    /* calculate moment of inertia  */
    g_sMID.sPar.fltJ =
        g_sMID.sPar.fltPp * g_sMID.sMech.fltTrqInteg * g_sMID.sPar.fltTauMech /
        (g_sMID.sPar.fltTauMech * (g_sMID.sMech.fltSpdElEnd - g_sMID.sMech.fltSpdElStart) + g_sMID.sMech.fltSpdInteg);

    /* speed controller parameters */
    fltTmp = 1.0F / (g_sMID.sAlgNew.fltGainTrq * g_sMID.sPar.fltLm);
    g_sMID.sAlgNew.fltSpdPIKp =
        MID_CALC_RPM2RAD * (4.0F * FLOAT_PI * g_sMID.fltFreqSLoop - g_sMID.sPar.fltTauMech) * g_sMID.sPar.fltJ * fltTmp;
    g_sMID.sAlgNew.fltSpdPIKi = MID_CALC_RPM2RAD * 4.0F * FLOAT_PI * FLOAT_PI * g_sMID.fltFreqSLoop *
                                g_sMID.fltFreqSLoop * g_sMID.sPar.fltJ * M1_SLOW_LOOP_TS * fltTmp;

    /* required speed limitation */
    fltTmp                        = 60.0F * g_sMID.sPar.fltFreqN / g_sMID.sPar.fltPp;
    g_sMID.sAlgNew.fltSpdMeReqMax = MID_CALC_SPDREQ_MAX * fltTmp;
    g_sMID.sAlgNew.fltSpdMeReqMin = MID_CALC_SPDREQ_MIN * fltTmp;

    /* scalar control */
    g_sMID.sAlgNew.fltGainSpdMe2PosEl = MID_CALC_RPM2RAD * g_sMID.sPar.fltPp * M1_FAST_LOOP_TS;
    g_sMID.sAlgNew.fltGainRpm2Volt    = g_sMID.sNoLoad.fltUMeasMax / g_sMID.sAlgNew.fltSpdMeReqMax;

    /* electrical parameter measurement done */
    g_sMID.eCalcMechPar = kMID_CalcDone;
}
