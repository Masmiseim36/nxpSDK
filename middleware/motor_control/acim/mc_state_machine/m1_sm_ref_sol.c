/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_sm_ref_sol.h"
#include "mcdrv.h"
#include "mid_sm_states.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define M1_SVM_SECTOR_DEFAULT (2) /* default SVM sector */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

static void M1_StateFaultFast(void);
static void M1_StateInitFast(void);
static void M1_StateStopFast(void);
static void M1_StateRunFast(void);

static void M1_StateFaultSlow(void);
static void M1_StateInitSlow(void);
static void M1_StateStopSlow(void);
static void M1_StateRunSlow(void);

static void M1_TransFaultStop(void);
static void M1_TransInitFault(void);
static void M1_TransInitStop(void);
static void M1_TransStopFault(void);
static void M1_TransStopRun(void);
static void M1_TransRunFault(void);
static void M1_TransRunStop(void);

static void M1_StateRunCalibFast(void);
static void M1_StateRunMeasureFast(void);
static void M1_StateRunReadyFast(void);
static void M1_StateRunStartupFast(void);
static void M1_StateRunSpinFast(void);
static void M1_StateRunFreewheelFast(void);

static void M1_StateRunCalibSlow(void);
static void M1_StateRunMeasureSlow(void);
static void M1_StateRunReadySlow(void);
static void M1_StateRunStartupSlow(void);
static void M1_StateRunSpinSlow(void);
static void M1_StateRunFreewheelSlow(void);

static void M1_TransRunCalibReady(void);
static void M1_TransRunCalibMeasure(void);
static void M1_TransRunReadyStartup(void);
static void M1_TransRunStartupSpin(void);
static void M1_TransRunStartupFreewheel(void);
static void M1_TransRunSpinFreewheel(void);
static void M1_TransRunFreewheelReady(void);

static void M1_ClearFOCVariables(void);

static void M1_FaultDetection(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Main control structure */
mcdef_acim_t g_sM1Drive;

/*! @brief Main application switch */
bool_t g_bM1SwitchAppOnOff;

/*! @brief M1 structure */
m1_run_substate_t g_eM1StateRun;

/*! @brief FreeMASTER scales */
volatile float g_fltM1voltageScale;
volatile float g_fltM1DCBvoltageScale;
volatile float g_fltM1currentScale;
volatile float g_fltM1speedScale;

/*! @brief Application state machine table - fast */
const sm_app_state_fcn_t s_STATE_FAST = {M1_StateFaultFast, M1_StateInitFast, M1_StateStopFast, M1_StateRunFast};

/*! @brief Application state machine table - slow */
const sm_app_state_fcn_t s_STATE_SLOW = {M1_StateFaultSlow, M1_StateInitSlow, M1_StateStopSlow, M1_StateRunSlow};

/*! @brief Application sub-state function field - fast */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_FAST[6] = {M1_StateRunCalibFast,     M1_StateRunReadyFast,
                                                            M1_StateRunStartupFast,   M1_StateRunSpinFast,
                                                            M1_StateRunFreewheelFast, M1_StateRunMeasureFast};

/*! @brief Application sub-state function field - slow */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_SLOW[6] = {M1_StateRunCalibSlow,     M1_StateRunReadySlow,
                                                            M1_StateRunStartupSlow,   M1_StateRunSpinSlow,
                                                            M1_StateRunFreewheelSlow, M1_StateRunMeasureSlow};

/*! @brief Application state-transition functions field  */
static const sm_app_trans_fcn_t s_TRANS = {M1_TransFaultStop, M1_TransInitFault, M1_TransInitStop, M1_TransStopFault,
                                           M1_TransStopRun,   M1_TransRunFault,  M1_TransRunStop};

/*! @brief  State machine structure declaration and initialization */
sm_app_ctrl_t g_sM1Ctrl = {
    /* g_sM1Ctrl.psState, User state functions  */
    &s_STATE_FAST,

    /* g_sM1Ctrl.psState, User state functions  */
    &s_STATE_SLOW,

    /* g_sM1Ctrl..psTrans, User state-transition functions */
    &s_TRANS,

    /* g_sM1Ctrl.uiCtrl, Default no control command */
    SM_CTRL_NONE,

    /* g_sM1Ctrl.eState, Default state after reset */
    kSM_AppInit};

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Fault state called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateFaultFast(void)
{
    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, and raw IPM temperature */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* convert voltages from fractional measured values to float */
    g_sM1Drive.sFocACIM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocACIM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocACIM.fltUDcBus, &g_sM1Drive.sFocACIM.sFiltParUDcBus);

    /* Braking resistor control */
    if (g_sM1Drive.sFocACIM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip)
        M1_BRAKE_SET();
    else
        M1_BRAKE_CLEAR();

    /* Disable user application switch */
    g_bM1SwitchAppOnOff = FALSE;

    /* clear fault state manually from FreeMASTER */
    if (g_sM1Drive.bFaultClearMan)
    {
        /* Clear fault state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
        g_sM1Drive.bFaultClearMan = FALSE;
    }

    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);

    /* Detects faults */
    M1_FaultDetection();
}

/*!
 * @brief State initialization routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateInitFast(void)
{
    /* Type the code to do when in the INIT state */

    g_sM1Drive.fltTsFastLoop = M1_FAST_LOOP_TS;
    g_sM1Drive.fltTsSlowLoop = M1_SLOW_LOOP_TS;

    /* init scalar control */
    g_sM1Drive.sScalarCtrl.fltGainSpdMe2PosEl        = M1_SCALAR_INTEG_GAIN;
    g_sM1Drive.sScalarCtrl.fltGainRpm2Volt           = M1_SCALAR_VHZ_FACTOR_GAIN;
    g_sM1Drive.sScalarCtrl.fltUMin                   = M1_SCALAR_U_MIN;
    g_sM1Drive.sScalarCtrl.sRampParSpdMe.fltRampUp   = M1_SCALAR_RAMP_UP;
    g_sM1Drive.sScalarCtrl.sRampParSpdMe.fltRampDown = M1_SCALAR_RAMP_DOWN;
    g_sM1Drive.sScalarCtrl.fltGainHz2PosEl           = (2.0F * FLOAT_PI * M1_FAST_LOOP_TS);

    /* init RFO */
    g_sM1Drive.sFocACIM.sRFO.fltKrInvGain       = M1_RFO_KR_INV_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltKrLsTotLeakGain = M1_RFO_LS_TOT_LEAK_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltPsiRA1Gain      = M1_RFO_PSI_RA1_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltPsiRB1Gain      = M1_RFO_PSI_RB1_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltPsiSA1Gain      = M1_RFO_PSI_SA1_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltPsiSA2Gain      = M1_RFO_PSI_SA2_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltRsEst           = M1_RFO_RS_EST;
    g_sM1Drive.sFocACIM.sRFO.sCtrl.fltPGain     = M1_RFO_COMP_KP_GAIN;
    g_sM1Drive.sFocACIM.sRFO.sCtrl.fltIGain     = M1_RFO_COMP_KI_GAIN;
    g_sM1Drive.sFocACIM.sRFO.fltTorqueGain      = M1_RFO_TRQ_CNST;

    /* init MRAS */
    g_sM1Drive.sFocACIM.sSpdObs.fltPsiRA1Gain                       = M1_RFO_PSI_RA1_GAIN;
    g_sM1Drive.sFocACIM.sSpdObs.fltPsiRB1Gain                       = M1_RFO_PSI_RB1_GAIN;
    g_sM1Drive.sFocACIM.sSpdObs.fltTs                               = M1_FAST_LOOP_TS;
    g_sM1Drive.sFocACIM.sSpdObs.sCtrl.fltIGain                      = M1_MRAS_KI_GAIN;
    g_sM1Drive.sFocACIM.sSpdObs.sCtrl.fltPGain                      = M1_MRAS_KP_GAIN;
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltB0 = M1_SPEED_IIR_B0;
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltB1 = M1_SPEED_IIR_B1;
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltA1 = M1_SPEED_IIR_A1;
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedMeGain                      = M1_MRAS_SPDCNST;

    /* default MCAT control mode after reset */
    g_sM1Drive.eControl = kControlMode_SpeedFOC;

    /* Timing control and general variables */
    g_sM1Drive.ui32CounterState           = M1_FAULT_DURATION;
    g_sM1Drive.ui32TimeFullSpeedFreeWheel = M1_FREEWHEEL_DURATION;
    g_sM1Drive.ui32TimeCalibration        = M1_CALIB_DURATION;
    g_sM1Drive.ui32TimeFaultRelease       = M1_FAULT_DURATION;
    g_bM1SwitchAppOnOff                   = FALSE;

    /* fault set to init states */
    M1_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);
    M1_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* fault thresholds */
    g_sM1Drive.sFaultThresholds.fltUDcBusOver  = M1_U_DCB_OVERVOLTAGE;
    g_sM1Drive.sFaultThresholds.fltUDcBusUnder = M1_U_DCB_UNDERVOLTAGE;
    g_sM1Drive.sFaultThresholds.fltUDcBusTrip  = M1_U_DCB_TRIP;
    g_sM1Drive.sFaultThresholds.fltSpeedOver   = M1_N_OVERSPEED;

    /* defined scaling for FreeMASTER */
    g_fltM1voltageScale    = M1_U_MAX;
    g_fltM1currentScale    = M1_I_MAX;
    g_fltM1DCBvoltageScale = M1_U_DCB_MAX;
    g_fltM1speedScale      = M1_N_OVERSPEED;

    /* setup PWM control */
    g_sM1Drive.sFocACIM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    g_sM1Drive.sFocACIM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16C = FRAC16(0.5);
    g_sM1Drive.sFocACIM.fltDutyLim    = M1_CLOOP_LIMIT;

    /* enable dead-time compensation */
    g_sM1Drive.sFocACIM.bFlagDTComp = TRUE;

    /* DC-bus voltage measurement*/
    g_sM1Drive.sFocACIM.f16UDcBus                      = FRAC16(0.0);
    g_sM1Drive.sFocACIM.fltUDcBus                      = 0.0F;
    g_sM1Drive.sFocACIM.fltUDcBusFilt                  = 0.0F;
    g_sM1Drive.sFocACIM.sFiltParUDcBus.sFltCoeff.fltB0 = M1_UDCB_IIR_B0;
    g_sM1Drive.sFocACIM.sFiltParUDcBus.sFltCoeff.fltB1 = M1_UDCB_IIR_B1;
    g_sM1Drive.sFocACIM.sFiltParUDcBus.sFltCoeff.fltA1 = M1_UDCB_IIR_A1;
    g_sM1Drive.sFocACIM.sFiltParUDcBus.fltFltBfrX[0]   = M1_U_DCB_UNDERVOLTAGE / 2.0F + M1_U_DCB_OVERVOLTAGE / 2.0F;
    g_sM1Drive.sFocACIM.sFiltParUDcBus.fltFltBfrY[0]   = g_sM1Drive.sFocACIM.sFiltParUDcBus.fltFltBfrX[0];

    /* speed and rotor flux control */
    g_sM1Drive.sSpdFlux.sRampParSpdMe.fltTs      = M1_SLOW_LOOP_TS;
    g_sM1Drive.sSpdFlux.sRampParSpdMe.fltIncrMax = M1_SPEED_REQ_RMP_ACC;
    g_sM1Drive.sSpdFlux.sRampParSpdMe.fltDA      = M1_SPEED_REQ_RMP_JRK;
    g_sM1Drive.sSpdFlux.fltSRampDuration         = M1_SPEED_REQ_RMP_DUR;

    g_sM1Drive.sSpdFlux.fltSpdMeReqMin = M1_SPEED_REQ_MIN;
    g_sM1Drive.sSpdFlux.fltSpdMeReqMax = M1_SPEED_REQ_MAX;

    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltPGain    = M1_SPEED_PI_KP_GAIN;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltIGain    = M1_SPEED_PI_KI_GAIN;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltBetaGain = M1_SPEED_PI_BETA_GAIN;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltUpperLim = M1_SPEED_LIMIT_HIGH;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltLowerLim = M1_SPEED_LIMIT_LOW;

    g_sM1Drive.sSpdFlux.fltIdReqMin       = M1_FLUX_ID_MIN;
    g_sM1Drive.sSpdFlux.fltIdReqMax       = M1_FLUX_ID_MAX;
    g_sM1Drive.sSpdFlux.fltIdMTPA         = M1_FLUX_ID_START;
    g_sM1Drive.sSpdFlux.fltIdMTPAFiltCoef = M1_FLUX_MTPA_FILT_COEFF;
    g_sM1Drive.sSpdFlux.fltIdStart        = M1_FLUX_ID_START;
    g_sM1Drive.sSpdFlux.fltIdStartMinPct  = M1_FLUX_ID_START_MINPCT;

    g_sM1Drive.sSpdFlux.sFluxWkng.fltIGainUgain                   = (M1_I_MAX / M1_U_MAX);
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltPGain             = M1_FLUX_FW_KP_GAIN;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltIGain             = M1_FLUX_FW_KI_GAIN;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltUpperLim          = M1_FLUX_ID_MAX;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltLowerLim          = M1_FLUX_FW_ID_MIN;
    g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltB0 = M1_FLUX_IIR_B0;
    g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltB1 = M1_FLUX_IIR_B1;
    g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltA1 = M1_FLUX_IIR_A1;
    g_sM1Drive.sSpdFlux.sFluxWkng.pbStopIntegFlag                 = &g_sM1Drive.sSpdFlux.bFlagPIFWStopInt;

    /* current control */
    g_sM1Drive.sFocACIM.sPIParId.fltPGain = M1_D_KP_GAIN;
    g_sM1Drive.sFocACIM.sPIParId.fltIGain = M1_D_KI_GAIN;

    g_sM1Drive.sFocACIM.sPIParIq.fltPGain = M1_Q_KP_GAIN;
    g_sM1Drive.sFocACIM.sPIParIq.fltIGain = M1_Q_KI_GAIN;

    /* MID initialization: I/O variables */
    g_sMID.sIO.pfltId      = &(g_sM1Drive.sFocACIM.sIDQ.fltD);
    g_sMID.sIO.pfltIq      = &(g_sM1Drive.sFocACIM.sIDQ.fltQ);
    g_sMID.sIO.pfltIdReq   = &(g_sM1Drive.sFocACIM.sIDQReq.fltD);
    g_sMID.sIO.pfltIqReq   = &(g_sM1Drive.sFocACIM.sIDQReq.fltQ);
    g_sMID.sIO.pfltUdReq   = &(g_sM1Drive.sFocACIM.sUDQReq.fltD);
    g_sMID.sIO.pfltFreqReq = &(g_sM1Drive.sScalarCtrl.fltSpdMeReq);
    g_sMID.sIO.pfltTorque  = &(g_sM1Drive.sFocACIM.sRFO.fltTorque);
    g_sMID.sIO.pfltUDCbus  = &(g_sM1Drive.sFocACIM.fltUDcBusFilt);
    g_sMID.sIO.pfltSpeedEl = &(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1);
    g_sMID.sIO.psIABC      = &(g_sM1Drive.sFocACIM.sIABC);

    /* MID initialization: motor and measurement parameters */
    g_sMID.sPar.fltPwrN   = M1_P_NOM;
    g_sMID.sPar.fltIphN   = M1_I_NOM;
    g_sMID.sPar.fltUphN   = M1_U_NOM;
    g_sMID.sPar.fltFreqN  = M1_F_NOM;
    g_sMID.sPar.fltPp     = M1_MOTOR_PP;
    g_sMID.bAbort         = FALSE;
    g_sMID.bMeasSuccDone  = FALSE;
    g_sMID.bCalib         = FALSE;
    g_sMID.bRotBlocked    = FALSE;
    g_sMID.bParUpdateNew  = FALSE;
    g_sMID.bParRestoreOld = FALSE;
    g_sMID.fltFreqCLoop   = M1_MID_CALC_FREQ_CLOOP;
    g_sMID.fltFreqSLoop   = M1_MID_CALC_FREQ_SLOOP;

    g_sMID.sRs.fltIMeas =
        M1_I_NOM * (M1_SQRT2); /* by default the stator resistance is measured with nominal stator current */
    g_sMID.sNoLoad.fltUMeas     = (M1_U_NOM * M1_SQRT2); /* default stator voltage amplitude for no-load measurement */
    g_sMID.sMech.fltTrqAccl     = M1_MID_MECH_TRQ; /* default startup torque for mechanical parameter measurement */
    g_sMID.sBlocked.fltIrmsMeas = M1_I_NOM;        /* default rms stator current for blocked rotor measurement */
    g_sMID.sPwrStgCh.ui16NumOfChPnts = MID_PWRCHR_CURR_POINT_NUM; /* number of points in calibration characteristics */

    /* Initialize dead time LUT */
    GFLIB_Lut1DInit_FLT(-DTCOMP_I_RANGE, DTCOMP_I_RANGE, DTCOMP_TABLE_SIZE, &sLUTUDtComp);

    /* Clear rest of variables  */
    M1_ClearFOCVariables();

    /* Init sensors/actuators pointers */
    M1_SET_PTR_DUTY(g_sM1Drive.sFocACIM.sDutyABC);
    M1_SET_PTR_U_DC_BUS(g_sM1Drive.sFocACIM.f16UDcBus);
    M1_SET_PTR_I_ABC(g_sM1Drive.sFocACIM.sIABCFrac);
    M1_SET_PTR_SECTOR(g_sM1Drive.sFocACIM.ui16SectorSVM);
    M1_SET_PTR_AUX_CHAN(g_sM1Drive.f16AdcAuxSample);

    /* INIT_DONE command */
    g_sM1Ctrl.uiCtrl |= SM_CTRL_INIT_DONE;
}

/*!
 * @brief Stop state routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateStopFast(void)
{
    /* read 3-phase motor currents, DC-bus voltage and raw IPM temperature */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* convert voltages from fractional measured values to float */
    g_sM1Drive.sFocACIM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocACIM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocACIM.fltUDcBus, &g_sM1Drive.sFocACIM.sFiltParUDcBus);

    /* If the user switches on  or set non-zero speed*/
    if ((g_bM1SwitchAppOnOff != 0) || (g_sM1Drive.sSpdFlux.fltSpdMeReq != 0.0F))
    {
        /* Set the switch on */
        g_bM1SwitchAppOnOff = TRUE;

        /* Start command */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_START;
    }

    /* Braking resistor control */
    if (g_sM1Drive.sFocACIM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip)
        M1_BRAKE_SET();
    else
        M1_BRAKE_CLEAR();

    /* MID: check for motor parameter update request */
    if (g_sMID.bMeasSuccDone)
    {
        if (g_sMID.bParUpdateNew)
        {
            M1_MIDApplyPar(&g_sMID.sAlgNew);
            g_sMID.bParUpdateNew = FALSE;
        }
        else if (g_sMID.bParRestoreOld)
        {
            M1_MIDApplyPar(&g_sMID.sAlgBck);
            g_sMID.bParRestoreOld = FALSE;
        }
    }

    /* Detect fault */
    M1_FaultDetection();

    /* If a fault occurred */
    if (g_sM1Drive.sFaultIdPending)
    {
        /* Switches to the FAULT state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT;
    }

    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);
}

/*!
 * @brief Run state routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunFast(void)
{
    /* read 3-phase motor currents, DC-bus voltage and raw IPM temperature */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* If the user switches off */
    if (!g_bM1SwitchAppOnOff)
    {
        /* Stop command */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_STOP;
    }

    /* detect fault */
    M1_FaultDetection();

    /* If a fault occurred */
    if (g_sM1Drive.sFaultIdPending != 0)
    {
        /* Switches to the FAULT state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT;
    }

    /* convert phase currents from fractional measured values to float */
    g_sM1Drive.sFocACIM.sIABC.fltA = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.sIABCFrac.f16A, g_fltM1currentScale);
    g_sM1Drive.sFocACIM.sIABC.fltB = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.sIABCFrac.f16B, g_fltM1currentScale);
    g_sM1Drive.sFocACIM.sIABC.fltC = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.sIABCFrac.f16C, g_fltM1currentScale);

    /* convert voltages from fractional measured values to float */
    g_sM1Drive.sFocACIM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocACIM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* filter DC-bus voltage */
    g_sM1Drive.sFocACIM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocACIM.fltUDcBus, &g_sM1Drive.sFocACIM.sFiltParUDcBus);

    /* Braking resistor control */
    if (g_sM1Drive.sFocACIM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip)
        M1_BRAKE_SET();
    else
        M1_BRAKE_CLEAR();

    /* read speed */
    g_sM1Drive.sSpdFlux.fltSpdMeFilt = g_sM1Drive.sFocACIM.sSpdObs.fltSpeedMeIIR1;

    /* Run sub-state function */
    s_M1_STATE_RUN_TABLE_FAST[g_eM1StateRun]();

    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);

    /* set current sensor for  sampling */
    M1_MCDRV_CURR_3PH_CHAN_ASSIGN(&g_sM1AdcSensor);
}

/*!
 * @brief Fault state routine called in slow state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateFaultSlow(void)
{
    /* get IPM temperature in Celsius degrees */
    g_sM1Drive.fltIPMTemperature = M1_GetIPMTemperature(g_sM1Drive.f16AdcAuxSample);

    /* after fault condition ends wait defined time to clear fault state */
    if (!M1_FAULT_ANY(g_sM1Drive.sFaultIdPending))
    {
        if (--g_sM1Drive.ui32CounterState == 0)
        {
            /* Clear fault state */
            g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
        }
    }
    else
    {
        g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeFaultRelease;
    }
}

/*!
 * @brief Fault state routine called in slow state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateInitSlow(void)
{
}

/*!
 * @brief Stop state routine called in slow state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateStopSlow(void)
{
    /* get IPM temperature in Celsius degrees */
    g_sM1Drive.fltIPMTemperature = M1_GetIPMTemperature(g_sM1Drive.f16AdcAuxSample);
}

/*!
 * @brief Run state routine called in slow state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunSlow(void)
{
    /* Run sub-state function */
    s_M1_STATE_RUN_TABLE_SLOW[g_eM1StateRun]();

    /* get IPM temperature in Celsius degrees */
    g_sM1Drive.fltIPMTemperature = M1_GetIPMTemperature(g_sM1Drive.f16AdcAuxSample);
}

/*!
 * @brief Transition from Fault to Stop state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransFaultStop(void)
{
    /* Type the code to do when going from the FAULT to the INIT state */
    M1_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);

    /* Clear all FOC variables, init filters, etc. */
    M1_ClearFOCVariables();
}

/*!
 * @brief Transition from Init to Fault state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransInitFault(void)
{
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
    g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeFaultRelease;
}

/*!
 * @brief Transition from Init to Stop state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransInitStop(void)
{
    /* type the code to do when going from the INIT to the STOP state */
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
}

/*!
 * @brief Transition from Stop to Fault state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransStopFault(void)
{
    /* type the code to do when going from the STOP to the FAULT state */
    /* load the fault release time to counter */
    g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeFaultRelease;
}

/*!
 * @brief Transition from Stop to Run state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransStopRun(void)
{
    /* type the code to do when going from the STOP to the RUN state */
    /* 50% duty cycle */
    g_sM1Drive.sFocACIM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16C = FRAC16(0.5);

    /* PWM duty cycles calculation and update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);

    /* Clear offset filters */
    M1_MCDRV_CURR_3PH_CALIB_INIT(&g_sM1AdcSensor);

    /* Enable PWM output */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);

    /* pass calibration routine duration to state counter*/
    g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeCalibration;

    /* Calibration sub-state when transition to RUN */
    g_eM1StateRun = kRunState_Calib;

    /* Acknowledge that the system can proceed into the RUN state */
    g_sM1Ctrl.uiCtrl |= SM_CTRL_RUN_ACK;
}

/*!
 * @brief Transition from Run to Fault state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunFault(void)
{
    /* type the code to do when going from the RUN to the FAULT state */
    /* disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Clear FOC variables */
    M1_ClearFOCVariables();
}

/*!
 * @brief Transition from Run to Stop state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunStop(void)
{
    /* type the code to do when going from the RUN to the STOP state */
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* clear FOC variables */
    M1_ClearFOCVariables();

    /* Acknowledge that the system can proceed into the STOP state */
    g_sM1Ctrl.uiCtrl |= SM_CTRL_STOP_ACK;
}

/*!
 * @brief Calibration process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunCalibFast(void)
{
    /* Type the code to do when in the RUN CALIB sub-state
       performing ADC offset calibration */

    /* call offset measurement */
    M1_MCDRV_CURR_3PH_CALIB(&g_sM1AdcSensor);

    /* change SVM sector in range <1;6> to measure all AD channel mapping combinations */
    if (++g_sM1Drive.sFocACIM.ui16SectorSVM > 6)
        g_sM1Drive.sFocACIM.ui16SectorSVM = 1;
}

/*!
 * @brief Motor identification process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */

static void M1_StateRunMeasureFast(void)
{
    /* check whether abort flag was set */
    if (g_sMID.bAbort)
        g_sMID.ui16FaultMID |= MID_FAULT_ABORT;

    /* motor parameters measurement state machine */
    MID_SM_StateMachine(&g_g_sMIDCtrl);

    /* turn off dead-time compensation in case of power-stage
       characterization */
    g_sM1Drive.sFocACIM.bFlagDTComp = (g_g_sMIDCtrl.eState != kMID_PwrStgCharact);

    /* current control with position locked at zero angle is selected in
       case of power-stage characterization and stator resistance measurement */
    if ((g_g_sMIDCtrl.eState == kMID_Rs) || (g_g_sMIDCtrl.eState == kMID_PwrStgCharact))
    {
        /* keep position at zero */
        g_sM1Drive.sFocACIM.bFlagSpdStart      = TRUE;
        g_sM1Drive.sFocACIM.sRFO.a32RotFluxPos = ACC32(0.0);
        g_sM1Drive.sFocACIM.sSCFOC.fltSin      = 0.0F;
        g_sM1Drive.sFocACIM.sSCFOC.fltCos      = 1.0F;

        /* force sector to 4 to ensure that Ia current is measured */
        g_sM1Drive.sFocACIM.ui16SectorSVM = 4;

        /* current control routine */
        MCS_ACIMFocCtrlCurrentA1(&g_sM1Drive.sFocACIM);
    }

    /* current control is selected in case of mechanical parameters
       measurement */
    if (g_g_sMIDCtrl.eState == kMID_Mech)
    {
        /* check whether startup is done */
        if (g_sMID.sMech.eState == kMID_MechInit)
            g_sM1Drive.sFocACIM.bFlagSpdStart = TRUE;
        else
            g_sM1Drive.sFocACIM.bFlagSpdStart = FALSE;

        /* current control routine */
        MCS_ACIMFocCtrlCurrentA1(&g_sM1Drive.sFocACIM);
    }

    /* perform scalar control in case of no-load and blocked rotor tests */
    if ((g_g_sMIDCtrl.eState == kMID_NoLoad) || (g_g_sMIDCtrl.eState == kMID_Blocked))
    {
        /* open-loop scalar control */
        MCS_ACIMOpenLoopScalarCtrlA2(&g_sM1Drive.sFocACIM, &g_sM1Drive.sScalarCtrl);
    }

    /* when the measurement is done go to STOP state and reset enable
       measurement flag */
    if (g_g_sMIDCtrl.uiCtrl & MID_SM_CTRL_STOP_DONE)
    {
        /* if the measurement was unsuccessful set flag, otherwise revoke
           previous parameters */
        if (!g_sMID.ui16FaultMID)
            g_sMID.bMeasSuccDone = TRUE;
        else
            M1_MIDApplyPar(&g_sMID.sAlgBck);

        g_bM1SwitchAppOnOff   = FALSE;
        g_sMID.ui16EnableMeas = 0;
    }
}

/*!
 * @brief Ready state called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunReadyFast(void)
{
    /* Type the code to do when in the RUN READY sub-state */
    float_t fltSpdMeReqAbs;

    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
        case kControlMode_Scalar:
            if (g_sM1Drive.sScalarCtrl.fltSpdMeReq != 0.0F)
            {
                g_sM1Drive.sFocACIM.sUDQReq.fltD = 0.0F;
                g_sM1Drive.sFocACIM.sUDQReq.fltQ = 0.0F;

                /* Transition to the RUN STARTUP sub-state */
                M1_TransRunReadyStartup();
            }
            break;

        case kControlMode_VoltageFOC:
            if ((g_sM1Drive.sFocACIM.sUDQReq.fltD != 0) && (g_sM1Drive.sFocACIM.sUDQReq.fltQ != 0))
            {
                /* Transition to the RUN STARTUP sub-state */
                M1_TransRunReadyStartup();
            }
            break;

        case kControlMode_CurrentFOC:
            if ((g_sM1Drive.sFocACIM.sIDQReq.fltD != 0) && (g_sM1Drive.sFocACIM.sIDQReq.fltQ != 0))
            {
                /* Transition to the RUN STARTUP sub-state */
                M1_TransRunReadyStartup();
            }
            break;

        case kControlMode_SpeedFOC:
        default:
            /* speed FOC */
            fltSpdMeReqAbs = MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq);
            if (fltSpdMeReqAbs < g_sM1Drive.sSpdFlux.fltSpdMeReqMin)
            {
                g_sM1Drive.sSpdFlux.fltSpdMeReq = 0.0F;
            }
            else
            {
                /* limit to maximal speed */
                if (fltSpdMeReqAbs > g_sM1Drive.sSpdFlux.fltSpdMeReqMax)
                {
                    g_sM1Drive.sSpdFlux.fltSpdMeReq =
                        g_sM1Drive.sSpdFlux.fltSpdMeReqMax * MLIB_Sign_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq);
                }

                /* transition to the RUN STARTUP sub-state */
                M1_TransRunReadyStartup();
            }
    }
}

/*!
 * @brief Start-up process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunStartupFast(void)
{
    /* Type the code to do when in the RUN STARTUP sub-state */

    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
        case kControlMode_Scalar:
        case kControlMode_VoltageFOC:
        case kControlMode_CurrentFOC:
            /* switch directly to SPIN state */
            M1_TransRunStartupSpin();
            break;

        case kControlMode_SpeedFOC:
        default:

            /* if user changes speed out of limits */
            if ((MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq) < g_sM1Drive.sSpdFlux.fltSpdMeReqMin) ||
                (MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq) >= g_sM1Drive.sSpdFlux.fltSpdMeReqMax))
            {
                /* switch to FREEWHEEL state */
                M1_TransRunStartupFreewheel();
                return;
            }

            /* reset RFO and MRAS */
            AMCLIB_ACIMRotFluxObsrvInit_FLT_FCi(&g_sM1Drive.sFocACIM.sRFO);
            AMCLIB_ACIMSpeedMRASInit_FLT_FCi(&g_sM1Drive.sFocACIM.sSpdObs);

            /* generate no torque and magnetize rotor*/
            g_sM1Drive.sFocACIM.sIDQReq.fltQ = 0.0F;
            g_sM1Drive.sFocACIM.sIDQReq.fltD = g_sM1Drive.sSpdFlux.fltIdStart;

            /* current control */
            MCS_ACIMFocCtrlCurrentA1(&g_sM1Drive.sFocACIM);

            /* if sufficient time elapsed go to spin */
            if (g_sM1Drive.sFocACIM.sIDQ.fltD > (g_sM1Drive.sSpdFlux.fltIdStartMinPct * g_sM1Drive.sSpdFlux.fltIdStart))
            {
                M1_TransRunStartupSpin();
            }
            break;
    }
}

/*!
 * @brief Spin state called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunSpinFast(void)
{
    /* Type the code to do when in the RUN SPIN sub-state */
    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
        case kControlMode_Scalar:
            /* scalar control */
            MCS_ACIMOpenLoopScalarCtrlA1(&g_sM1Drive.sFocACIM, &g_sM1Drive.sScalarCtrl);

            /* check whether the speed is below minimum */
            if (g_sM1Drive.sScalarCtrl.fltSpdMeReq == 0.0F)
            {
                /* Sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_VoltageFOC:
            /* voltage FOC */
            MCS_ACIMFocCtrlVoltageA2(&g_sM1Drive.sFocACIM);

            /* check whether the required voltages are non-zero */
            if ((g_sM1Drive.sFocACIM.sUDQReq.fltD == 0.0F) || (g_sM1Drive.sFocACIM.sUDQReq.fltQ == 0.0F))
            {
                /* sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_CurrentFOC:
            /* current FOC */
            MCS_ACIMFocCtrlCurrentA1(&g_sM1Drive.sFocACIM);

            /* check whether the required currents are non-zero */
            if ((g_sM1Drive.sFocACIM.sIDQReq.fltD == 0.0F) || (g_sM1Drive.sFocACIM.sIDQReq.fltQ == 0.0F))
            {
                /* sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_SpeedFOC:
        default:
            /* full (speed) FOC */
            MCS_ACIMFocCtrlCurrentA1(&g_sM1Drive.sFocACIM);
            break;
    }
}

/*!
 * @brief Free-wheel process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunFreewheelFast(void)
{
    /* Type the code to do when in the RUN FREEWHEEL sub-state */

    /* clear actual speed values */
    g_sM1Drive.sSpdFlux.fltSpdMeRamp = 0.0F;

    /* control variables */
    g_sM1Drive.sFocACIM.sIDQReq.fltD = 0.0F;
    g_sM1Drive.sFocACIM.sIDQReq.fltQ = 0.0F;
    g_sM1Drive.sFocACIM.sUDQReq.fltD = 0.0F;
    g_sM1Drive.sFocACIM.sUDQReq.fltQ = 0.0F;
}

/*!
 * @brief Calibration process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunCalibSlow(void)
{
    if (--g_sM1Drive.ui32CounterState == 0)
    {
        /* write calibrated offset values */
        M1_MCDRV_CURR_3PH_CALIB_SET(&g_sM1AdcSensor);

        if (g_sMID.ui16EnableMeas != 0)
            /* To switch to the RUN MEASURE sub-state */
            M1_TransRunCalibMeasure();
        else
            /* To switch to the RUN READY sub-state */
            M1_TransRunCalibReady();
    }
}

/*!
 * @brief StartUp state called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunMeasureSlow(void)
{
    GMCLIB_3COOR_T_FLT sUABC;
    /* type the code to do when in the RUN MEASURE sub-state */

    /* calculate three-phase power if needed */
    if (g_sMID.sCalcPwr.bCalcPwr)
    {
        GMCLIB_ClarkInv_FLT_Ci(&g_sM1Drive.sFocACIM.sUAlBe, &sUABC);
        g_sMID.sCalcPwr.fltUphA = sUABC.fltA;
        g_sMID.sCalcPwr.fltUphB = sUABC.fltB;
        g_sMID.sCalcPwr.fltUphC = sUABC.fltC;
        MID_CalcPwr(&g_sMID.sCalcPwr);
    }

    /* calculate electrical parameters if needed */
    if (g_sMID.eCalcElPar == kMID_CalcWorking)
    {
        /* calculate */
        MID_CalcElPar();

        /* apply newly obtained electrical parameters */
        M1_MIDApplyPar(&g_sMID.sAlgNew);

        /* reset FOC */
        M1_ClearFOCVariables();
    }

    /* calculate mechanical parameters if needed */
    if (g_sMID.eCalcMechPar == kMID_CalcWorking)
    {
        /* calculate */
        MID_CalcMechPar();

        /* restore FOC parameters back to previous state*/
        M1_MIDApplyPar(&g_sMID.sAlgBck);
    }

    /* if in kMID_Blocked state, calculate power and control Irms */
    if ((g_g_sMIDCtrl.eState == kMID_Blocked) && g_sMID.bRotBlocked)
    {
        /* get motor input power */
        MID_ReadPwr(&g_sMID.sCalcPwr);

        /* calculate voltage limitation */
        g_sMID.sBlocked.sIrmsPIPar.fltUpperLim = M1_CLOOP_LIMIT * g_sM1Drive.sFocACIM.fltUDcBusFilt;
        g_sMID.sBlocked.sIrmsPIPar.fltLowerLim = -g_sMID.sBlocked.sIrmsPIPar.fltUpperLim;

        /* control rms current */
        g_sMID.sBlocked.fltUMeas = GFLIB_CtrlPIpAW_FLT(g_sMID.sPar.fltIphN - g_sMID.sCalcPwr.fltIrmsAvg,
                                                       &g_sMID.sBlocked.bIrmsPISatFlg, &g_sMID.sBlocked.sIrmsPIPar);
    }
}

/*!
 * @brief Ready state called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunReadySlow(void)
{
    /* type the code to do when in the RUN READY sub-state */
}

/*!
 * @brief Start-up process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunStartupSlow(void)
{
    /* type the code to do when in the RUN STARTUP sub-state */
}

/*!
 * @brief Spin state called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunSpinSlow(void)
{
    float_t fltSpdMeReqAbs;

    /* type the code to do when in the RUN SPIN sub-state */

    if (g_sM1Drive.eControl == kControlMode_SpeedFOC)
    {
        /* calculate absolute value of required speed */
        fltSpdMeReqAbs = MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq);

        /* speed reverse */
        if (fltSpdMeReqAbs > g_sM1Drive.sSpdFlux.fltSpdMeReqMin)
        {
            /* pass required speed values lower than nominal speed */
            if (fltSpdMeReqAbs > g_sM1Drive.sSpdFlux.fltSpdMeReqMax)
            {
                /* set required speed to nominal speed if over speed command > speed nominal */
                g_sM1Drive.sSpdFlux.fltSpdMeReq =
                    g_sM1Drive.sSpdFlux.fltSpdMeReqMax * MLIB_Sign_FLT(g_sM1Drive.sSpdFlux.fltSpdMeReq);
            }
        }
        else
        {
            g_sM1Drive.sSpdFlux.fltSpdMeRamp = 0.0F;
            if (MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.sRampParSpdMe.fltState) < g_sM1Drive.sSpdFlux.fltSpdMeReqMin)
            {
                /* sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }
        }

        /* call speed control */
        MCS_ACIMSpeedFluxCtrlA1(&g_sM1Drive.sFocACIM, &g_sM1Drive.sSpdFlux);
    }
}

/*!
 * @brief Free-wheel process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunFreewheelSlow(void)
{
    /* wait until free-wheel time passes */
    if (--g_sM1Drive.ui32CounterState == 0)
    {
        /* switch to sub state READY */
        M1_TransRunFreewheelReady();
    }
}

/*!
 * @brief Transition from Calib to Ready state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunCalibReady(void)
{
    /* Type the code to do when going from the RUN CALIB to the RUN READY sub-state */

    /* set 50% PWM duty cycle */
    g_sM1Drive.sFocACIM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16C = FRAC16(0.5);

    /* switch to sub state READY */
    g_eM1StateRun = kRunState_Ready;
}

/*!
 * @brief Transition from Calib to Measure state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunCalibMeasure(void)
{
    /* Type the code to do when going from the RUN CALIB to the RUN MEASURE sub-state */

    /* backup parameters before starting the measurement */
    /* current controllers */
    g_sMID.sAlgBck.fltIdPIKp = g_sM1Drive.sFocACIM.sPIParId.fltPGain;
    g_sMID.sAlgBck.fltIdPIKi = g_sM1Drive.sFocACIM.sPIParId.fltIGain;
    g_sMID.sAlgBck.fltIqPIKp = g_sM1Drive.sFocACIM.sPIParIq.fltPGain;
    g_sMID.sAlgBck.fltIqPIKi = g_sM1Drive.sFocACIM.sPIParIq.fltIGain;

    /* speed controller */
    g_sMID.sAlgBck.fltSpdPIKp     = g_sM1Drive.sSpdFlux.sPIParSpdMe.fltPGain;
    g_sMID.sAlgBck.fltSpdPIKi     = g_sM1Drive.sSpdFlux.sPIParSpdMe.fltIGain;
    g_sMID.sAlgBck.fltSpdMeReqMin = g_sM1Drive.sSpdFlux.fltSpdMeReqMin;
    g_sMID.sAlgBck.fltSpdMeReqMax = g_sM1Drive.sSpdFlux.fltSpdMeReqMax;
    g_sMID.sAlgBck.fltIMax        = g_sM1Drive.sSpdFlux.sPIParSpdMe.fltUpperLim;

    /* rotor flux control */
    g_sMID.sAlgBck.fltIdStart = g_sM1Drive.sSpdFlux.fltIdStart;
    g_sMID.sAlgBck.fltIdMin   = g_sM1Drive.sSpdFlux.fltIdReqMin;
    g_sMID.sAlgBck.fltIdMax   = g_sM1Drive.sSpdFlux.fltIdReqMax;
    g_sMID.sAlgBck.fltFWPIKp  = g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltPGain;
    g_sMID.sAlgBck.fltFWPIKi  = g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltIGain;

    /* speed and rotor-flux estimators */
    g_sMID.sAlgBck.fltMRASPsiRA1Gain  = g_sM1Drive.sFocACIM.sSpdObs.fltPsiRA1Gain;
    g_sMID.sAlgBck.fltMRASPsiRB1Gain  = g_sM1Drive.sFocACIM.sSpdObs.fltPsiRB1Gain;
    g_sMID.sAlgBck.fltGainSpd         = g_sM1Drive.sFocACIM.sSpdObs.fltSpeedMeGain;
    g_sMID.sAlgBck.fltPsiRA1Gain      = g_sM1Drive.sFocACIM.sRFO.fltPsiRA1Gain;
    g_sMID.sAlgBck.fltPsiRB1Gain      = g_sM1Drive.sFocACIM.sRFO.fltPsiRB1Gain;
    g_sMID.sAlgBck.fltPsiSA1Gain      = g_sM1Drive.sFocACIM.sRFO.fltPsiSA1Gain;
    g_sMID.sAlgBck.fltPsiSA2Gain      = g_sM1Drive.sFocACIM.sRFO.fltPsiSA2Gain;
    g_sMID.sAlgBck.fltKrInvGain       = g_sM1Drive.sFocACIM.sRFO.fltKrInvGain;
    g_sMID.sAlgBck.fltKrLsTotLeakGain = g_sM1Drive.sFocACIM.sRFO.fltKrLsTotLeakGain;
    g_sMID.sAlgBck.fltRsEst           = g_sM1Drive.sFocACIM.sRFO.fltRsEst;
    g_sMID.sAlgBck.fltGainTrq         = g_sM1Drive.sFocACIM.sRFO.fltTorqueGain;

    /* open-loop scalar */
    g_sMID.sAlgBck.fltGainSpdMe2PosEl = g_sM1Drive.sScalarCtrl.fltGainSpdMe2PosEl;
    g_sMID.sAlgBck.fltGainRpm2Volt    = g_sM1Drive.sScalarCtrl.fltGainRpm2Volt;

    /* backup dead-time settings */
    g_sMID.sAlgBck.bFlagDTComp = g_sM1Drive.sFocACIM.bFlagDTComp;

    /* prepare current controllers for stator resistance measurement */
    g_sM1Drive.sFocACIM.sPIParId.fltPGain = MID_RS_KP_GAIN;
    g_sM1Drive.sFocACIM.sPIParId.fltIGain = MID_RS_KI_GAIN;
    g_sM1Drive.sFocACIM.sPIParIq.fltPGain = MID_RS_KP_GAIN;
    g_sM1Drive.sFocACIM.sPIParIq.fltIGain = MID_RS_KI_GAIN;

    /* clear state registers */
    g_g_sMIDCtrl.uiCtrl   = 0;
    g_sMID.ui16FaultMID   = 0;
    g_sMID.ui16WarningMID = 0;

    /* First state in MID state machine will be kMID_Start */
    g_g_sMIDCtrl.eState = kMID_Start;

    /* Sub-state RUN MEASURE */
    g_eM1StateRun = kRunState_Measure;
}

/*!
 * @brief Transition from Ready to Startup state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunReadyStartup(void)
{
    /* Type the code to do when going from the RUN READY to the RUN STARTUP sub-state */

    /* set the startup flag */
    g_sM1Drive.sFocACIM.bFlagSpdStart = TRUE;

    /* Go to sub-state RUN STARTUP */
    g_eM1StateRun = kRunState_Startup;
}

/*!
 * @brief Transition from Startup to Spin state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunStartupSpin(void)
{
    /* Type the code to do when going from the RUN STARTUP to the RUN SPIN sub-state */

    /* clear the startup flag */
    g_sM1Drive.sFocACIM.bFlagSpdStart = FALSE;

    /* To switch to the RUN kRunState_Spin sub-state */
    g_eM1StateRun = kRunState_Spin;
}

/*!
 * @brief Transition from Startup to Free-wheel state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunStartupFreewheel(void)
{
    /* type the code to do when going from the RUN STARTUP to the RUN FREEWHEEL
       sub-state */

    /* turn off all transistors */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* clear application parameters */
    M1_ClearFOCVariables();

    /* Free-wheel duration set-up */
    g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeFullSpeedFreeWheel;

    /* enter FREEWHEEL sub-state */
    g_eM1StateRun = kRunState_Freewheel;
}

/*!
 * @brief Transition from Spin to Free-wheel state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunSpinFreewheel(void)
{
    /* Type the code to do when going from the RUN SPIN to the RUN FREEWHEEL sub-state */
    /* set 50% PWM duty cycle */

    /* turn off all transistors */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* clear application parameters */
    M1_ClearFOCVariables();

    /* Generates a time gap to assure the rotor is not rotating */
    g_sM1Drive.ui32CounterState = g_sM1Drive.ui32TimeFullSpeedFreeWheel;

    /* enter FREEWHEEL sub-state */
    g_eM1StateRun = kRunState_Freewheel;
}

/*!
 * @brief Transition from Free-wheel to Ready state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunFreewheelReady(void)
{
    /* Type the code to do when going from the RUN kRunState_FreeWheel to the RUN kRunState_Ready sub-state */

    /* enable PWM output */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);

    /* Sub-state RUN READY */
    g_eM1StateRun = kRunState_Ready;
}

/*!
 * @brief Clear FOc variables in global variable
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_ClearFOCVariables(void)
{
    /* Reset ACIM FOC algorithm state variables */

    /* Clear voltages */
    g_sM1Drive.sFocACIM.sUDQReq.fltD          = 0.0F;
    g_sM1Drive.sFocACIM.sUDQReq.fltQ          = 0.0F;
    g_sM1Drive.sFocACIM.sUAlBe.fltAlpha       = 0.0F;
    g_sM1Drive.sFocACIM.sUAlBe.fltBeta        = 0.0F;
    g_sM1Drive.sFocACIM.sDutyABC.f16A         = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16B         = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sDutyABC.f16C         = FRAC16(0.5);
    g_sM1Drive.sFocACIM.sUAlBeDTComp.fltAlpha = 0.0F;
    g_sM1Drive.sFocACIM.sUAlBeDTComp.fltBeta  = 0.0F;

    /* Clear scalar speed ramp */
    g_sM1Drive.sScalarCtrl.fltSpdMeRamp           = 0.0F;
    g_sM1Drive.sScalarCtrl.sRampParSpdMe.fltState = 0.0F;

    /* Clear currents */
    g_sM1Drive.sFocACIM.sIAlBe.fltAlpha = 0.0F;
    g_sM1Drive.sFocACIM.sIAlBe.fltBeta  = 0.0F;
    g_sM1Drive.sFocACIM.sIDQ.fltD       = 0.0F;
    g_sM1Drive.sFocACIM.sIDQ.fltQ       = 0.0F;
    g_sM1Drive.sFocACIM.sIDQReq.fltD    = 0.0F;
    g_sM1Drive.sFocACIM.sIDQReq.fltQ    = 0.0F;
    g_sM1Drive.sFocACIM.sIABC.fltA      = 0.0F;
    g_sM1Drive.sFocACIM.sIABC.fltB      = 0.0F;
    g_sM1Drive.sFocACIM.sIABC.fltC      = 0.0F;

    /* Clear d-axis current controller */
    g_sM1Drive.sFocACIM.sPIParId.fltIAccK_1  = 0.0F;
    g_sM1Drive.sFocACIM.sPIParId.fltInErrK_1 = 0.0F;
    g_sM1Drive.sFocACIM.sPIParId.bLimFlag    = FALSE;
    g_sM1Drive.sFocACIM.sIDQErr.fltD         = 0.0F;
    g_sM1Drive.sFocACIM.bFlagPIIdStopInt     = FALSE;
    GFLIB_CtrlPIpAWInit_FLT(0.0F, &g_sM1Drive.sFocACIM.sPIParId);

    /* Clear q-axis current controller */
    g_sM1Drive.sFocACIM.sPIParIq.fltIAccK_1  = 0.0F;
    g_sM1Drive.sFocACIM.sPIParIq.fltInErrK_1 = 0.0F;
    g_sM1Drive.sFocACIM.sPIParIq.bLimFlag    = FALSE;
    g_sM1Drive.sFocACIM.sIDQErr.fltQ         = 0.0F;
    g_sM1Drive.sFocACIM.bFlagPIIqStopInt     = FALSE;
    GFLIB_CtrlPIpAWInit_FLT(0.0F, &g_sM1Drive.sFocACIM.sPIParIq);

    /* Clear rotor flux position and rotor speed estimation algorithms*/
    g_sM1Drive.sFocACIM.sSCFOC.fltSin          = 0.0F;
    g_sM1Drive.sFocACIM.sSCFOC.fltCos          = 1.0F;
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1 = 0.0F;
    AMCLIB_ACIMRotFluxObsrvInit_FLT_FCi(&g_sM1Drive.sFocACIM.sRFO);
    AMCLIB_ACIMSpeedMRASInit_FLT_FCi(&g_sM1Drive.sFocACIM.sSpdObs);
    GDFLIB_FilterIIR1Init_FLT(&g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param);

    /* Speed and flux control reset */
    g_sM1Drive.sSpdFlux.fltSpdMeReq             = 0.0F;
    g_sM1Drive.sSpdFlux.fltSpdMeRamp            = 0.0F;
    g_sM1Drive.sSpdFlux.fltSpdMeFilt            = 0.0F;
    g_sM1Drive.sSpdFlux.fltIdMTPA               = 0.0F;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltIAccK_1  = 0.0F;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltInErrK_1 = 0.0F;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.bLimFlag    = FALSE;
    g_sM1Drive.sSpdFlux.fltSpdMeErr             = 0.0F;
    g_sM1Drive.sSpdFlux.bFlagPISpdMeStopInt     = FALSE;
    g_sM1Drive.sSpdFlux.bFlagPIFWStopInt        = FALSE;
    g_sM1Drive.sSpdFlux.bStartupDone            = FALSE;
    g_sM1Drive.sSpdFlux.sRampParSpdMe.fltTarget = 0.0F;
    AMCLIB_CtrlFluxWkngInit_FLT(g_sM1Drive.sSpdFlux.fltIdReqMax, &g_sM1Drive.sSpdFlux.sFluxWkng);
    GFLIB_FlexSRampInit_FLT(0.0F, &g_sM1Drive.sSpdFlux.sRampParSpdMe);
}

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
 * @brief Apply MID measured parameters
 *
 * @param mid_acim_alg_prms_a1_t sAlgPar parameters
 *
 * @return None
 */
void M1_MIDApplyPar(mid_acim_alg_prms_a1_t *sAlgPar)
{
    /* apply MID parameters to FOC */

    /* current PI controllers gain */
    g_sM1Drive.sFocACIM.sPIParId.fltPGain = sAlgPar->fltIdPIKp;
    g_sM1Drive.sFocACIM.sPIParId.fltIGain = sAlgPar->fltIdPIKi;
    g_sM1Drive.sFocACIM.sPIParIq.fltPGain = sAlgPar->fltIqPIKp;
    g_sM1Drive.sFocACIM.sPIParIq.fltIGain = sAlgPar->fltIqPIKi;

    /* speed control setup */
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltPGain    = sAlgPar->fltSpdPIKp;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltIGain    = sAlgPar->fltSpdPIKi;
    g_sM1Drive.sSpdFlux.fltSpdMeReqMin          = sAlgPar->fltSpdMeReqMin;
    g_sM1Drive.sSpdFlux.fltSpdMeReqMax          = sAlgPar->fltSpdMeReqMax;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltUpperLim = g_sMID.sAlgBck.fltIMax;
    g_sM1Drive.sSpdFlux.sPIParSpdMe.fltLowerLim = -g_sMID.sAlgBck.fltIMax;

    /* flux control setup */
    g_sM1Drive.sSpdFlux.fltIdStart                       = sAlgPar->fltIdStart;
    g_sM1Drive.sSpdFlux.fltIdReqMin                      = sAlgPar->fltIdMin;
    g_sM1Drive.sSpdFlux.fltIdReqMax                      = sAlgPar->fltIdMax;
    g_sM1Drive.sSpdFlux.fltIdMTPA                        = sAlgPar->fltIdStart;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltPGain    = sAlgPar->fltFWPIKp;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltIGain    = sAlgPar->fltFWPIKi;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltUpperLim = sAlgPar->fltIdMax;
    g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltLowerLim = sAlgPar->fltIdMin;

    /* speed and rotor-flux estimation algorithms */
    g_sM1Drive.sFocACIM.sSpdObs.fltSpeedMeGain  = sAlgPar->fltGainSpd;
    g_sM1Drive.sFocACIM.sSpdObs.fltPsiRA1Gain   = sAlgPar->fltMRASPsiRA1Gain;
    g_sM1Drive.sFocACIM.sSpdObs.fltPsiRB1Gain   = sAlgPar->fltMRASPsiRB1Gain;
    g_sM1Drive.sFocACIM.sRFO.fltPsiRA1Gain      = sAlgPar->fltPsiRA1Gain;
    g_sM1Drive.sFocACIM.sRFO.fltPsiRB1Gain      = sAlgPar->fltPsiRB1Gain;
    g_sM1Drive.sFocACIM.sRFO.fltPsiSA1Gain      = sAlgPar->fltPsiSA1Gain;
    g_sM1Drive.sFocACIM.sRFO.fltPsiSA2Gain      = sAlgPar->fltPsiSA2Gain;
    g_sM1Drive.sFocACIM.sRFO.fltKrInvGain       = sAlgPar->fltKrInvGain;
    g_sM1Drive.sFocACIM.sRFO.fltKrLsTotLeakGain = sAlgPar->fltKrLsTotLeakGain;
    g_sM1Drive.sFocACIM.sRFO.fltRsEst           = sAlgPar->fltRsEst;
    g_sM1Drive.sFocACIM.sRFO.fltTorqueGain      = sAlgPar->fltGainTrq;

    /* open-loop control */
    g_sM1Drive.sScalarCtrl.fltGainSpdMe2PosEl = sAlgPar->fltGainSpdMe2PosEl;
    g_sM1Drive.sScalarCtrl.fltGainRpm2Volt    = sAlgPar->fltGainRpm2Volt;

    /* enable/disable dead-time compensation */
    g_sM1Drive.sFocACIM.bFlagDTComp = sAlgPar->bFlagDTComp;
}

/*!
 * @brief Set application switch value to On or Off mode
 *
 * @param bValue  bool value, true - On of false - Off
 *
 * @return None
 */
void M1_SetAppSwitch(bool_t bValue)
{
    g_bM1SwitchAppOnOff = bValue;
}

/*!
 * @brief Get application switch value
 *
 * @param void  No input parameter
 *
 * @return bool_t Return bool value, true or false
 */
bool_t M1_GetAppSwitch(void)
{
    return (g_bM1SwitchAppOnOff);
}

/*!
 * @brief Get application state
 *
 * @param void  No input parameter
 *
 * @return uint16_t Return current application state
 */
uint16_t M1_GetAppState()
{
    return ((uint16_t)g_sM1Ctrl.eState);
}

/*!
 * @brief Set spin speed of the motor in fractional value
 *
 * @param f16SpeedCmd  Speed command - set speed
 *
 * @return None
 */
void M1_SetSpeed(float fltSpdMeReq)
{
    if (g_bM1SwitchAppOnOff)
    {
        /* Set speed */
        if (MLIB_Abs_FLT(fltSpdMeReq) < g_sM1Drive.sSpdFlux.fltSpdMeReqMin)
        {
            g_sM1Drive.sSpdFlux.fltSpdMeReq = 0.0F;
        }
        else if (MLIB_Abs_FLT(fltSpdMeReq) > g_sM1Drive.sSpdFlux.fltSpdMeReqMax)
        {
            g_sM1Drive.sSpdFlux.fltSpdMeReq = g_sM1Drive.sSpdFlux.fltSpdMeReqMax;
        }
        else
        {
            g_sM1Drive.sSpdFlux.fltSpdMeReq = fltSpdMeReq;
        }
    }
    else
    {
        /* Set zero speed */
        g_sM1Drive.sSpdFlux.fltSpdMeRamp = 0;
    }
}

/*!
 * @brief Get spin speed of the motor in fractional value
 *
 * @param void  No input parameter
 *
 * @return frac16_t Fractional value of the current speed
 */
float M1_GetSpeed(void)
{
    /* return speed */
    return g_sM1Drive.sSpdFlux.fltSpdMeReq;
}

/*!
 * @brief Fault detention routine - check various faults
 *
 * @param void  No input parameter
 *
 * @return None
 */
void M1_FaultDetection(void)
{
    /* clearing actual faults before detecting them again */

    /* clear all faults */
    M1_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* fault: DC-bus over-current */
    if (M1_MCDRV_PWM3PH_FLT_GET(&g_sM1Pwm3ph))
        M1_FAULT_SET(g_sM1Drive.sFaultIdPending, M1_FAULT_I_DCBUS_OVER);

    /* fault: DC-bus over-voltage */
    if (g_sM1Drive.sFocACIM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusOver)
        M1_FAULT_SET(g_sM1Drive.sFaultIdPending, M1_FAULT_U_DCBUS_OVER);

    /* fault: DC-bus under-voltage */
    if (g_sM1Drive.sFocACIM.fltUDcBusFilt < g_sM1Drive.sFaultThresholds.fltUDcBusUnder)
        M1_FAULT_SET(g_sM1Drive.sFaultIdPending, M1_FAULT_U_DCBUS_UNDER);

    /* fault: over-speed  */
    if ((MLIB_Abs_FLT(g_sM1Drive.sSpdFlux.fltSpdMeFilt) > g_sM1Drive.sFaultThresholds.fltSpeedOver) &&
        g_sM1Drive.sSpdFlux.bStartupDone && (g_eM1StateRun == kRunState_Spin))
        M1_FAULT_SET(g_sM1Drive.sFaultIdPending, M1_FAULT_SPEED_OVER);

    /* pass fault to fault ID */
    g_sM1Drive.sFaultIdCaptured |= g_sM1Drive.sFaultIdPending;
}
