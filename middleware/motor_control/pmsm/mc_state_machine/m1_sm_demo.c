/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_sm_demo.h"
#include "mcdrv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define M1_SVM_SECTOR_DEFAULT (2) /* default SVM sector */
#define M1_BLOCK_ROT_FAULT_SH (5) /* filter window */

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

static void M1_ClearFOCVariables(void);

static void M1_FaultDetection(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Main control structure */
mcdef_pmsm_t g_sM1Drive;

/*! @brief Main application switch */
bool_t g_bM1SwitchAppOnOff;

/*! @brief M1 structure */
m1_run_substate_t g_eM1StateRun;

/*! @brief FreeMASTER scales */
/*! DO NOT USE THEM in the code to avoid float library include */
volatile float g_fltM1voltageScale;
volatile float g_fltM1DCBvoltageScale;
volatile float g_fltM1currentScale;
volatile float g_fltM1frequencyScale;
volatile float g_fltM1speedScale;

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
    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.f16UDcBusFilt =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sFocPMSM.f16UDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

    /* Disable user application switch */
    g_bM1SwitchAppOnOff = FALSE;

    /* clear fault state manually from FreeMASTER */
    if(g_sM1Drive.bFaultClearMan)
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

    /* PMSM FOC params */
    g_sM1Drive.sFocPMSM.sIdPiParams.a32PGain = M1_D_KP_GAIN;
    g_sM1Drive.sFocPMSM.sIdPiParams.a32IGain = M1_D_KI_GAIN;
    g_sM1Drive.sFocPMSM.sIdPiParams.f16UpperLim = FRAC16(1.0);
    g_sM1Drive.sFocPMSM.sIdPiParams.f16LowerLim = FRAC16(-1.0);

    g_sM1Drive.sFocPMSM.sIqPiParams.a32PGain = M1_Q_KP_GAIN;
    g_sM1Drive.sFocPMSM.sIqPiParams.a32IGain = M1_Q_KI_GAIN;
    g_sM1Drive.sFocPMSM.sIqPiParams.f16UpperLim = FRAC16(1.0);
    g_sM1Drive.sFocPMSM.sIqPiParams.f16LowerLim = FRAC16(-1.0);

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    g_sM1Drive.sFocPMSM.f16DutyCycleLimit = M1_CLOOP_LIMIT;

    g_sM1Drive.sFocPMSM.f16UDcBus = 0;
    g_sM1Drive.sFocPMSM.f16UDcBusFilt = 0;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.f32B0 = M1_UDCB_IIR_B0;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.f32B1 = M1_UDCB_IIR_B1;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.f32A1 = M1_UDCB_IIR_A1;
    /* Filter init not to enter to fault */
    g_sM1Drive.sFocPMSM.sUDcBusFilter.f16FltBfrX[0] =
        (frac16_t)((M1_U_DCB_UNDERVOLTAGE / 2.0) + (M1_U_DCB_OVERVOLTAGE / 2.0));
    g_sM1Drive.sFocPMSM.sUDcBusFilter.f32FltBfrY[0] =
        (frac32_t)((M1_U_DCB_UNDERVOLTAGE / 2.0) + (M1_U_DCB_OVERVOLTAGE / 2.0)) << 16;

    g_sM1Drive.sAlignment.f16UdReq = M1_ALIGN_VOLTAGE;
    g_sM1Drive.sAlignment.ui16Time = M1_ALIGN_DURATION;
    g_sM1Drive.sAlignment.f16PosStep1 = FRAC16(-120.0/180.0);
    g_sM1Drive.sAlignment.f16PosStep2 = FRAC16(120.0/180.0);

    /* Position and speed observer */
    g_sM1Drive.sFocPMSM.sTo.f16PGain = M1_TO_KP_GAIN;
    g_sM1Drive.sFocPMSM.sTo.i16PGainSh = M1_TO_KP_SHIFT;
    g_sM1Drive.sFocPMSM.sTo.f16IGain = M1_TO_KI_GAIN;
    g_sM1Drive.sFocPMSM.sTo.i16IGainSh = M1_TO_KI_SHIFT;
    g_sM1Drive.sFocPMSM.sTo.f16ThGain = M1_TO_THETA_GAIN;
    g_sM1Drive.sFocPMSM.sTo.i16ThGainSh = M1_TO_THETA_SHIFT;

    g_sM1Drive.sFocPMSM.sBemfObsrv.a32IGain = M1_I_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.a32UGain = M1_U_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.a32EGain = M1_E_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.a32WIGain = M1_WI_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.a32PGain = M1_BEMF_DQ_KP_GAIN;
    g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.a32IGain = M1_BEMF_DQ_KI_GAIN;

    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.f32B0 = M1_TO_SPEED_IIR_B0;
    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.f32B1 = M1_TO_SPEED_IIR_B1;
    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.f32A1 = M1_TO_SPEED_IIR_A1;
    GDFLIB_FilterIIR1Init_F16(&g_sM1Drive.sFocPMSM.sSpeedElEstFilt);

    /* Speed params */
    g_sM1Drive.sSpeed.sSpeedPiParams.a32PGain = M1_SPEED_PI_PROP_GAIN;
    g_sM1Drive.sSpeed.sSpeedPiParams.a32IGain = M1_SPEED_PI_INTEG_GAIN;
    g_sM1Drive.sSpeed.sSpeedPiParams.f16UpperLim = M1_SPEED_LOOP_HIGH_LIMIT;
    g_sM1Drive.sSpeed.sSpeedPiParams.f16LowerLim = M1_SPEED_LOOP_LOW_LIMIT;

    g_sM1Drive.sSpeed.sSpeedRampParams.f32RampUp = M1_SPEED_RAMP_UP;
    g_sM1Drive.sSpeed.sSpeedRampParams.f32RampDown = M1_SPEED_RAMP_DOWN;

    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32B0 = M1_SPEED_IIR_B0;
    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32B1 = M1_SPEED_IIR_B1;
    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32A1 = M1_SPEED_IIR_A1;

    g_sM1Drive.sSpeed.f16SpeedCmd = 0;

    /* Open loop start up */
    g_sM1Drive.sStartUp.sSpeedIntegrator.a32Gain = M1_SCALAR_INTEG_GAIN;
    g_sM1Drive.sStartUp.f16CoeffMerging = M1_MERG_COEFF;
    g_sM1Drive.sStartUp.f16SpeedCatchUp = M1_MERG_SPEED_TRH;
    g_sM1Drive.sStartUp.f16CurrentStartup = M1_OL_START_I;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.f32RampUp = M1_OL_START_RAMP_INC;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.f32RampDown = M1_OL_START_RAMP_INC;
    g_sM1Drive.sStartUp.bOpenLoop = TRUE;

    /* Timing control and general variables */
    g_sM1Drive.ui16CounterState = 0;
    g_sM1Drive.ui16TimeFullSpeedFreeWheel = M1_FREEWHEEL_DURATION;
    g_sM1Drive.ui16TimeCalibration = M1_CALIB_DURATION;
    g_sM1Drive.ui16TimeFaultRelease = M1_FAULT_DURATION;
    g_bM1SwitchAppOnOff = FALSE;

    /* fault set to init states */
    MC_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);
    MC_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* fault thresholds */
    g_sM1Drive.sFaultThresholds.f16UDcBusOver = M1_U_DCB_OVERVOLTAGE;
    g_sM1Drive.sFaultThresholds.f16UDcBusUnder = M1_U_DCB_UNDERVOLTAGE;
    g_sM1Drive.sFaultThresholds.f16SpeedOver = M1_N_OVERSPEED;
    g_sM1Drive.sFaultThresholds.f16SpeedMin = M1_N_MIN;
    g_sM1Drive.sFaultThresholds.f16SpeedNom = M1_N_NOM;
    g_sM1Drive.sFaultThresholds.f16UqBemf = M1_U_BLOCK_TRH;
    g_sM1Drive.sFaultThresholds.ui16BlockedPerNum = M1_U_BLOCK_TRH;

    /* fault blocked rotor filter */
    g_sM1Drive.msM1BlockedRotorUqFilt.u16Sh = M1_BLOCK_ROT_FAULT_SH;

    /* Defined scaling for FreeMASTER */
    g_fltM1voltageScale = M1_U_MAX;
    g_fltM1currentScale = M1_I_MAX;
    g_fltM1DCBvoltageScale = M1_U_DCB_MAX;
    g_fltM1speedScale = M1_N_MAX;
    g_fltM1frequencyScale = M1_FREQ_MAX;

    /* Clear rest of variables  */
    M1_ClearFOCVariables();

    /* Init sensors/actuators pointers */
    /* For PWM driver */
    g_sM1Pwm3ph.psUABC            = &(g_sM1Drive.sFocPMSM.sDutyABC);
    /* For ADC driver */
    g_sM1AdcSensor.pf16UDcBus     = &(g_sM1Drive.sFocPMSM.f16UDcBus);
    g_sM1AdcSensor.psIABC         = &(g_sM1Drive.sFocPMSM.sIABC);
    g_sM1AdcSensor.pui16SVMSector = &(g_sM1Drive.sFocPMSM.ui16SectorSVM);
    g_sM1AdcSensor.pui16AuxChan   = &(g_sM1Drive.f16AdcAuxSample);

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
    /* read ADC results (ADC triggered by HW trigger from PDB) */
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.f16UDcBusFilt =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sFocPMSM.f16UDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

    /* If the user switches on  or set non-zero speed*/
    if ((g_bM1SwitchAppOnOff != 0) || (g_sM1Drive.sSpeed.f16SpeedCmd != 0))
    {
        /* Set the switch on */
        g_bM1SwitchAppOnOff = TRUE;

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

    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.f16UDcBusFilt =
        GDFLIB_FilterIIR1_F16(g_sM1Drive.sFocPMSM.f16UDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

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
    s_M1_STATE_RUN_TABLE_SLOW[g_eM1StateRun]();
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
    MC_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);

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
    /* type the code to do when going from the INIT to the FAULT state */
    /* disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;

    g_sM1Drive.sSpeed.f16SpeedCmd = 0;
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
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Enable Open loop start up */
    g_sM1Drive.sStartUp.bOpenLoop = TRUE;
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
    /* 50% duty cycle */
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);

    /* PWM duty cycles calculation and update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);

    /* Clear offset filters */
    M1_MCDRV_CURR_3PH_CALIB_INIT(&g_sM1AdcSensor);

    /* Enable PWM output */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);

    /* pass calibration routine duration to state counter*/
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeCalibration;

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
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;

    /* clear over load flag */
    g_sM1Drive.sSpeed.bSpeedPiStopInteg = FALSE;

    g_sM1Drive.sSpeed.f16SpeedCmd = 0;

    /* Clear actual speed values */
    g_sM1Drive.sSpeed.f16Speed = 0;
    g_sM1Drive.sSpeed.f16SpeedFilt = 0;
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

    g_sM1Drive.sSpeed.f16SpeedCmd = 0;

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
    if (++g_sM1Drive.sFocPMSM.ui16SectorSVM > 6)
        g_sM1Drive.sFocPMSM.ui16SectorSVM = 1;
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
    /* Clear actual speed values */
    g_sM1Drive.sSpeed.f16Speed = 0;
    g_sM1Drive.sSpeed.f16SpeedFilt = 0;
    g_sM1Drive.sFocPMSM.f16PosElEst = 0;
    g_sM1Drive.sFocPMSM.f16SpeedElEst = 0;

    if ((MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedCmd) > g_sM1Drive.sFaultThresholds.f16SpeedMin) &&
        (MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedCmd) <= g_sM1Drive.sFaultThresholds.f16SpeedNom))
    {
        /* Transition to the RUN ALIGN sub-state */
        M1_TransRunReadyAlign();
    }
    else
    {
        g_sM1Drive.sSpeed.f16SpeedCmd = 0;
    }
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
    /* When alignment elapsed go to Startup */
    if (--g_sM1Drive.ui16CounterState == 0U)
    {
        /* Transition to the RUN kRunState_Startup sub-state */
        M1_TransRunAlignStartup();
    }
    
    /* If zero speed command go back to Ready */
    if (g_sM1Drive.sSpeed.f16SpeedCmd == 0)
    {
        M1_TransRunAlignReady();
    }
    
    /* type the code to do when in the RUN ALIGN sub-state */
    /* clear actual speed values */
    g_sM1Drive.sSpeed.f16Speed = 0;
    g_sM1Drive.sSpeed.f16SpeedFilt = 0;
    g_sM1Drive.sFocPMSM.f16PosElEst = 0;
    g_sM1Drive.sFocPMSM.f16SpeedElEst = 0;

    MCS_PMSMAlignment(&g_sM1Drive.sAlignment);
    g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.sAlignment.f16PosAlign;
    MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
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
    /* If f16SpeedCmd = 0, go to Free-wheel state */
    if (g_sM1Drive.sSpeed.f16SpeedCmd == 0)
        M1_TransRunStartupFreewheel();

    /* Type the code to do when in the RUN STARTUP sub-state */
    /* pass actual estimation position to OL startup structure */
    g_sM1Drive.sStartUp.f16PosEst = g_sM1Drive.sFocPMSM.f16PosElEst;

    /*open loop startup */
    MCS_PMSMOpenLoopStartUp(&g_sM1Drive.sStartUp);

    /* Pass f16SpeedRampOpenloop to f16SpeedRamp*/
    g_sM1Drive.sSpeed.f16SpeedRamp = g_sM1Drive.sStartUp.f16SpeedRampOpenLoop;

    /* Position and speed for FOC */
    g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.sStartUp.f16PosMerged;

    /* Current control loop */
    g_sM1Drive.sFocPMSM.sIDQReq.f16D = 0;

    /* during the open loop start up the values of required Iq current are kept in pre-defined level*/
    if (g_sM1Drive.sSpeed.f16SpeedCmd > 0)
        g_sM1Drive.sFocPMSM.sIDQReq.f16Q = g_sM1Drive.sStartUp.f16CurrentStartup;
    else
        g_sM1Drive.sFocPMSM.sIDQReq.f16Q = MLIB_Neg_F16(g_sM1Drive.sStartUp.f16CurrentStartup);

    /* Init Bemf observer if open-loop speed is under SpeedCatchUp/2 */
    if (MLIB_AbsSat_F16(g_sM1Drive.sStartUp.f16SpeedRampOpenLoop) <
        MLIB_ShR_F16(g_sM1Drive.sStartUp.f16SpeedCatchUp, 1))
    {
        AMCLIB_PMSMBemfObsrvDQInit_F16(&g_sM1Drive.sFocPMSM.sBemfObsrv);
        AMCLIB_TrackObsrvInit_F16(FRAC16(0.0), &g_sM1Drive.sFocPMSM.sTo);
    }

        /* FOC */
        g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
        MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
        
        /* pass estimated speed to actual speed value */
        g_sM1Drive.sSpeed.f16Speed = g_sM1Drive.sFocPMSM.f16SpeedElEst;

    /* switch to close loop  */
    if (!g_sM1Drive.sStartUp.bOpenLoop)
    {
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
    /* Type the code to do when in the RUN SPIN sub-state */

    if (MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedRamp) < g_sM1Drive.sFaultThresholds.f16SpeedMin)
    {
        /* Sub-state RUN FREEWHEEL */
        M1_TransRunSpinFreewheel();
    }

    /* no external position sensor */    
    g_sM1Drive.sFocPMSM.bPosExtOn = FALSE;
        
    /* FOC */
    g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
    MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
    /* pass estimated speed to actual speed value */
    g_sM1Drive.sSpeed.f16Speed = g_sM1Drive.sFocPMSM.f16SpeedElEst;
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
    g_sM1Drive.sSpeed.f16Speed = 0;
    g_sM1Drive.sSpeed.f16SpeedFilt = 0;
    g_sM1Drive.sSpeed.f16SpeedRamp = 0;
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
    if (--g_sM1Drive.ui16CounterState == 0)
    {
        /* write calibrated offset values */
        M1_MCDRV_CURR_3PH_CALIB_SET(&g_sM1AdcSensor);

        /* To switch to the RUN READY sub-state */
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
    /* pass estimated speed to actual speed value */
    g_sM1Drive.sSpeed.f16Speed = g_sM1Drive.sFocPMSM.f16SpeedElEst;

    /* actual speed filter */
    g_sM1Drive.sSpeed.f16SpeedFilt = GDFLIB_FilterIIR1_F16(g_sM1Drive.sSpeed.f16Speed, &g_sM1Drive.sSpeed.sSpeedFilter);

    /* pass required speed values lower than nominal speed */
    if ((MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedCmd) > g_sM1Drive.sFaultThresholds.f16SpeedNom))
    {
        /* set required speed to nominal speed if over speed command > speed nominal */
        if (g_sM1Drive.sSpeed.f16SpeedCmd > 0)
            g_sM1Drive.sSpeed.f16SpeedCmd = g_sM1Drive.sFaultThresholds.f16SpeedNom;
        else
            g_sM1Drive.sSpeed.f16SpeedCmd = MLIB_Neg_F16(g_sM1Drive.sFaultThresholds.f16SpeedNom);
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
    /* pass estimated speed to actual speed value */
    g_sM1Drive.sSpeed.f16Speed = g_sM1Drive.sFocPMSM.f16SpeedElEst;

    /* actual speed filter */
    g_sM1Drive.sSpeed.f16SpeedFilt = GDFLIB_FilterIIR1_F16(g_sM1Drive.sSpeed.f16Speed, &g_sM1Drive.sSpeed.sSpeedFilter);

    /* pass required speed values lower than nominal speed */
    if ((MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedCmd) > g_sM1Drive.sFaultThresholds.f16SpeedNom))
    {
        /* set required speed to nominal speed if over speed command > speed nominal */
        if (g_sM1Drive.sSpeed.f16SpeedCmd > 0)
            g_sM1Drive.sSpeed.f16SpeedCmd = g_sM1Drive.sFaultThresholds.f16SpeedNom;
        else
            g_sM1Drive.sSpeed.f16SpeedCmd = MLIB_Neg_F16(g_sM1Drive.sFaultThresholds.f16SpeedNom);
    }

    if (MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedRamp) < (g_sM1Drive.sFaultThresholds.f16SpeedMin))
        M1_TransRunSpinFreewheel();

    /* call PMSM speed control */
        g_sM1Drive.sSpeed.bIqPiLimFlag = g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag;
        MCS_PMSMFocCtrlSpeed(&g_sM1Drive.sSpeed);
        g_sM1Drive.sFocPMSM.sIDQReq.f16Q = g_sM1Drive.sSpeed.f16IqReq;
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
    /* Type the code to do when going from the RUN CALIB to the RUN READY sub-state */

    /* set 50% PWM duty cycle */
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);

    /* switch to sub state READY */
    g_eM1StateRun = kRunState_Ready;
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
    /* Type the code to do when going from the RUN kRunState_Ready to the RUN kRunState_Align sub-state */
    /* Alignment duration set-up */
    g_sM1Drive.ui16CounterState = g_sM1Drive.sAlignment.ui16Time;
    /* Counter of half alignment duration */
    g_sM1Drive.sAlignment.ui16TimeHalf = MLIB_ShR_F16(g_sM1Drive.sAlignment.ui16Time, 2);

    /* set required alignment voltage to Ud */
    g_sM1Drive.sFocPMSM.sUDQReq.f16D = g_sM1Drive.sAlignment.f16UdReq;
    g_sM1Drive.sFocPMSM.sUDQReq.f16Q = 0;
    /* get required motor rotation direction */
    if(g_sM1Drive.sSpeed.f16SpeedCmd > 0)
        g_sM1Drive.sAlignment.bDirection  = TRUE;
    else
        g_sM1Drive.sAlignment.bDirection  = FALSE;

    /* enable passing required position to FOC */
    g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;

    /* disable current FOC */
    g_sM1Drive.sFocPMSM.bCurrentLoopOn = FALSE;
    g_sM1Drive.sFocPMSM.bOpenLoop = TRUE;

    /* Enable PWM output */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);

    /* Sub-state RUN ALIGN */
    g_eM1StateRun = kRunState_Align;
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
    /* Type the code to do when going from the RUN kRunState_Align to the RUN kRunState_Startup sub-state */
    /* clear application parameters */
    M1_ClearFOCVariables();

    /* pass required speed to open loop start-up structure */
    if (g_sM1Drive.sSpeed.f16SpeedCmd > FRAC16(0.0))
        g_sM1Drive.sStartUp.f16SpeedReq = g_sM1Drive.sStartUp.f16SpeedCatchUp;
    else
        g_sM1Drive.sStartUp.f16SpeedReq = MLIB_Neg_F16(g_sM1Drive.sStartUp.f16SpeedCatchUp);

    /* enable Open loop mode in main control structure */
    g_sM1Drive.sStartUp.bOpenLoop = TRUE;
    g_sM1Drive.sFocPMSM.bOpenLoop = TRUE;

    /* enable Open loop mode in FOC module */
    g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    GDFLIB_FilterIIR1Init_F16(&g_sM1Drive.sSpeed.sSpeedFilter);

    /* Go to sub-state RUN STARTUP */
    g_eM1StateRun = kRunState_Startup;
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
    /* Type the code to do when going from the RUN kRunState_Align to the RUN kRunState_Ready sub-state */
    /* Clear FOC accumulators */
    M1_ClearFOCVariables();

    /* Go to sub-state RUN READY */
    g_eM1StateRun = kRunState_Ready;
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
    /* Type the code to do when going from the RUN kRunState_Startup to the RUN kRunState_Spin sub-state */
    /* for FOC control switch open loop off in DQ observer */
    g_sM1Drive.sFocPMSM.bPosExtOn = FALSE; /* disable passing external electrical position to FOC */
    g_sM1Drive.sFocPMSM.bOpenLoop = FALSE; /* disable parallel running open-loop and estimator */

    g_sM1Drive.sSpeed.sSpeedPiParams.f32IAccK_1 = MLIB_Conv_F32s(g_sM1Drive.sFocPMSM.sIDQReq.f16Q);
    g_sM1Drive.sSpeed.sSpeedRampParams.f32State = MLIB_Conv_F32s(g_sM1Drive.sStartUp.f16SpeedRampOpenLoop);

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
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Free-wheel duration set-up */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFullSpeedFreeWheel;

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
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;

    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Generates a time gap before the alignment to assure the rotor is not rotating */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFullSpeedFreeWheel;

    g_sM1Drive.sFocPMSM.sIDQReq.f16D = 0;
    g_sM1Drive.sFocPMSM.sIDQReq.f16Q = 0;

    g_sM1Drive.sFocPMSM.sUDQReq.f16D = 0;
    g_sM1Drive.sFocPMSM.sUDQReq.f16Q = 0;

    g_sM1Drive.sFocPMSM.sIAlBe.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sIAlBe.f16Beta = 0;
    g_sM1Drive.sFocPMSM.sUAlBeReq.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sUAlBeReq.f16Beta = 0;
    g_sM1Drive.sFocPMSM.sUAlBeComp.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sUAlBeComp.f16Beta = 0;

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
    /* clear application parameters */
    M1_ClearFOCVariables();
    /* Type the code to do when going from the RUN kRunState_FreeWheel to the RUN kRunState_Ready sub-state */
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
    g_sM1Drive.sAlignment.ui16TimeHalf = 0;

    g_sM1Drive.sFocPMSM.sIABC.f16A = 0;
    g_sM1Drive.sFocPMSM.sIABC.f16B = 0;
    g_sM1Drive.sFocPMSM.sIABC.f16C = 0;
    g_sM1Drive.sFocPMSM.sIAlBe.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sIAlBe.f16Beta = 0;
    g_sM1Drive.sFocPMSM.sIDQ.f16D = 0;
    g_sM1Drive.sFocPMSM.sIDQ.f16Q = 0;
    g_sM1Drive.sFocPMSM.sIDQReq.f16D = 0;
    g_sM1Drive.sFocPMSM.sIDQReq.f16Q = 0;
    g_sM1Drive.sFocPMSM.sIDQError.f16D = 0;
    g_sM1Drive.sFocPMSM.sIDQError.f16Q = 0;
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sUAlBeReq.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sUAlBeReq.f16Beta = 0;
    g_sM1Drive.sFocPMSM.sUAlBeComp.f16Alpha = 0;
    g_sM1Drive.sFocPMSM.sUAlBeComp.f16Beta = 0;
    g_sM1Drive.sFocPMSM.sUDQReq.f16D = 0;
    g_sM1Drive.sFocPMSM.sUDQReq.f16Q = 0;
    g_sM1Drive.sFocPMSM.sAnglePosEl.f16Sin = 0;
    g_sM1Drive.sFocPMSM.sAnglePosEl.f16Cos = 0;
    g_sM1Drive.sFocPMSM.sAnglePosEl.f16Sin = 0;
    g_sM1Drive.sFocPMSM.sAnglePosEl.f16Cos = 0;
    g_sM1Drive.sFocPMSM.sIdPiParams.bLimFlag = FALSE;
    g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag = FALSE;
    g_sM1Drive.sFocPMSM.sIdPiParams.f32IAccK_1 = 0;
    g_sM1Drive.sFocPMSM.sIdPiParams.f16InErrK_1 = 0;
    g_sM1Drive.sFocPMSM.sIqPiParams.f32IAccK_1 = 0;
    g_sM1Drive.sFocPMSM.sIqPiParams.f16InErrK_1 = 0;
    GDFLIB_FilterIIR1Init_F16(&g_sM1Drive.sFocPMSM.sSpeedElEstFilt);

    /* Clear Speed control state variables */
    g_sM1Drive.sSpeed.sSpeedRampParams.f32State = 0;
    g_sM1Drive.sSpeed.f16Speed = 0;
    g_sM1Drive.sSpeed.f16SpeedFilt = 0;
    g_sM1Drive.sSpeed.f16SpeedError = 0;
    g_sM1Drive.sSpeed.f16SpeedRamp = 0;
    g_sM1Drive.sSpeed.sSpeedPiParams.f32IAccK_1 = 0;
    g_sM1Drive.sSpeed.sSpeedPiParams.bLimFlag = FALSE;
    g_sM1Drive.sSpeed.sSpeedFilter.f16FltBfrX[0] = 0;
    g_sM1Drive.sSpeed.sSpeedFilter.f32FltBfrY[0] = 0;
    g_sM1Drive.sSpeed.bSpeedPiStopInteg = FALSE;
    GDFLIB_FilterIIR1Init_F16(&g_sM1Drive.sSpeed.sSpeedFilter);

    /* Init Blocked rotor filter */
    GDFLIB_FilterMAInit_F16(FRAC16(0.0), &g_sM1Drive.msM1BlockedRotorUqFilt);

    /* Clear Startup variables */
    g_sM1Drive.sStartUp.f16PosMerged = 0;
    g_sM1Drive.sStartUp.f16PosEst = 0;
    g_sM1Drive.sStartUp.f16PosGen = 0;
    g_sM1Drive.sStartUp.f16RatioMerging = 0;
    g_sM1Drive.sStartUp.f16SpeedRampOpenLoop = 0;
    g_sM1Drive.sStartUp.f16SpeedReq = 0;
    g_sM1Drive.sStartUp.sSpeedIntegrator.f32IAccK_1 = 0;
    g_sM1Drive.sStartUp.sSpeedIntegrator.f16InValK_1 = 0;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.f32State = 0;

    /* Clear BEMF and Tracking observers state variables */
    AMCLIB_PMSMBemfObsrvDQInit_F16(&g_sM1Drive.sFocPMSM.sBemfObsrv);
    AMCLIB_TrackObsrvInit_F16(FRAC16(0.0), &g_sM1Drive.sFocPMSM.sTo);
    g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.f32IQ_1 = 0;
}

/*!
 * @brief Fault detention routine - check various faults
 *
 * @param void  No input parameter
 *
 * @return None
 */
static void M1_FaultDetection(void)
{
    /* Clearing actual faults before detecting them again  */
    /* Clear all faults */
    MC_FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* Fault:   DC-bus over-current */
    if (M1_MCDRV_PWM3PH_FLT_GET(&g_sM1Pwm3ph))
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_I_DCBUS_OVER);

    /* Fault:   DC-bus over-voltage */
    if (g_sM1Drive.sFocPMSM.f16UDcBusFilt > g_sM1Drive.sFaultThresholds.f16UDcBusOver)
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_OVER);

    /* Fault:   DC-bus under-voltage */
    if (g_sM1Drive.sFocPMSM.f16UDcBusFilt < g_sM1Drive.sFaultThresholds.f16UDcBusUnder)
        MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_U_DCBUS_UNDER);

    /* Check only in SPEED_FOC control, RUN state, kRunState_Spin and kRunState_FreeWheel sub-states */
    if ((g_sM1Ctrl.eState == kSM_AppRun) && (g_eM1StateRun == kRunState_Spin || g_eM1StateRun == kRunState_Freewheel))
    {
        /* Fault: Overload  */
        frac16_t f16SpeedFiltAbs = MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedFilt);
        frac16_t f16SpeedRampAbs = MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedRamp);

        if ((f16SpeedFiltAbs < g_sM1Drive.sFaultThresholds.f16SpeedMin) &&
            (f16SpeedRampAbs > g_sM1Drive.sFaultThresholds.f16SpeedMin) &&
            (g_sM1Drive.sSpeed.bSpeedPiStopInteg == TRUE))
            MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_LOAD_OVER);

        /* Fault: Over-speed  */
        if ((MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedFilt) > g_sM1Drive.sFaultThresholds.f16SpeedOver) &&
            (MLIB_AbsSat_F16(g_sM1Drive.sSpeed.f16SpeedCmd) > g_sM1Drive.sFaultThresholds.f16SpeedMin))
            MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_SPEED_OVER);
 
        /* Fault: Blocked rotor detection */
        /* filter of bemf Uq voltage */
        g_sM1Drive.f16BemfUqAvg = GDFLIB_FilterMA_F16(MLIB_Conv_F16l(g_sM1Drive.sFocPMSM.sBemfObsrv.sEObsrv.f32Q),
                                                      &g_sM1Drive.msM1BlockedRotorUqFilt);
        /* check the bemf Uq voltage threshold only in kRunState_Spin - RUN state */
        if ((MLIB_AbsSat_F16(g_sM1Drive.f16BemfUqAvg) < g_sM1Drive.sFaultThresholds.f16UqBemf) &&
            (g_eM1StateRun == kRunState_Spin))
            g_sM1Drive.ui16BlockRotorCnt++;
        else
            g_sM1Drive.ui16BlockRotorCnt = 0;
        /* for bemf voltage detected above limit longer than defined period number set blocked rotor fault*/
        if (g_sM1Drive.ui16BlockRotorCnt > g_sM1Drive.sFaultThresholds.ui16BlockedPerNum)
        {
            MC_FAULT_SET(g_sM1Drive.sFaultIdPending, MC_FAULT_ROTOR_BLOCKED);
            g_sM1Drive.ui16BlockRotorCnt = 0;
        }
    }
    /* pass fault to Fault ID Captured */
    g_sM1Drive.sFaultIdCaptured |= g_sM1Drive.sFaultIdPending;
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
        if (MLIB_Abs_F16(f16SpeedCmd) < g_sM1Drive.sStartUp.f16SpeedCatchUp)
        {
            g_sM1Drive.sSpeed.f16SpeedCmd = 0;
        }
        else if (MLIB_Abs_F16(f16SpeedCmd) > M1_N_NOM)
        {
            g_sM1Drive.sSpeed.f16SpeedCmd = 0;
        }
        else
        {
            g_sM1Drive.sSpeed.f16SpeedCmd = f16SpeedCmd;
        }
    }
    else
    {
        /* Set zero speed */
        g_sM1Drive.sSpeed.f16SpeedCmd = 0;
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
    return g_sM1Drive.sSpeed.f16SpeedCmd;
}

