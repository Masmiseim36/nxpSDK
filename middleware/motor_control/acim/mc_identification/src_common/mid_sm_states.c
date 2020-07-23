/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "mid_sm_states.h"

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* motor parameters identification state machine functions */
static void MID_StateStart(void);
static void MID_StateStop(void);
static void MID_StatePwrStgCharact(void);
static void MID_StateRs(void);
static void MID_StateNoLoad(void);
static void MID_StateBlocked(void);
static void MID_StateMech(void);

/* motor parameters identification state-transition functions */
static void MID_TransStart2PwrStgCharact(void);
static void MID_TransStart2Rs(void);
static void MID_TransPwrStgCharact2Stop(void);
static void MID_TransRs2NoLoad(void);
static void MID_TransNoLoad2Blocked(void);
static void MID_TransBlocked2Mech(void);
static void MID_TransMech2Stop(void);
static void MID_TransAll2Stop(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* state machine functions field */
static const mid_sm_app_state_fcn_t msSTATE = {MID_StateStart,  MID_StateStop,    MID_StatePwrStgCharact, MID_StateRs,
                                               MID_StateNoLoad, MID_StateBlocked, MID_StateMech};

/* state-transition functions field */
/* note: order of the transition function should be the same as in definition
 * of the mid_sm_app_trans_fcn_t type */
static const mid_sm_app_trans_fcn_t msTRANS = {
    MID_TransStart2PwrStgCharact, MID_TransStart2Rs,     MID_TransPwrStgCharact2Stop, MID_TransRs2NoLoad,
    MID_TransNoLoad2Blocked,      MID_TransBlocked2Mech, MID_TransMech2Stop,          MID_TransAll2Stop};

/* state machine structure definition and initialization */
mid_sm_app_ctrl_t g_g_sMIDCtrl = {
    /* g_g_sMIDCtrl.psState, user state functions  */
    &msSTATE,
    /* g_g_sMIDCtrl.psTrans, user state-transition functions */
    &msTRANS,
    /* g_g_sMIDCtrl.uiCtrl, default no control command */
    MID_SM_CTRL_NONE,
    /* g_g_sMIDCtrl.eState, default state after reset */
    kMID_Start};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief MID START state
 *
 * @param None
 *
 * @return None
 */
static void MID_StateStart(void)
{
    /* type the code to do when in the START state */
    g_sMID.sRs.ui16Active       = FALSE;
    g_sMID.sNoLoad.ui16Active   = FALSE;
    g_sMID.sBlocked.ui16Active  = FALSE;
    g_sMID.sPwrStgCh.ui16Active = FALSE;
    g_sMID.sMech.ui16Active     = FALSE;

    g_sMID.bAbort        = FALSE;
    g_sMID.bRotBlocked   = FALSE;
    g_sMID.bMeasSuccDone = FALSE;
    g_sMID.eCalcElPar    = kMID_CalcWait;
    g_sMID.eCalcMechPar  = kMID_CalcWait;

    g_sMID.sCalcPwr.pfltIphA = &(g_sMID.sIO.psIABC->fltA);
    g_sMID.sCalcPwr.pfltIphB = &(g_sMID.sIO.psIABC->fltB);
    g_sMID.sCalcPwr.pfltIphC = &(g_sMID.sIO.psIABC->fltC);

    /* set START_DONE to switch to following transition state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_START_DONE;
}

/*!
 * @brief MID PWR_STG_CHARACT state
 *
 * @param None
 *
 * @return None
 */
static void MID_StatePwrStgCharact(void)
{
    /* type the code to do when in the PWR_STG_CHARACT state */
    MID_GetTrfChar();

    /* when charcterization ends, go to STOP state */
    if (g_sMID.sPwrStgCh.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_DONE;
    }
}

/*!
 * @brief MID RS state
 *
 * @param None
 *
 * @return None
 */
static void MID_StateRs(void)
{
    /* Type the code to do when in the RS state */
    MID_getRs();

    /* When Rs measurement done, go to NOLOAD state */
    if (g_sMID.sRs.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_DONE;
    }
}

/*!
 * @brief MID NOLOAD state
 *
 * @param None
 *
 * @return None
 */
static void MID_StateNoLoad(void)
{
    /* type the code to do when in the RS state */
    MID_testNoLoad();

    /* when No Load test done, go to NOLOAD state */
    if (g_sMID.sNoLoad.ui16Active == FALSE)
    {
        /* set _DONE to switch to following transition state */
        g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_NOLOAD_DONE;
    }
}

/*!
 * @brief MID BLOCKED state
 *
 * @param None
 *
 * @return None
 */
static void MID_StateBlocked(void)
{
    /* type the code to do when in the BLOCKED state */
    if (g_sMID.eCalcElPar == kMID_CalcWait)
        MID_testBlocked();

    /* when blocked-rotor test  and parameter calculation done,
       go to STOP state */
    if ((g_sMID.sBlocked.ui16Active == FALSE) && (g_sMID.eCalcElPar == kMID_CalcDone))
    {
        /* set _DONE to switch to following transition state */
        g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_BLOCKED_DONE;
    }
}

/*!
 * @brief MID mechanical parameters measurement state
 *
 * @details  In this state a moment of inertia is measured and viscous friction
            coefficient. More details about measurement are in mech_measure.c

            Preconditions:
                - properly set RFO and MRAS speed estimator
                - current control
                - DC-bus voltage measurement

            Escape the state:
            when the MECH measurement is done, which indicated by
            g_sMIDMech.ui16Active == FALSE
 *
 * @param None
 *
 * @return None
 */
static void MID_StateMech(void)
{
    /* type the code to do when in the mech. meas. state */

    /* call mech measurement routine */
    if (g_sMID.eCalcMechPar == kMID_CalcWait)
        MID_getMech();

    /* when the MECH measurement is finished, set the MECH_DONE flag */
    if ((g_sMID.sMech.ui16Active == FALSE) && (g_sMID.eCalcMechPar == kMID_CalcDone))
    {
        /* set MECH_DONE to switch to following transition state */
        g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_MECH_DONE;
    }
}

/*!
 * @brief MID STOP state
 *
 * @param None
 *
 * @return None
 */
static void MID_StateStop(void)
{
    /* type the code to do when in the STOP state */

    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_DONE;
}

/*!
 * @brief MID START to MID PWR_STG_CHARACT transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransStart2PwrStgCharact(void)
{
    /* type the code to do when going from the START to the
       PWR_STG_CHARACT state */
    /* enable start of characterization */

    /* acknowledge that the state machine can proceed into
       PWR_STG_CHARACT state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_PWR_STG_CHARACT_ACK;
}

/*!
 * @brief MID START to MID RS transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransStart2Rs(void)
{
    /* type the code to do when going from the START to the RS state */

    /* enable start of Rs measurement */
    g_sMID.sRs.ui32TimeMeas   = MID_RS_TIME_MEAS * M1_MID_TIME_ONESEC;
    g_sMID.sRs.ui32TimeSettle = (MID_RS_TIME_MEAS + MID_RS_TIME_SETTLE) * M1_MID_TIME_ONESEC;

    /* acknowledge that the state machine can proceed into RS state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_RS_ACK;
}

/*!
 * @brief MID PWR_STG_CHARACT to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransPwrStgCharact2Stop(void)
{
    /* type the code to do when going from the PWR_STG_CHARACT to the STOP state */

    /* acknowledge that the state machine can proceed into RS state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief MID RS to NOLOAD transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransRs2NoLoad(void)
{
    /* type the code to do when going from the RS_MEAS to the NOLOAD state */

    /* calculate voltage ramp */
    g_sMID.sNoLoad.sUMeasRmpPar.fltRampUp = M1_FAST_LOOP_TS * g_sMID.sNoLoad.fltUMeas;
    if (g_sMID.sPar.fltPwrN < 500.0F)
    {
        g_sMID.sNoLoad.sUMeasRmpPar.fltRampUp /= MID_NL_TIME_URAMP_0KW5;
        g_sMID.sNoLoad.ui32TimeMeas = MID_NL_TIME_URAMP_0KW5 * M1_MID_TIME_ONESEC;
    }
    else if (g_sMID.sPar.fltPwrN < 1500.0F)
    {
        g_sMID.sNoLoad.sUMeasRmpPar.fltRampUp /= MID_NL_TIME_URAMP_1KW5;
        g_sMID.sNoLoad.ui32TimeMeas = MID_NL_TIME_URAMP_1KW5 * M1_MID_TIME_ONESEC;
    }
    else /*if(g_sMID.sPar.fltPwrN < 3000.0F)*/
    {
        g_sMID.sNoLoad.sUMeasRmpPar.fltRampUp /= MID_NL_TIME_URAMP_3KW0;
        g_sMID.sNoLoad.ui32TimeMeas = MID_NL_TIME_URAMP_3KW0 * M1_MID_TIME_ONESEC;
    }
    g_sMID.sNoLoad.sUMeasRmpPar.fltRampDown = 10.0F * g_sMID.sNoLoad.sUMeasRmpPar.fltRampUp;

    g_sMID.sNoLoad.ui32TimeMeas += (MID_NL_TIME_MEAS * M1_MID_TIME_ONESEC);
    g_sMID.sNoLoad.ui32TimeSettle = g_sMID.sNoLoad.ui32TimeMeas + ((uint32_t)(MID_NL_TIME_SETTLE * M1_MID_TIME_ONESEC));

    /* acknowledge that the state machine can proceed into NOLOAD state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_NOLOAD_ACK;
}

/*!
 * @brief MID NOLOAD to MID BLOCKED transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransNoLoad2Blocked(void)
{
    /* type the code to do when going from the NOLOAD to the BLOCKED state */

    /* rms current PI controller gain */
    if (g_sMID.sPar.fltPwrN < 500.0F)
    {
        g_sMID.sBlocked.sIrmsPIPar.fltPGain = MID_BR_IRMS_KP_GAIN_0KW5;
        g_sMID.sBlocked.sIrmsPIPar.fltIGain = MID_BR_IRMS_KI_GAIN_0KW5;
    }
    else if (g_sMID.sPar.fltPwrN < 1500.0F)
    {
        g_sMID.sBlocked.sIrmsPIPar.fltPGain = MID_BR_IRMS_KP_GAIN_1KW5;
        g_sMID.sBlocked.sIrmsPIPar.fltIGain = MID_BR_IRMS_KI_GAIN_1KW5;
    }
    else /*if(g_sMID.sPar.fltPwrN < 3000.0F)*/
    {
        g_sMID.sBlocked.sIrmsPIPar.fltPGain = MID_BR_IRMS_KP_GAIN_3KW0;
        g_sMID.sBlocked.sIrmsPIPar.fltIGain = MID_BR_IRMS_KI_GAIN_3KW0;
    }

    g_sMID.sBlocked.ui32TimeMeas = MID_BR_TIME_MEAS * M1_MID_TIME_ONESEC;

    /* acknowledge that the state machine can proceed into BLOCKED state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_BLOCKED_ACK;
}

/*!
 * @brief MID BLOCKED to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransBlocked2Mech(void)
{
    /* type the code to do when going from the BLOCKED to the MECH state */

    g_sMID.sMech.fltSpdElStart     = (MID_MECH_SPDEL_MIN_PCT * 2.0F * FLOAT_PI) * g_sMID.sPar.fltFreqN;
    g_sMID.sMech.fltSpdElEnd       = (MID_MECH_SPDEL_MAX_PCT * 2.0F * FLOAT_PI) * g_sMID.sPar.fltFreqN;
    g_sMID.sMech.ui32TimeMeasMax   = MID_MECH_TIME_MEASMAX * M1_MID_TIME_ONESEC;
    g_sMID.sMech.ui32TimeSettleMax = MID_MECH_TIME_SETTLEMAX * M1_MID_TIME_ONESEC;

    /* acknowledge that the state machine can proceed into STOP state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_MECH_ACK;
}

/*!
 * @brief MID MECH to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransMech2Stop(void)
{
    /* type the code to do when going from the MECH to the STOP state */

    /* acknowledge that the state machine can proceed into STOP state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}

/*!
 * @brief (general) Any state to MID STOP transition
 *
 * @param None
 *
 * @return None
 */
static void MID_TransAll2Stop(void)
{
    /* type the code to do when going to the STOP state */

    /* acknowledge that the state machine can proceed into STOP state */
    g_g_sMIDCtrl.uiCtrl |= MID_SM_CTRL_STOP_ACK;
}
