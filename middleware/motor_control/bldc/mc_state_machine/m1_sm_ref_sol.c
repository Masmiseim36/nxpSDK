/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_sm_ref_sol.h"
#include "mcdrv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define M1_MAX_CMT_ERRORS (3) /* maximum number of commutation errors */
#define M1_CURRENT_CONTROLLER_ALIGN_LIM_LOW \
    FRAC16(2.0 / 100.0) /* align current PI controller's low output limit - in percentage / 100 */
#define M1_CURRENT_CONTROLLER_ALIGN_LIM_HIGH \
    FRAC16(90.0 / 100.0) /* align current PI controller's high output limit - in percentage / 100 */

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
static void M1_StateRunReadyFast(void);
static void M1_StateRunAlignFast(void);
static void M1_StateRunStartupFast(void);
static void M1_StateRunSpinFast(void);
static void M1_StateRunFreewheelFast(void);

static void M1_StateRunCalibSlow(void);
static void M1_StateRunReadySlow(void);
static void M1_StateRunAlignSlow(void);
static void M1_StateRunStartupSlow(void);
static void M1_StateRunSpinSlow(void);
static void M1_StateRunFreewheelSlow(void);

static void M1_TransRunCalibReady(void);
static void M1_TransRunReadyAlign(void);
static void M1_TransRunAlignStartup(void);
static void M1_TransRunAlignReady(void);
static void M1_TransRunStartupSpin(void);
static void M1_TransRunStartupFreewheel(void);
static void M1_TransRunSpinFreewheel(void);
static void M1_TransRunFreewheelReady(void);

static void M1_FaultDetection(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Main control structure */
mcdef_bldc_t g_sM1Drive;

/*! @brief Main application switch */
bool_t g_bM1SwitchAppOnOff;

/*! @brief M1 structure */
static m1_run_substate_t s_eM1StateRun;

/*! @brief FreeMASTER scales */
/*! DO NOT USE THEM in the code to avoid float library include */
static volatile float s_fltM1DCBvoltageScale;
static volatile float s_fltM1currentScale;
static volatile float s_fltM1speedScale;

/*! @brief Application state machine table - fast */
const sm_app_state_fcn_t s_STATE_FAST = {M1_StateFaultFast, M1_StateInitFast, M1_StateStopFast, M1_StateRunFast};

/*! @brief Application state machine table - slow */
const sm_app_state_fcn_t s_STATE_SLOW = {M1_StateFaultSlow, M1_StateInitSlow, M1_StateStopSlow, M1_StateRunSlow};

/*! @brief Application sub-state function field - fast */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_FAST[6] = {M1_StateRunCalibFast, M1_StateRunReadyFast,
                                                            M1_StateRunAlignFast, M1_StateRunStartupFast,
                                                            M1_StateRunSpinFast,  M1_StateRunFreewheelFast};

/*! @brief Application sub-state function field - slow */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_SLOW[6] = {M1_StateRunCalibSlow, M1_StateRunReadySlow,
                                                            M1_StateRunAlignSlow, M1_StateRunStartupSlow,
                                                            M1_StateRunSpinSlow,  M1_StateRunFreewheelSlow};

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
    /* type the code to do when in the FAULT state */

    /* determine which ADC has desired phase voltage value */
    g_sM1AdcSensor.ui16Sector = g_sM1Drive.sCtrlBLDC.i16SectorCmt;

    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* DC-Bus current filter */
    g_sM1Drive.sCtrlBLDC.f16IDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16IDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sIDcBusFilter);

    /* DC-Bus voltage filter */
    g_sM1Drive.sCtrlBLDC.f16UDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16UDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sUDcBusFilter);

    /* Disable user application switch */
    g_bM1SwitchAppOnOff = FALSE;

    /* clear fault state manually from FreeMASTER */
    if (g_sM1Drive.bFaultClearMan)
    {
        /* Clear fault state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
        g_sM1Drive.bFaultClearMan = FALSE;
    }

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
    /* type the code to do when in the INIT state */

    /* initialize free wheel period */
    g_sM1Drive.ui16PeriodFreewheelLong = M1_FREEWHEEL_T_LONG;
    g_sM1Drive.ui16PeriodFreewheelShort = M1_FREEWHEEL_T_SHORT;

    /* initialize alignment period */
    g_sM1Drive.ui16TimeAlignment = M1_ALIGN_DURATION;
    g_sM1Drive.sCtrlBLDC.f16IDcBusAlign = M1_ALIGN_CURRENT;

    /* DC-bus current measurement */
    g_sM1Drive.sCtrlBLDC.f16IDcBusLim = M1_I_DCB_LIMIT;
    g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32B0 = M1_IDCB_IIR_B0;
    g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32B1 = M1_IDCB_IIR_B1;
    g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32A1 = M1_IDCB_IIR_A1;
    g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32B0 = M1_UDCB_IIR_B0;
    g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32B1 = M1_UDCB_IIR_B1;
    g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32A1 = M1_UDCB_IIR_A1;

    /* current controller parameters */
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.a32PGain = M1_TORQUE_LOOP_KP_GAIN;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.a32IGain = M1_TORQUE_LOOP_KI_GAIN;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16UpperLim = M1_CTRL_LOOP_LIM_HIGH;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16LowerLim = M1_CTRL_LOOP_LIM_LOW;
    g_sM1Drive.sCtrlBLDC.bIDcBusPiStopIntFlag = FALSE;

    /* reset required speed */
    g_sM1Drive.sCtrlBLDC.f16SpeedCmd = FRAC16(0.0);
    g_sM1Drive.sCtrlBLDC.f16SpeedMinimal = M1_N_MIN;

    /* initialize speed ramp */
    g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32RampDown = M1_SPEED_LOOP_RAMP_DOWN;
    g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32RampUp = M1_SPEED_LOOP_RAMP_UP;

    /* speed controller parameters */
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.a32PGain = M1_SPEED_LOOP_KP_GAIN;
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.a32IGain = M1_SPEED_LOOP_KI_GAIN;
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16UpperLim = M1_CTRL_LOOP_LIM_HIGH;
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16LowerLim = M1_CTRL_LOOP_LIM_LOW;
    g_sM1Drive.sCtrlBLDC.bSpeedPiStopIntFlag = FALSE;

    g_sM1Drive.sCtrlBLDC.i16SpeedScaleConst = M1_SPEED_SCALE_CONST;
    g_sM1Drive.sCtrlBLDC.f16SpeedNominal = M1_N_NOM;

    /* initialize sensorless algorithm */
    g_sM1Drive.sCtrlBLDC.i16SectorCmt = 0;
    g_sM1Drive.f16StartCmtAcceleration = M1_START_CMT_ACCELER;
    g_sM1Drive.ui16StartCmtNumber = M1_STARTUP_CMT_CNT;
    g_sM1Drive.ui16PeriodCmtNextInit = M1_STARTUP_CMT_PER;
    g_sM1Drive.ui16PeriodToffInit = M1_CMT_T_OFF;
    g_sM1Drive.f32UBemfIntegThreshold = M1_INTEG_TRH;

    /* fault thresholds */
    g_sM1Drive.sFaultThresholds.f16IDcBusOver = M1_I_DCB_OVERCURRENT;
    g_sM1Drive.sFaultThresholds.f16UDcBusOver = M1_U_DCB_OVERVOLTAGE;
    g_sM1Drive.sFaultThresholds.f16UDcBusUnder = M1_U_DCB_UNDERVOLTAGE;

    g_sM1Drive.ui16TimeFaultRelease = M1_FAULT_DURATION;
    g_sM1Drive.ui16TimeCalibration = M1_CALIB_DURATION;

    /* frequencies of control loop and commutation timer for MCAT constant calculation */
    g_sM1Drive.ui32FreqCmtTimer = (uint32_t)g_sClockSetup.ui32CmtTimerFreq;
    g_sM1Drive.ui16FreqCtrlLoop = CTRL_LOOP_FREQ;
    g_sM1Drive.ui16FreqPwm = PWM_FREQ;

    /* Defined scaling for FreeMASTER */
    s_fltM1currentScale = M1_I_MAX;
    s_fltM1DCBvoltageScale = M1_U_DCB_MAX;
    s_fltM1speedScale = M1_N_MAX;

    /* Filter init not to enter to fault */
    g_sM1Drive.sCtrlBLDC.sUDcBusFilter.f16FltBfrX[0] =
        (frac16_t)((M1_U_DCB_UNDERVOLTAGE / 2.0) + (M1_U_DCB_OVERVOLTAGE / 2.0));
    g_sM1Drive.sCtrlBLDC.sUDcBusFilter.f32FltBfrY[0] =
        (frac32_t)((M1_U_DCB_UNDERVOLTAGE / 2.0) + (M1_U_DCB_OVERVOLTAGE / 2.0)) << 16;

    /* Init sensors/actuators pointers */
    M1_SET_PTR_U_DC_BUS(g_sM1Drive.sCtrlBLDC.f16UDcBusNoFilt);
    M1_SET_PTR_I_DC_BUS(g_sM1Drive.sCtrlBLDC.f16IDcBusNoFilt);
    M1_SET_PTR_BEMF_VOLT(g_sM1Drive.sCtrlBLDC.f16UPhase);
    M1_SET_PTR_AUX_CHAN(g_sM1Drive.ui16Aux);
    M1_SET_PTR_CNT_ACT(g_sM1Drive.ui16TimeCurrent);
    M1_SET_PTR_VALUE_ACT(g_sM1Drive.ui16TimeCurrentEvent);

    /* INIT_DONE command */
    g_sM1Ctrl.uiCtrl |= SM_CTRL_INIT_DONE;
}

/*!
 * @brief Stop state routine routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateStopFast(void)
{
    /* type the code to do when in the STOP state */

    /* determine which ADC has desired phase voltage value */
    g_sM1AdcSensor.ui16Sector = g_sM1Drive.sCtrlBLDC.i16SectorCmt;

    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* DC-Bus current filter */
    g_sM1Drive.sCtrlBLDC.f16IDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16IDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sIDcBusFilter);

    /* DC-Bus voltage filter */
    g_sM1Drive.sCtrlBLDC.f16UDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16UDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sUDcBusFilter);

    /* calculate BEMF voltage from phase voltage */
    g_sM1Drive.sCtrlBLDC.f16UPhaseBemf =
        MLIB_Sub_F16(g_sM1Drive.sCtrlBLDC.f16UPhase, (g_sM1Drive.sCtrlBLDC.f16UDcBus >> 1));

    /* if the user switches on or set non-zero speed */
    if ((g_bM1SwitchAppOnOff != 0) || (g_sM1Drive.sCtrlBLDC.f16SpeedCmd != 0))
    {
        /* Set the switch on */
        g_bM1SwitchAppOnOff = 1;

        /* Start command */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_START;
    }

    M1_FaultDetection();

    /* If a fault occurred */
    if (g_sM1Drive.sFaultIdPending)
    {
        /* Switches to the FAULT state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT;
    }
}

/*!
 * @brief Run state routine routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunFast(void)
{
    /* read timer counter and value registers */
    M1_MCDRV_TMR_CMT_GET(&g_sM1CmtTmr);

    /* type the code to do when in the RUN state */
    /* check application main switch */
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

    /* all ADC result registers must be read before next measurement */

    /* determine which ADC has desired phase voltage value */
    g_sM1AdcSensor.ui16Sector = g_sM1Drive.sCtrlBLDC.i16SectorCmt;

    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* DC-Bus current filter */
    g_sM1Drive.sCtrlBLDC.f16IDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16IDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sIDcBusFilter);

    /* DC-Bus voltage filter */
    g_sM1Drive.sCtrlBLDC.f16UDcBus =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sCtrlBLDC.f16UDcBusNoFilt, &g_sM1Drive.sCtrlBLDC.sUDcBusFilter);

    /* calculate BEMF voltage from phase voltage */
    g_sM1Drive.sCtrlBLDC.f16UPhaseBemf =
        MLIB_Sub_F16(g_sM1Drive.sCtrlBLDC.f16UPhase, (g_sM1Drive.sCtrlBLDC.f16UDcBus >> 1));

    /* Run sub-state function */
    s_M1_STATE_RUN_TABLE_FAST[s_eM1StateRun]();

    /* configure ADC for next measurement */
    M1_MCDRV_ADC_ASSIGN_BEMF(&g_sM1AdcSensor);
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
    /* after fault condition ends wait defined time to clear fault state */
    if (!MC_FAULT_ANY(g_sM1Drive.sFaultIdPending))
    {
        if (--g_sM1Drive.ui16CounterState == 0)
        {
            /* Clear fault state */
            g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT_CLEAR;
        }
    }
    else
    {
        g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;
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
    s_M1_STATE_RUN_TABLE_SLOW[s_eM1StateRun]();
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
    /* type the code to do when going from the INIT to the FAULT state */
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

    /* clear required speed */
    g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;
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
    /* type the code to do when going from the INIT to the FAULT state */
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);
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
    /* Type the code to do when going from the STOP to the FAULT state */
    /* Disable PWM output */
    g_sM1Drive.sFaultId = g_sM1Drive.sFaultIdPending;
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;
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
    /* pass calibration routine duration to state counter*/
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeCalibration;

    /* initialize calibration state */
    M1_MCDRV_CURR_CALIB_INIT(&g_sM1AdcSensor);

    /* got to CALIB sub-state */
    s_eM1StateRun = kRunState_Calib;

    /* acknowledge that the system can proceed into the RUN state */
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
    /* turn off application */
    g_bM1SwitchAppOnOff = 0;

    /* clear required speed */
    g_sM1Drive.sCtrlBLDC.f16SpeedCmd = FRAC16(0.0);

    /* clear current limitation flag */
    g_sM1Drive.sCtrlBLDC.bSpeedPiStopIntFlag = FALSE;

    /* set long free-wheel period */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelLong;

    /* go to FREEWHEEL sub-state */
    s_eM1StateRun = kRunState_Freewheel;
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
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

    g_sM1Drive.sCtrlBLDC.bSpeedPiStopIntFlag = FALSE;

    /* reinitialize PI controllers and duty cycle */
    GFLIB_CtrlPIpAWInit_F16(FRAC16(0.0), &g_sM1Drive.sCtrlBLDC.sIDcBusPiParams);
    g_sM1Drive.sCtrlBLDC.f16IDcBusPiOutput = FRAC16(0.0);
    GFLIB_CtrlPIpAWInit_F16(FRAC16(0.0), &g_sM1Drive.sCtrlBLDC.sSpeedPiParams);
    g_sM1Drive.sCtrlBLDC.f16SpeedPiOutput = FRAC16(0.0);
    g_sM1Drive.sCtrlBLDC.f16DutyCycle = FRAC16(0.0);

    /* clear speed command */
    g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;

    /* enter READY sub-state */
    s_eM1StateRun = kRunState_Ready;

    /* acknowledge that the system can proceed into the STOP state */
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
}

/*!
 * @brief Alignment process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunAlignFast(void)
{
    /* type the code to do when in the RUN ALIGN sub-state */
    MCS_BLDCAlignment(&g_sM1Drive.sCtrlBLDC);

    /* update PWM duty cycle and set sector */
    M1_MCDRV_PWM3PH_SET_DUTY(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.f16DutyCycle);
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
    if (g_sM1Drive.ui16CounterStartCmt == 0)
    {
        /* Transition to the RUN STARTUP sub-state */
        M1_TransRunStartupSpin();
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
    /* clear sensorless commutation flag */
    g_sM1Drive.bCommutatedSnsless = FALSE;

    /* check Toff period after commutation event */
    if ((uint16_t)MLIB_SubSat_F16(g_sM1Drive.ui16TimeCurrent, g_sM1Drive.ui16TimeOfCmt) >= g_sM1Drive.ui16PeriodToff)
    {
        /* mirror BEMF voltage according to the sector (change falling BEMF to rising BEMF) */
        if (!((g_sM1Drive.sCtrlBLDC.i16SectorCmt ^ g_sM1Drive.sCtrlBLDC.ui16MotorDir) & 1))
            g_sM1Drive.sCtrlBLDC.f16UPhaseBemf = MLIB_Neg_F16(g_sM1Drive.sCtrlBLDC.f16UPhaseBemf);

        /* integrate if positive BEMF voltage */
        if (g_sM1Drive.sCtrlBLDC.f16UPhaseBemf > 0)
        {
            if (g_sM1Drive.f32UBemfIntegSum == 0)
            {
                g_sM1Drive.sCtrlBLDC.f16IDcBusZC = g_sM1Drive.sCtrlBLDC.f16IDcBus;
            }
            g_sM1Drive.f32UBemfIntegSum += (frac32_t)g_sM1Drive.sCtrlBLDC.f16UPhaseBemf;
        }

        /* check whether the integral of BEMF voltage threshold has been reached */
        if (g_sM1Drive.f32UBemfIntegSum >= g_sM1Drive.f32UBemfIntegThreshold)
        {
            /* check whether commutation period was longer than minimum */
            if ((uint16_t)MLIB_SubSat_F16(g_sM1Drive.ui16TimeCurrent, g_sM1Drive.ui16TimeOfCmt) < (M1_CMT_PER_MIN >> 1))
            {
                /* PWM output disable request - actual commutation period was shorter than minimum */
                M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

                /* set long free-wheel period */
                g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelLong;

                /* go to FREEWHEEL state */
                M1_TransRunSpinFreewheel();
                return;
            }

            /* save commutation time and periods */
            g_sM1Drive.ui16TimeOfCmtOld = g_sM1Drive.ui16TimeOfCmt;
            g_sM1Drive.ui16TimeOfCmt = g_sM1Drive.ui16TimeCurrent;
            g_sM1Drive.ui16PeriodCmtNext =
                (uint16_t)MLIB_Sub_F16(g_sM1Drive.ui16TimeOfCmt, g_sM1Drive.ui16TimeOfCmtOld);
            g_sM1Drive.ui16PeriodCmt[g_sM1Drive.sCtrlBLDC.i16SectorCmt] = g_sM1Drive.ui16PeriodCmtNext;

            /* perform commutation */
            MCS_BLDCCommutation(&g_sM1Drive.sCtrlBLDC);

            /* request sector change */
            M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.i16SectorCmt);

            /* calculate Toff time */
            g_sM1Drive.ui16PeriodToff = MLIB_Mul_F16(g_sM1Drive.ui16PeriodCmtNext, g_sM1Drive.ui16PeriodToffInit);

            /* calculate next time of safety commutation  */
            g_sM1Drive.ui16PeriodCmtNext = MLIB_Sh1LSat_F16(g_sM1Drive.ui16PeriodCmtNext);

            /* request next time event */
            g_sM1Drive.ui16TimeNextEvent = MLIB_Add_F16(g_sM1Drive.ui16TimeCurrent, g_sM1Drive.ui16PeriodCmtNext);
            M1_MCDRV_TMR_CMT_SET(&g_sM1CmtTmr, g_sM1Drive.ui16TimeNextEvent);

            /* reset BEMF integral value */
            g_sM1Drive.f32UBemfIntegSum = 0;

            /* decrement commutation error counter */
            if (g_sM1Drive.ui16CounterCmtError > 0)
                g_sM1Drive.ui16CounterCmtError--;

            /* raise sensorless commutation flag */
            g_sM1Drive.bCommutatedSnsless = TRUE;
        }
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
    /* type the code to do when in the RUN CALIB sub-state */

    /* integrate DC-bus current offset */
    M1_MCDRV_CURR_CALIB(&g_sM1AdcSensor);

    /* wait until the calibration duration passes */
    if (--g_sM1Drive.ui16CounterState == 0)
    {
        /* set the DC-bus measurement offset value */
        M1_MCDRV_CURR_CALIB_SET(&g_sM1AdcSensor);

        /* Transition to the RUN READY sub-state */
        M1_TransRunCalibReady();
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
    /* if the required speed is higher than minimal, switch to Ready state */
    if (MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedCmd) > g_sM1Drive.sCtrlBLDC.f16SpeedMinimal)
    {
        /* check required spin direction */
        if (g_sM1Drive.sCtrlBLDC.f16SpeedCmd > 0)
            g_sM1Drive.sCtrlBLDC.ui16MotorDir = 0;
        else
            g_sM1Drive.sCtrlBLDC.ui16MotorDir = 1;

        /* Transition to the RUN ALIGN sub-state */
        M1_TransRunReadyAlign();
    }
}

/*!
 * @brief Alignment process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_StateRunAlignSlow(void)
{
    /* check if the required speed is lower than minimal */
    if (MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedCmd) < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal)
    {
        s_eM1StateRun = kRunState_Freewheel;
        return;
    }

    if (--g_sM1Drive.ui16CounterState == 0)
    {
        /* Transition to the RUN STARTUP sub-state */
        M1_TransRunAlignStartup();
    }

    /* If zero speed command go back to Ready */
    if (g_sM1Drive.sCtrlBLDC.f16SpeedCmd == 0)
        M1_TransRunAlignReady();
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
    /* check if the required speed is lower than minimal */
    if (MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedCmd) < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal)
    {
        /* Transition to the RUN FREEWHEEL sub-state */
        M1_TransRunStartupFreewheel();
    }
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
    /* variables are added due to issue with optimization */
    frac16_t f16SpeedMeasTemp = 0;
    frac16_t f16SpeedRampTemp = 0;

    /* calculate absolute value of average speed */
    g_sM1Drive.sCtrlBLDC.ui32PeriodSixCmtSum =
        ((uint32_t)g_sM1Drive.ui16PeriodCmt[0]) + ((uint32_t)g_sM1Drive.ui16PeriodCmt[1]) +
        ((uint32_t)g_sM1Drive.ui16PeriodCmt[2]) + ((uint32_t)g_sM1Drive.ui16PeriodCmt[3]) +
        ((uint32_t)g_sM1Drive.ui16PeriodCmt[4]) + ((uint32_t)g_sM1Drive.ui16PeriodCmt[5]);

    /* call BLDC control loop - speed and current PI controller */
    MCS_BLDCControl(&g_sM1Drive.sCtrlBLDC);

    /* update duty cycle */
    M1_MCDRV_PWM3PH_SET_DUTY(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.f16DutyCycle);

    /* check minimal speed boundary */
    f16SpeedMeasTemp = MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedMeasured);
    f16SpeedRampTemp = MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedRamp);
    if ((f16SpeedMeasTemp < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal) ||
        (f16SpeedRampTemp < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal))
    {
        /* set long free-wheel period */
        g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelLong;

        /* clear measured speed values */
        g_sM1Drive.sCtrlBLDC.f16SpeedMeasured = 0;

        /* go to FREEWHEEL state */
        M1_TransRunSpinFreewheel();
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
    if (--g_sM1Drive.ui16CounterState == 0)
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
    /* switch to sub state READY */
    s_eM1StateRun = kRunState_Ready;
}

/*!
 * @brief Transition from Ready to Align state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunReadyAlign(void)
{
    /* request alignment vector and zero duty cycle */
    g_sM1Drive.sCtrlBLDC.f16DutyCycle = FRAC16(0.0);

    /* update PWM duty cycle and set sector */
    M1_MCDRV_PWM3PH_SET_DUTY(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.f16DutyCycle);

    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 6);

    /* init alignment period */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeAlignment;

    /* initialize align current controller */
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16InErrK_1 = FRAC16(0.0);
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f32IAccK_1 = MLIB_Conv_F32s(g_sM1Drive.sCtrlBLDC.f16DutyCycle);
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16UpperLim = M1_CURRENT_CONTROLLER_ALIGN_LIM_HIGH;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16LowerLim = M1_CURRENT_CONTROLLER_ALIGN_LIM_LOW;

    /* Sub-state RUN ALIGN */
    s_eM1StateRun = kRunState_Align;
}

/*!
 * @brief Transition from Align to Startup state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunAlignStartup(void)
{
    /* alignment is done, proceed to startup */
    /* initialize startup commutation period counter */
    g_sM1Drive.ui16CounterStartCmt = g_sM1Drive.ui16StartCmtNumber;

    /* initialize next commutation period time */
    g_sM1Drive.ui16PeriodCmtNext = g_sM1Drive.ui16PeriodCmtNextInit;

    /* request next time event */
    g_sM1Drive.ui16TimeNextEvent = MLIB_Add_F16(g_sM1Drive.ui16TimeCurrent, g_sM1Drive.ui16PeriodCmtNext);

    M1_MCDRV_TMR_CMT_SET(&g_sM1CmtTmr, g_sM1Drive.ui16TimeNextEvent);

    /* select next sector based on required spin direction */
    if (g_sM1Drive.sCtrlBLDC.ui16MotorDir == 0)
        g_sM1Drive.sCtrlBLDC.i16SectorCmt = 2;
    else
        g_sM1Drive.sCtrlBLDC.i16SectorCmt = 5;

    /* request sector change */
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.i16SectorCmt);

    /* Go to sub-state RUN STARTUP */
    s_eM1StateRun = kRunState_Startup;
}

/*!
 * @brief Transition from Align to Ready state
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_TransRunAlignReady(void)
{
    /* Go to sub-state RUN READY */
    s_eM1StateRun = kRunState_Ready;
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
    /* startup is done, initialize startup */
    /* calculate Toff period */
    g_sM1Drive.ui16PeriodToff = MLIB_Mul_F16(g_sM1Drive.ui16PeriodCmtNext, g_sM1Drive.ui16PeriodToffInit);

    /* initialize commutation period buffer */
    g_sM1Drive.ui16PeriodCmt[0] = g_sM1Drive.ui16PeriodCmtNext;
    g_sM1Drive.ui16PeriodCmt[1] = g_sM1Drive.ui16PeriodCmtNext;
    g_sM1Drive.ui16PeriodCmt[2] = g_sM1Drive.ui16PeriodCmtNext;
    g_sM1Drive.ui16PeriodCmt[3] = g_sM1Drive.ui16PeriodCmtNext;
    g_sM1Drive.ui16PeriodCmt[4] = g_sM1Drive.ui16PeriodCmtNext;
    g_sM1Drive.ui16PeriodCmt[5] = g_sM1Drive.ui16PeriodCmtNext;

    /* clear BEMF integrator */
    g_sM1Drive.f32UBemfIntegSum = 0;

    /* DC-bus current PI controller initialization */
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16UpperLim = g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16UpperLim;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16LowerLim = g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16LowerLim;
    g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16InErrK_1 = FRAC16(0.0);

    /* speed PI controller initialization */
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16InErrK_1 = FRAC16(0.0);
    g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f32IAccK_1 = MLIB_Conv_F32s(g_sM1Drive.sCtrlBLDC.f16DutyCycle);

    /* reset commutation error counter */
    g_sM1Drive.ui16CounterCmtError = 0;

    /* initialize speed ramp based on spin direction */
    if (g_sM1Drive.sCtrlBLDC.ui16MotorDir == 0)
    {
        /* forward */
        g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32State = MLIB_Conv_F32s(M1_N_START_TRH);
    }
    else
    {
        /* backward */
        g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32State = MLIB_Conv_F32s(-M1_N_START_TRH);
    }

    /* this is last startup commutation, safety commutation will be
       performed in close-loop (if not commutated in sensorless mode) */
    g_sM1Drive.ui16TimeNextEvent = MLIB_Add_F16(g_sM1Drive.ui16TimeCurrentEvent, (g_sM1Drive.ui16PeriodCmtNext << 1));
    g_sM1Drive.ui16TimeOfCmt = g_sM1Drive.ui16TimeCurrent;
    g_sM1Drive.f32UBemfIntegSum = 0;

    /* request next time event (commutation ISR) */
    M1_MCDRV_TMR_CMT_SET(&g_sM1CmtTmr, g_sM1Drive.ui16TimeNextEvent);

    /* Go to sub-state RUN SPIN */
    s_eM1StateRun = kRunState_Spin;
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
    /* set long free-wheel period - expected motor spinning */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelLong;

    /* enter FREEWHEEL sub-state */
    s_eM1StateRun = kRunState_Freewheel;
}

/*!
 * @brief Transition from Startup to Free-wheel state
 *
 * @param void  No input parameter
 *
 * @return None
 */ static void M1_TransRunStartupFreewheel(void)
{
    /* required speed is below minimum - go to free-wheel */
    /* set short free-wheel period */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelShort;

    /* unstable or very low speed - PWM output disable request */
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

    /* enter FREEWHEEL sub-state */
    s_eM1StateRun = kRunState_Freewheel;
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
    /* Type the code to do when going from the RUN FREEWHEEL to the RUN READY sub-state */
    if (MLIB_AbsSat_F16(g_sM1Drive.sCtrlBLDC.f16SpeedCmd) < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal)
        g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;

    /* PWM output disable request */
    M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

    /* reinitialize PI controllers and duty cycle */
    GFLIB_CtrlPIpAWInit_F16(FRAC16(0.0), &g_sM1Drive.sCtrlBLDC.sIDcBusPiParams);
    g_sM1Drive.sCtrlBLDC.f16IDcBusPiOutput = FRAC16(0.0);
    GFLIB_CtrlPIpAWInit_F16(FRAC16(0.0), &g_sM1Drive.sCtrlBLDC.sSpeedPiParams);
    g_sM1Drive.sCtrlBLDC.f16SpeedPiOutput = FRAC16(0.0);
    g_sM1Drive.sCtrlBLDC.f16DutyCycle = FRAC16(0.0);

    /* Sub-state RUN READY */
    s_eM1StateRun = kRunState_Ready;
}

/*!
 * @brief Forced commutation if regular commutation not detected using BEMF method
 *
 * @param void  No input parameter
 *
 * @return None
 */
void M1_TimeEvent(void)
{
    /* read timer counter and value registers */
    M1_MCDRV_TMR_CMT_GET(&g_sM1CmtTmr);

    /* take action based on current RUN sub-state */
    switch (s_eM1StateRun)
    {
        /* RUN sub-state STARTUP is active  */
        case kRunState_Startup:
            /* perform commutation */
            MCS_BLDCCommutation(&g_sM1Drive.sCtrlBLDC);
            /* request sector change */
            M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.i16SectorCmt);

            /* set next startup commutation period */
            g_sM1Drive.ui16PeriodCmtNext =
                MLIB_Mul_F16(g_sM1Drive.ui16PeriodCmtNext, g_sM1Drive.f16StartCmtAcceleration);
            g_sM1Drive.ui16TimeNextEvent = MLIB_Add_F16(g_sM1Drive.ui16TimeCurrentEvent, g_sM1Drive.ui16PeriodCmtNext);

            /* request next time event (commutation ISR) */
            M1_MCDRV_TMR_CMT_SET(&g_sM1CmtTmr, g_sM1Drive.ui16TimeNextEvent);

            /* decrement startup commutation timer */
            g_sM1Drive.ui16CounterStartCmt--;
            break;

        /* RUN sub-state SPIN is active  */
        case kRunState_Spin:
            /* check whether commutation happened in ADC ISR just before this ISR */
            if (g_sM1Drive.bCommutatedSnsless)
                return;

            /* check commutation stability (number of commutation errors) */
            if (g_sM1Drive.ui16CounterCmtError > (M1_MAX_CMT_ERRORS * 3))
            {
                /* unstable or very low speed - PWM output disable request */
                M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, 7);

                /* set long free-wheel time period */
                g_sM1Drive.ui16CounterState = g_sM1Drive.ui16PeriodFreewheelLong;

                /* clear required speed */
                g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;

                /* go to FREEWHEEL state */
                M1_TransRunSpinFreewheel();
                return;
            }
            else
            {
                /* increment error counter */
                g_sM1Drive.ui16CounterCmtError += 3;
            }

            /* calculate next commutation time and period */
            g_sM1Drive.ui16TimeOfCmtOld = g_sM1Drive.ui16TimeOfCmt;
            g_sM1Drive.ui16TimeOfCmt = g_sM1Drive.ui16TimeCurrent;
            g_sM1Drive.ui16PeriodCmtNext = MLIB_Sub_F16(g_sM1Drive.ui16TimeOfCmt, g_sM1Drive.ui16TimeOfCmtOld);
            g_sM1Drive.ui16PeriodCmt[g_sM1Drive.sCtrlBLDC.i16SectorCmt] = g_sM1Drive.ui16PeriodCmtNext;

            /* perform commutation */
            MCS_BLDCCommutation(&g_sM1Drive.sCtrlBLDC);
            /* request sector change */
            M1_MCDRV_PWM3PH_SET_PWM_OUTPUT(&g_sM1Pwm3ph, g_sM1Drive.sCtrlBLDC.i16SectorCmt);
            /* clear BEMF integrator */
            g_sM1Drive.f32UBemfIntegSum = 0;

            /* calculate Toff period */
            g_sM1Drive.ui16PeriodToff = MLIB_Mul_F16(g_sM1Drive.ui16PeriodCmtNext, g_sM1Drive.ui16PeriodToffInit);

            /* request next time event to double of last commutation period */
            g_sM1Drive.ui16PeriodCmtNext = MLIB_Sh1LSat_F16(g_sM1Drive.ui16PeriodCmtNext);

            /* calculate next time of forced commutation */
            g_sM1Drive.ui16TimeNextEvent = MLIB_Add_F16(g_sM1Drive.ui16TimeCurrent, g_sM1Drive.ui16PeriodCmtNext);
            /* request next time event (commutation ISR) */
            M1_MCDRV_TMR_CMT_SET(&g_sM1CmtTmr, g_sM1Drive.ui16TimeNextEvent);
            break;

        default:
            break;
    }
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
    /* Clearing actual faults before detecting them again  */
    /* Clear all faults */
    MC_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* Fault:   DC-bus over-current */
    if (g_sM1Drive.sCtrlBLDC.f16IDcBus > g_sM1Drive.sFaultThresholds.f16IDcBusOver)
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_I_DCBUS_OVER);

    /* Fault:   DC-bus over-voltage */
    if (g_sM1Drive.sCtrlBLDC.f16UDcBus > g_sM1Drive.sFaultThresholds.f16UDcBusOver)
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_OVER);

    /* Fault:   DC-bus under-voltage */
    if (g_sM1Drive.sCtrlBLDC.f16UDcBus < g_sM1Drive.sFaultThresholds.f16UDcBusUnder)
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_UNDER);

    /* pass fault to Fault ID */
    g_sM1Drive.sFaultId |= g_sM1Drive.sFaultIdPending;
}

/*******************************************************************************
 * API
 ******************************************************************************/

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
void M1_SetSpeed(frac16_t f16SpeedCmd)
{
    if (g_bM1SwitchAppOnOff)
    {
        /* Set speed */
        if (MLIB_Abs_F16(f16SpeedCmd) < g_sM1Drive.sCtrlBLDC.f16SpeedMinimal)
        {
            g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;
        }
        else if (MLIB_Abs_F16(f16SpeedCmd) > M1_N_NOM)
        {
            g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;
        }
        else
        {
            g_sM1Drive.sCtrlBLDC.f16SpeedCmd = f16SpeedCmd;
        }
    }
    else
    {
        /* Set zero speed */
        g_sM1Drive.sCtrlBLDC.f16SpeedCmd = 0;
    }
}

/*!
 * @brief Get spin speed of the motor in fractional value
 *
 * @param void  No input parameter
 *
 * @return frac16_t Fractional value of the current speed
 */
frac16_t M1_GetSpeed(void)
{
    /* return speed */
    return g_sM1Drive.sCtrlBLDC.f16SpeedCmd;
}

