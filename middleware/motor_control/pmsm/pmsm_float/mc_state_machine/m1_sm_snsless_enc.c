/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "m1_sm_snsless_enc.h"
#include "mc_periph_init.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define M1_SVM_SECTOR_DEFAULT (2)        /* default SVM sector */
#define M1_BLOCK_ROT_FAULT_SH (0.03125F) /* filter window */

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

RAM_FUNC_LIB
static void M1_StateFaultFast(void);
RAM_FUNC_LIB
static void M1_StateInitFast(void);
RAM_FUNC_LIB
static void M1_StateStopFast(void);
RAM_FUNC_LIB
static void M1_StateRunFast(void);

RAM_FUNC_LIB
static void M1_StateFaultSlow(void);
RAM_FUNC_LIB
static void M1_StateInitSlow(void);
RAM_FUNC_LIB
static void M1_StateStopSlow(void);
RAM_FUNC_LIB
static void M1_StateRunSlow(void);

RAM_FUNC_LIB
static void M1_TransFaultStop(void);
RAM_FUNC_LIB
static void M1_TransInitFault(void);
RAM_FUNC_LIB
static void M1_TransInitStop(void);
RAM_FUNC_LIB
static void M1_TransStopFault(void);
RAM_FUNC_LIB
static void M1_TransStopRun(void);
RAM_FUNC_LIB
static void M1_TransRunFault(void);
RAM_FUNC_LIB
static void M1_TransRunStop(void);

RAM_FUNC_LIB
static void M1_StateRunCalibFast(void);
RAM_FUNC_LIB
static void M1_StateRunReadyFast(void);
RAM_FUNC_LIB
static void M1_StateRunAlignFast(void);
RAM_FUNC_LIB
static void M1_StateRunStartupFast(void);
RAM_FUNC_LIB
static void M1_StateRunSpinFast(void);
RAM_FUNC_LIB
static void M1_StateRunFreewheelFast(void);

RAM_FUNC_LIB
static void M1_StateRunCalibSlow(void);
RAM_FUNC_LIB
static void M1_StateRunReadySlow(void);
RAM_FUNC_LIB
static void M1_StateRunAlignSlow(void);
RAM_FUNC_LIB
static void M1_StateRunStartupSlow(void);
RAM_FUNC_LIB
static void M1_StateRunSpinSlow(void);
RAM_FUNC_LIB
static void M1_StateRunFreewheelSlow(void);

RAM_FUNC_LIB
static void M1_TransRunCalibReady(void);
RAM_FUNC_LIB
static void M1_TransRunReadyAlign(void);
RAM_FUNC_LIB
static void M1_TransRunReadySpin(void);
RAM_FUNC_LIB
static void M1_TransRunAlignStartup(void);
RAM_FUNC_LIB
static void M1_TransRunAlignReady(void);
RAM_FUNC_LIB
static void M1_TransRunAlignSpin(void);
RAM_FUNC_LIB
static void M1_TransRunStartupSpin(void);
RAM_FUNC_LIB
static void M1_TransRunStartupFreewheel(void);
RAM_FUNC_LIB
static void M1_TransRunSpinFreewheel(void);
RAM_FUNC_LIB
static void M1_TransRunFreewheelReady(void);

RAM_FUNC_LIB
static void M1_ClearFOCVariables(void);

RAM_FUNC_LIB
static void M1_FaultDetection(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/

/*! @brief Main control structure */
mcdef_pmsm_t g_sM1Drive;

/*! @brief Main application switch */
bool_t g_bM1SwitchAppOnOff;

/*! @brief M1 structure */
run_substate_t g_eM1StateRun;

/*! @brief FreeMASTER scales */
/*! DO NOT USE THEM in the code to avoid float library include */
volatile float g_fltM1voltageScale;
volatile float g_fltM1DCBvoltageScale;
volatile float g_fltM1currentScale;
volatile float g_fltM1speedScale;
volatile float g_fltM1speedAngularScale;
volatile float g_fltM1speedMechanicalScale;

/*! @brief Application state machine table - fast */
const sm_app_state_fcn_t s_M1_STATE_FAST = {M1_StateFaultFast, M1_StateInitFast, M1_StateStopFast, M1_StateRunFast};

/*! @brief Application state machine table - slow */
const sm_app_state_fcn_t s_M1_STATE_SLOW = {M1_StateFaultSlow, M1_StateInitSlow, M1_StateStopSlow, M1_StateRunSlow};

/*! @brief Application sub-state function field - fast */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_FAST[6] = {
    M1_StateRunCalibFast, M1_StateRunReadyFast,     M1_StateRunAlignFast,  M1_StateRunStartupFast,
    M1_StateRunSpinFast,  M1_StateRunFreewheelFast};

/*! @brief Application sub-state function field - slow */
static const pfcn_void_void s_M1_STATE_RUN_TABLE_SLOW[6] = {
    M1_StateRunCalibSlow, M1_StateRunReadySlow,     M1_StateRunAlignSlow,  M1_StateRunStartupSlow,
    M1_StateRunSpinSlow,  M1_StateRunFreewheelSlow};

/*! @brief Application state-transition functions field  */
static const sm_app_trans_fcn_t s_TRANS = {M1_TransFaultStop, M1_TransInitFault, M1_TransInitStop, M1_TransStopFault,
                                           M1_TransStopRun,   M1_TransRunFault,  M1_TransRunStop};

/*! @brief  State machine structure declaration and initialization */
sm_app_ctrl_t g_sM1Ctrl = {
    /* g_sM1Ctrl.psState, User state functions  */
    &s_M1_STATE_FAST,

    /* g_sM1Ctrl.psState, User state functions  */
    &s_M1_STATE_SLOW,

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
RAM_FUNC_LIB
static void M1_StateFaultFast(void)
{
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* convert voltages from fractional measured values to float */
    g_sM1Drive.sFocPMSM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocPMSM.fltUDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

    /* Braking resistor control with hysteresis */
    if (g_sM1Drive.sFocPMSM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F + M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_SET();
    }
    else if (g_sM1Drive.sFocPMSM.fltUDcBusFilt < g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F - M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_CLEAR();
    }

    /* Disable user application switch */
    g_bM1SwitchAppOnOff = FALSE;

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
RAM_FUNC_LIB
static void M1_StateInitFast(void)
{
    /* Type the code to do when in the INIT state */
    g_sM1Drive.sFocPMSM.sIdPiParams.fltInErrK_1 = 0.0F;
    g_sM1Drive.sFocPMSM.sIdPiParams.bLimFlag    = FALSE;

    g_sM1Drive.sFocPMSM.sIqPiParams.fltInErrK_1 = 0.0F;
    g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag    = FALSE;

    /* PMSM FOC params */
    g_sM1Drive.sFocPMSM.sIdPiParams.fltPGain    = M1_D_KP_GAIN;
    g_sM1Drive.sFocPMSM.sIdPiParams.fltIGain    = M1_D_KI_GAIN;
    g_sM1Drive.sFocPMSM.sIdPiParams.fltUpperLim = M1_U_MAX;
    g_sM1Drive.sFocPMSM.sIdPiParams.fltLowerLim = -M1_U_MAX;

    g_sM1Drive.sFocPMSM.sIqPiParams.fltPGain    = M1_Q_KP_GAIN;
    g_sM1Drive.sFocPMSM.sIqPiParams.fltIGain    = M1_Q_KI_GAIN;
    g_sM1Drive.sFocPMSM.sIqPiParams.fltUpperLim = M1_U_MAX;
    g_sM1Drive.sFocPMSM.sIqPiParams.fltLowerLim = -M1_U_MAX;

    g_sM1Drive.sFocPMSM.ui16SectorSVM     = M1_SVM_SECTOR_DEFAULT;
    g_sM1Drive.sFocPMSM.fltDutyCycleLimit = M1_CLOOP_LIMIT;

    g_sM1Drive.sFocPMSM.fltUDcBus                     = 0.0F;
    g_sM1Drive.sFocPMSM.fltUDcBusFilt                 = 0.0F;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB0 = M1_UDCB_IIR_B0;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB1 = M1_UDCB_IIR_B1;
    g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltA1 = M1_UDCB_IIR_A1;
    /* Filter init not to enter to fault */
    g_sM1Drive.sFocPMSM.sUDcBusFilter.fltFltBfrX[0] = (M1_U_DCB_UNDERVOLTAGE / 2.0F) + (M1_U_DCB_OVERVOLTAGE / 2.0F);
    g_sM1Drive.sFocPMSM.sUDcBusFilter.fltFltBfrY[0] = (M1_U_DCB_UNDERVOLTAGE / 2.0F) + (M1_U_DCB_OVERVOLTAGE / 2.0F);

    g_sM1Drive.sAlignment.fltUdReq = M1_ALIGN_VOLTAGE;
    g_sM1Drive.sAlignment.ui16Time = M1_ALIGN_DURATION;

    /* Openloop spin mode initialization */
    g_sM1Drive.sOpenloop.f16Theta = FRAC16(0);
    g_sM1Drive.sOpenloop.sUDQReq.fltD = 0.0F;
    g_sM1Drive.sOpenloop.sUDQReq.fltQ = 0.0F;
    g_sM1Drive.sOpenloop.sIDQReq.fltD = 0.0F;
    g_sM1Drive.sOpenloop.sIDQReq.fltQ = 0.0F;

    g_sM1Drive.sOpenloop.bCurrentControl = FALSE;

    GFLIB_IntegratorInit_F16(FRAC16(0.0F), &g_sM1Drive.sOpenloop.sFreqIntegrator);
    g_sM1Drive.sOpenloop.sFreqIntegrator.a32Gain = M1_SCALAR_INTEG_GAIN;
    g_sM1Drive.sOpenloop.fltFreqMax = M1_FREQ_MAX;
    g_sM1Drive.sOpenloop.fltFreqReq = 0.0F;

    /* Position and speed observer */
    g_sM1Drive.sFocPMSM.sTo.fltPGain  = M1_TO_KP_GAIN;
    g_sM1Drive.sFocPMSM.sTo.fltIGain  = M1_TO_KI_GAIN;
    g_sM1Drive.sFocPMSM.sTo.fltThGain = M1_TO_THETA_GAIN;

    g_sM1Drive.sFocPMSM.sBemfObsrv.fltIGain       = M1_I_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.fltUGain       = M1_U_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.fltEGain       = M1_E_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.fltWIGain      = M1_WI_SCALE;
    g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain = M1_BEMF_DQ_KP_GAIN;
    g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain = M1_BEMF_DQ_KI_GAIN;

    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB0 = M1_TO_SPEED_IIR_B0;
    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB1 = M1_TO_SPEED_IIR_B1;
    g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltA1 = M1_TO_SPEED_IIR_A1;
    GDFLIB_FilterIIR1Init_FLT(&g_sM1Drive.sFocPMSM.sSpeedElEstFilt);

    /* Speed params */
    g_sM1Drive.sSpeed.sSpeedPiParams.fltPGain    = M1_SPEED_PI_PROP_GAIN;
    g_sM1Drive.sSpeed.sSpeedPiParams.fltIGain    = M1_SPEED_PI_INTEG_GAIN;
    g_sM1Drive.sSpeed.sSpeedPiParams.fltUpperLim = M1_SPEED_LOOP_HIGH_LIMIT;
    g_sM1Drive.sSpeed.sSpeedPiParams.fltLowerLim = M1_SPEED_LOOP_LOW_LIMIT;

    g_sM1Drive.sSpeed.sSpeedRampParams.fltRampUp   = M1_SPEED_RAMP_UP;
    g_sM1Drive.sSpeed.sSpeedRampParams.fltRampDown = M1_SPEED_RAMP_DOWN;

    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB0 = M1_SPEED_IIR_B0;
    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB1 = M1_SPEED_IIR_B1;
    g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltA1 = M1_SPEED_IIR_A1;

    g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;

    /* Position params */
    g_sM1Drive.sPosition.f16PositionPGain = M1_POS_P_PROP_GAIN;
    g_sM1Drive.sPosition.a32Position      = ACC32(0.0);
    g_sM1Drive.sPosition.a32PositionError = ACC32(0.0);
    g_sM1Drive.sPosition.a32PositionCmd   = ACC32(0.0);
    g_sM1Drive.sPosition.f16SpeedReq      = FRAC16(0.0);

    /* Scalar control params */
    g_sM1Drive.sScalarCtrl.fltVHzGain                  = M1_SCALAR_VHZ_FACTOR_GAIN;
    g_sM1Drive.sScalarCtrl.fltUqMin                    = M1_SCALAR_UQ_MIN;
    g_sM1Drive.sScalarCtrl.sFreqRampParams.fltRampUp   = M1_SCALAR_RAMP_UP;
    g_sM1Drive.sScalarCtrl.sFreqRampParams.fltRampDown = M1_SCALAR_RAMP_DOWN;
    g_sM1Drive.sScalarCtrl.sFreqIntegrator.a32Gain     = M1_SCALAR_INTEG_GAIN;
    g_sM1Drive.sScalarCtrl.fltFreqMax                  = M1_FREQ_MAX;

    /* Open loop start up */
    g_sM1Drive.sStartUp.sSpeedIntegrator.a32Gain             = M1_SCALAR_INTEG_GAIN;
    g_sM1Drive.sStartUp.f16CoeffMerging                      = M1_MERG_COEFF;
    g_sM1Drive.sStartUp.fltSpeedCatchUp                      = M1_MERG_SPEED_TRH;
    g_sM1Drive.sStartUp.fltCurrentStartup                    = M1_OL_START_I;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampUp   = M1_OL_START_RAMP_INC;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampDown = M1_OL_START_RAMP_INC;
    g_sM1Drive.sStartUp.fltSpeedMax                          = M1_N_MAX;
    g_sM1Drive.sStartUp.bOpenLoop                            = TRUE;

    /* MCAT cascade control variables */
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD = 0.0F;
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.ui16PospeSensor  = MCAT_SENSORLESS_CTRL;

    /* Timing control and general variables */
    g_sM1Drive.ui16CounterState           = 0U;
    g_sM1Drive.ui16TimeFullSpeedFreeWheel = M1_FREEWHEEL_DURATION;
    g_sM1Drive.ui16TimeCalibration        = M1_CALIB_DURATION;
    g_sM1Drive.ui16TimeFaultRelease       = M1_FAULT_DURATION;
    g_bM1SwitchAppOnOff                   = FALSE;
    /* Default MCAT control mode after reset */
    g_sM1Drive.eControl = kControlMode_SpeedFOC;

    /* fault set to init states */
    FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);
    FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* fault thresholds */
    g_sM1Drive.sFaultThresholds.fltUDcBusOver     = M1_U_DCB_OVERVOLTAGE;
    g_sM1Drive.sFaultThresholds.fltUDcBusUnder    = M1_U_DCB_UNDERVOLTAGE;
    g_sM1Drive.sFaultThresholds.fltUDcBusTrip     = M1_U_DCB_TRIP;
    g_sM1Drive.sFaultThresholds.fltSpeedOver      = M1_N_OVERSPEED;
    g_sM1Drive.sFaultThresholds.fltSpeedMin       = M1_N_MIN;
    g_sM1Drive.sFaultThresholds.fltSpeedNom       = M1_N_NOM;
    g_sM1Drive.sFaultThresholds.fltUqBemf         = M1_E_BLOCK_TRH;
    g_sM1Drive.sFaultThresholds.ui16BlockedPerNum = M1_E_BLOCK_PER;

    /* fault blocked rotor filter */
    g_sM1Drive.msM1BlockedRotorUqFilt.fltLambda = M1_BLOCK_ROT_FAULT_SH;

    /* Defined scaling for FreeMASTER */
    g_fltM1voltageScale         = M1_U_MAX;
    g_fltM1currentScale         = M1_I_MAX;
    g_fltM1DCBvoltageScale      = M1_U_DCB_MAX;
    g_fltM1speedScale           = M1_N_MAX;
    g_fltM1speedAngularScale    = M1_N_ANGULAR_MAX;
    g_fltM1speedMechanicalScale = (60.0F / (2.0F * FLOAT_PI));

    /* Application timing */
    g_sM1Drive.ui16FastCtrlLoopFreq = (g_sClockSetup.ui16M1PwmFreq / ((uint16_t)M1_FOC_FREQ_VS_PWM_FREQ));
    g_sM1Drive.ui16SlowCtrlLoopFreq = g_sClockSetup.ui16M1SpeedLoopFreq;
    g_sM1Drive.ui32CpuFrequency = g_sClockSetup.ui32CpuFrequency;

    /* Clear rest of variables  */
    M1_ClearFOCVariables();

    /* Init sensors/actuators pointers */
    /* For PWM driver */
    g_sM1Pwm3ph.psUABC = &(g_sM1Drive.sFocPMSM.sDutyABC);
    /* For ADC driver */
    g_sM1AdcSensor.pf16UDcBus     = &(g_sM1Drive.sFocPMSM.f16UDcBus);
    g_sM1AdcSensor.psIABC         = &(g_sM1Drive.sFocPMSM.sIABCFrac);
    g_sM1AdcSensor.pui16SVMSector = &(g_sM1Drive.sFocPMSM.ui16SectorSVM);
    g_sM1AdcSensor.pui16AuxChan   = &(g_sM1Drive.f16AdcAuxSample);

    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* For ENC driver */
    g_sM1Enc.pf16PosElEst = &(g_sM1Drive.f16PosElEnc);
    g_sM1Enc.pfltSpdMeEst = &(g_sM1Drive.fltSpeedEnc);

    /* INIT_DONE command */
    g_sM1Ctrl.uiCtrl |= SM_CTRL_INIT_DONE;

    /* Enable all MC faults */
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_I_DCBUS_OVER);
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_U_DCBUS_UNDER);
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_U_DCBUS_OVER);
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_LOAD_OVER);
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_SPEED_OVER);
    FAULT_SET(g_sM1Drive.sFaultIdEnable, FAULT_ROTOR_BLOCKED);
}

/*!
 * @brief Stop state routine called in fast state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateStopFast(void)
{
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* Set encoder direction */
    M1_MCDRV_QD_SET_DIRECTION(&g_sM1Enc);

    /* get position and speed from quadrature encoder sensor */
    M1_MCDRV_QD_GET(&g_sM1Enc);

    /* convert voltages from fractional measured values to float */
    g_sM1Drive.sFocPMSM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocPMSM.fltUDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

    /* If the user switches on or set non-zero speed*/
    if ((g_bM1SwitchAppOnOff != FALSE) || (g_sM1Drive.sSpeed.fltSpeedCmd != 0.0F))
    {
        /* Set the switch on */
        g_bM1SwitchAppOnOff = TRUE;

        /* Start command */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_START;
    }

    /* Braking resistor control with hysteresis */
    if (g_sM1Drive.sFocPMSM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F + M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_SET();
    }
    else if (g_sM1Drive.sFocPMSM.fltUDcBusFilt < g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F - M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_CLEAR();
    }

    M1_FaultDetection();

    /* If a fault occurred */
    if ((bool_t)g_sM1Drive.sFaultIdPending)
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
RAM_FUNC_LIB
static void M1_StateRunFast(void)
{
    /* get all adc samples - DC-bus voltage, current, bemf and aux sample */
    M1_MCDRV_ADC_GET(&g_sM1AdcSensor);

    /* get position and speed from quadrature encoder sensor */
    M1_MCDRV_QD_GET(&g_sM1Enc);

    /* If the user switches off */
    if (!g_bM1SwitchAppOnOff)
    {
        /* Stop command */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_STOP;

        g_sM1Drive.sPosition.a32PositionCmd = 0;
        g_sM1Drive.sPosition.a32Position    = 0;
    }

    /* detect fault */
    M1_FaultDetection();

    /* If a fault occurred */
    if (g_sM1Drive.sFaultIdPending != 0U)
    {
        /* Switches to the FAULT state */
        g_sM1Ctrl.uiCtrl |= SM_CTRL_FAULT;
    }

    /* Convert phase currents from fractional measured values to float */
    g_sM1Drive.sFocPMSM.sIABC.fltA = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.sIABCFrac.f16A, g_fltM1currentScale);
    g_sM1Drive.sFocPMSM.sIABC.fltB = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.sIABCFrac.f16B, g_fltM1currentScale);
    g_sM1Drive.sFocPMSM.sIABC.fltC = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.sIABCFrac.f16C, g_fltM1currentScale);

    /* Convert voltages from fractional measured values to float */
    g_sM1Drive.sFocPMSM.fltUDcBus = MLIB_ConvSc_FLTsf(g_sM1Drive.sFocPMSM.f16UDcBus, g_fltM1DCBvoltageScale);

    /* Sampled DC-Bus voltage filter */
    g_sM1Drive.sFocPMSM.fltUDcBusFilt =
        GDFLIB_FilterIIR1_FLT(g_sM1Drive.sFocPMSM.fltUDcBus, &g_sM1Drive.sFocPMSM.sUDcBusFilter);

    /* Braking resistor control with hysteresis */
    if (g_sM1Drive.sFocPMSM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F + M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_SET();
    }
    else if (g_sM1Drive.sFocPMSM.fltUDcBusFilt < g_sM1Drive.sFaultThresholds.fltUDcBusTrip * (1.0F - M1_U_DCB_HYSTERESIS))
    {
    	M1_BRAKE_CLEAR();
    }

    /* Run sub-state function */
    s_M1_STATE_RUN_TABLE_FAST[g_eM1StateRun]();

    /* PWM peripheral update */
    M1_MCDRV_PWM3PH_SET(&g_sM1Pwm3ph);

    /* Set current sensor for sampling - applies only to some devices. */
    M1_MCDRV_CURR_3PH_CHAN_ASSIGN(&g_sM1AdcSensor);
}

/*!
 * @brief Fault state routine called in slow state machine
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateFaultSlow(void)
{
    /* After fault condition ends wait defined time to clear fault state */
    if (!FAULT_ANY(g_sM1Drive.sFaultIdPending))
    {
        if (--g_sM1Drive.ui16CounterState == 0U)
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
static void M1_TransFaultStop(void)
{
    /* Type the code to do when going from the FAULT to the INIT state */
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
RAM_FUNC_LIB
static void M1_TransInitFault(void)
{
    /* Type the code to do when going from the INIT to the FAULT state */
    /* Disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;

    g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;
}

/*!
 * @brief Transition from Init to Stop state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransInitStop(void)
{
    /* Type the code to do when going from the INIT to the STOP state */
    /* Disable PWM outputs */
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
RAM_FUNC_LIB
static void M1_TransStopFault(void)
{
    /* Type the code to do when going from the STOP to the FAULT state */
    /* Load the fault release time to counter */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;
}

/*!
 * @brief Transition from Stop to Run state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransStopRun(void)
{
    /* Type the code to do when going from the STOP to the RUN state */
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

    /* Update modulo counter */
    M1_MCDRV_QD_SET_PULSES(&g_sM1Enc);

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
RAM_FUNC_LIB
static void M1_TransRunFault(void)
{
    /* Type the code to do when going from the RUN to the FAULT state */
    /* Disable PWM output */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFaultRelease;

    /* Clear over load flag */
    g_sM1Drive.sSpeed.bSpeedPiStopInteg = FALSE;

    g_sM1Drive.sSpeed.fltSpeedCmd         = 0.0F;
    g_sM1Drive.sScalarCtrl.fltFreqCmd     = 0.0F;
    g_sM1Drive.sScalarCtrl.sUDQReq.fltQ   = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD = 0.0F;
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD = 0.0F;

    /* Clear actual speed values */
    g_sM1Drive.sScalarCtrl.fltFreqRamp = 0.0F;
    g_sM1Drive.sSpeed.fltSpeed         = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedFilt     = 0.0F;
}

/*!
 * @brief Transition from Run to Stop state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunStop(void)
{
    /* Type the code to do when going from the RUN to the STOP state */
    /* Disable PWM outputs */
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    g_sM1Drive.sSpeed.fltSpeedCmd         = 0.0F;
    g_sM1Drive.sScalarCtrl.fltFreqCmd     = 0.0F;
    g_sM1Drive.sScalarCtrl.sUDQReq.fltQ   = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD = 0.0F;
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ = 0.0F;
    g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD = 0.0F;

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
RAM_FUNC_LIB
static void M1_StateRunCalibFast(void)
{
    /* Type the code to do when in the RUN CALIB sub-state
       performing ADC offset calibration */

    /* Call offset measurement */
    M1_MCDRV_CURR_3PH_CALIB(&g_sM1AdcSensor);

    /* Change SVM sector in range <1;6> to measure all AD channel mapping combinations */
    if (++g_sM1Drive.sFocPMSM.ui16SectorSVM > 6U)
    {
    	g_sM1Drive.sFocPMSM.ui16SectorSVM = 1U;
    }
}

/*!
 * @brief Ready state called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateRunReadyFast(void)
{
    /* Type the code to do when in the RUN READY sub-state */
    /* Clear actual speed values */
    g_sM1Drive.sScalarCtrl.fltFreqRamp = 0.0F;
    g_sM1Drive.sSpeed.fltSpeed         = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedFilt     = 0.0F;
    g_sM1Drive.sFocPMSM.f16PosElEst    = FRAC16(0.0);
    g_sM1Drive.sFocPMSM.fltSpeedElEst  = 0.0F;

    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
    	case kControlMode_OpenLoop:
    		M1_TransRunReadySpin();
			break;

        case kControlMode_Scalar:
            if (!(g_sM1Drive.sScalarCtrl.fltFreqCmd == 0.0F))
            {
                g_sM1Drive.sScalarCtrl.fltFreqRamp  = 0.0F;
                g_sM1Drive.sScalarCtrl.sUDQReq.fltQ = 0.0F;

                /* Transition to the RUN ALIGN sub-state */
                M1_TransRunReadyAlign();
            }
            break;

        case kControlMode_VoltageFOC:
            if (!(g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ == 0.0F))
            {
                if (g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ > 0.0F)
                {
                	g_sM1Drive.sSpeed.fltSpeedCmd = g_sM1Drive.sStartUp.fltSpeedCatchUp * 2.0F;
                }
                else
                {
                	g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_Neg_FLT(g_sM1Drive.sStartUp.fltSpeedCatchUp * 2.0F);
                }

                /* Transition to the RUN ALIGN sub-state */
                M1_TransRunReadyAlign();
            }
            break;

        case kControlMode_CurrentFOC:
            if (!(g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ == 0.0F))
            {
                if (g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ > 0.0F)
                {
                	g_sM1Drive.sSpeed.fltSpeedCmd = g_sM1Drive.sStartUp.fltSpeedCatchUp * 2.0F;
                }
                else
                {
                	g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_Neg_FLT(g_sM1Drive.sStartUp.fltSpeedCatchUp * 2.0F);
                }

                /* Transition to the RUN ALIGN sub-state */
                M1_TransRunReadyAlign();
            }
            break;

        default:
            /* Align function */
            /* Value of fltSpeedNom is increased because of float conversion */
            if ((g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL) ||
                ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) > g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
                 (MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) <= (g_sM1Drive.sFaultThresholds.fltSpeedNom + 0.1F))))
            {
                /* Transition to the RUN ALIGN sub-state */
                M1_TransRunReadyAlign();
            }
            else
            {
                g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;
            }
            break;
    }
}

/*!
 * @brief Alignment process called in fast state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateRunAlignFast(void)
{
    /* Type the code to do when in the RUN ALIGN sub-state */
    /* When alignment elapsed go to Startup */
    if (--g_sM1Drive.ui16CounterState == 0U)
    {
        if ((g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL) && (g_sM1Drive.eControl != kControlMode_Scalar))
        {
            /* Transition to the RUN kRunState_Spin sub-state */
            M1_TransRunAlignSpin();
        }
        else
        {
            /* Transition to the RUN kRunState_Startup sub-state */
            M1_TransRunAlignStartup();
        }
    }

    /* If zero speed command go back to Ready */
    if ((g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_SENSORLESS_CTRL) && (g_sM1Drive.sSpeed.fltSpeedCmd == 0.0F) &&
        (g_sM1Drive.sScalarCtrl.fltFreqCmd == 0.0F))
    {
    	M1_TransRunAlignReady();
    }

    /* Clear actual speed values */
    g_sM1Drive.sScalarCtrl.fltFreqRamp = 0.0F;
    g_sM1Drive.sSpeed.fltSpeed         = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedFilt     = 0.0F;
    g_sM1Drive.sFocPMSM.f16PosElEst    = FRAC16(0.0);
    g_sM1Drive.sFocPMSM.fltSpeedElEst  = 0.0F;

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
RAM_FUNC_LIB
static void M1_StateRunStartupFast(void)
{
    /* If f16SpeedCmd = 0, go to Free-wheel state */
    if ((g_sM1Drive.sSpeed.fltSpeedCmd == 0.0F) && (g_sM1Drive.eControl == kControlMode_SpeedFOC))
    {
    	M1_TransRunStartupFreewheel();
    }

    /* Type the code to do when in the RUN STARTUP sub-state */
    /* Pass actual estimation position to OL startup structure */
    g_sM1Drive.sStartUp.f16PosEst = g_sM1Drive.sFocPMSM.f16PosElEst;

    /* Open loop startup */
    MCS_PMSMOpenLoopStartUp(&g_sM1Drive.sStartUp);

    /* Pass f16SpeedRampOpenloop to f16SpeedRamp*/
    g_sM1Drive.sSpeed.fltSpeedRamp = g_sM1Drive.sStartUp.fltSpeedRampOpenLoop;

    /* Position and speed for FOC */
    g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.sStartUp.f16PosMerged;

    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
        case kControlMode_Scalar:
            /* Switch directly to SPIN state */
            M1_TransRunStartupSpin();
            break;

        case kControlMode_VoltageFOC:
            /* Pass MCAT required values in run-time */
            g_sM1Drive.sFocPMSM.sUDQReq.fltD = g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD;
            g_sM1Drive.sFocPMSM.sUDQReq.fltQ = g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ;
            /* FOC */
            g_sM1Drive.sFocPMSM.bCurrentLoopOn = FALSE;
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
            break;

        case kControlMode_CurrentFOC:
            /* FOC */
            g_sM1Drive.sFocPMSM.sIDQReq.fltD   = g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD;
            g_sM1Drive.sFocPMSM.sIDQReq.fltQ   = g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ;
            g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
            break;

        default:
            /* Current control loop */
            g_sM1Drive.sFocPMSM.sIDQReq.fltD = 0.0F;

            /* During the open loop start up the values of required Iq current are kept in pre-defined level*/
            if (g_sM1Drive.sSpeed.fltSpeedCmd > 0.0F)
            {
            	g_sM1Drive.sFocPMSM.sIDQReq.fltQ = g_sM1Drive.sStartUp.fltCurrentStartup;
            }
            else
            {
            	g_sM1Drive.sFocPMSM.sIDQReq.fltQ = MLIB_Neg_FLT(g_sM1Drive.sStartUp.fltCurrentStartup);
            }

            /* Init Bemf observer if open-loop speed is under SpeedCatchUp/2 */
            if (MLIB_Abs_FLT(g_sM1Drive.sStartUp.fltSpeedRampOpenLoop) < (g_sM1Drive.sStartUp.fltSpeedCatchUp / 2.0F))
            {
                AMCLIB_PMSMBemfObsrvDQInit_A32fff(&g_sM1Drive.sFocPMSM.sBemfObsrv);
                AMCLIB_TrackObsrvInit_A32af(ACC32(0.0), &g_sM1Drive.sFocPMSM.sTo);
            }

            /* FOC */
            g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);

            /* select source of actual speed value */
            if (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL)
            {
                /* pass encoder speed to actual speed value */
                g_sM1Drive.sSpeed.fltSpeed = g_sM1Drive.fltSpeedEnc * ((float_t)(g_sM1Enc.ui16Pp));
            }
            else
            {
                /* pass estimated speed to actual speed value */
                g_sM1Drive.sSpeed.fltSpeed = g_sM1Drive.sFocPMSM.fltSpeedElEst;
            }
            break;

    }

    /* Switch to close loop  */
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
RAM_FUNC_LIB
static void M1_StateRunSpinFast(void)
{
    /* Type the code to do when in the RUN SPIN sub-state */
    /* MCAT control structure switch */
    switch (g_sM1Drive.eControl)
    {
    	case kControlMode_OpenLoop:
    		g_sM1Drive.sFocPMSM.sUDQReq = g_sM1Drive.sOpenloop.sUDQReq;
    		g_sM1Drive.sFocPMSM.sIDQReq = g_sM1Drive.sOpenloop.sIDQReq;

			MCS_PMSMOpenloop(&g_sM1Drive.sOpenloop);

    	    g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.sOpenloop.f16PosElExt;
			MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);
			break;

        case kControlMode_Scalar:
            /* Scalar control */
            MCS_PMSMScalarCtrl(&g_sM1Drive.sScalarCtrl);

            /* Pass required voltages to Bemf Observer to work */
            g_sM1Drive.sFocPMSM.sUDQReq.fltQ = g_sM1Drive.sScalarCtrl.sUDQReq.fltQ;
            g_sM1Drive.sFocPMSM.sUDQReq.fltD = g_sM1Drive.sScalarCtrl.sUDQReq.fltD;
            g_sM1Drive.sFocPMSM.f16PosElExt  = g_sM1Drive.sScalarCtrl.f16PosElScalar;

            /* Call voltage FOC to calculate PWM duty cycles */
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);

            /* Sub-state RUN FREEWHEEL */
            if (g_sM1Drive.sScalarCtrl.fltFreqCmd == 0.0F)
            {
            	M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_VoltageFOC:
            /* FOC */
            g_sM1Drive.sFocPMSM.sUDQReq.fltQ   = g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ;
            g_sM1Drive.sFocPMSM.sUDQReq.fltD   = g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD;
            g_sM1Drive.sFocPMSM.bCurrentLoopOn = FALSE;

            /* Pass encoder position to FOC is enabled */
            if (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL)
            {
                g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.f16PosElEnc;
                g_sM1Drive.sFocPMSM.bPosExtOn   = TRUE;
            }
            else
            {
                g_sM1Drive.sFocPMSM.bPosExtOn = FALSE;
            }

            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);

            /* Sub-state RUN FREEWHEEL */
            if (g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ == 0.0F)
            {
            	M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_CurrentFOC:
            /* Current FOC */
            g_sM1Drive.sFocPMSM.sIDQReq.fltQ = g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ;
            g_sM1Drive.sFocPMSM.sIDQReq.fltD = g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD;

            /* Pass encoder position to FOC is enabled */
            if (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL)
            {
                g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.f16PosElEnc;
                g_sM1Drive.sFocPMSM.bPosExtOn   = TRUE;
            }
            else
            {
                g_sM1Drive.sFocPMSM.bPosExtOn = FALSE;
            }

            g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);

            /* Sub-state RUN FREEWHEEL */
            if (g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ == 0.0F)
            {
            	M1_TransRunSpinFreewheel();
            }
            break;

        case kControlMode_SpeedFOC:
        case kControlMode_PositionFOC:
        default:
            if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedRamp) < g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
                (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_SENSORLESS_CTRL))
            {
                /* Sub-state RUN FREEWHEEL */
                M1_TransRunSpinFreewheel();
            }

            /* Pass encoder position to FOC is enabled */
            if (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL)
            {
                g_sM1Drive.sFocPMSM.f16PosElExt = g_sM1Drive.f16PosElEnc;
                g_sM1Drive.sFocPMSM.bPosExtOn   = TRUE;
            }
            else
            {
                g_sM1Drive.sFocPMSM.bPosExtOn = FALSE;
            }

            /* FOC */
            g_sM1Drive.sFocPMSM.bCurrentLoopOn = TRUE;
            MCS_PMSMFocCtrl(&g_sM1Drive.sFocPMSM);

            /* select source of actual speed value */
            if (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_ENC_CTRL)
            {
                /* pass encoder speed to actual speed value */
                g_sM1Drive.sSpeed.fltSpeed = g_sM1Drive.fltSpeedEnc * ((float_t)(g_sM1Enc.ui16Pp));
            }
            else
            {
                /* Pass estimated speed to actual speed value */
                g_sM1Drive.sSpeed.fltSpeed = g_sM1Drive.sFocPMSM.fltSpeedElEst;
            }
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
RAM_FUNC_LIB
static void M1_StateRunFreewheelFast(void)
{
    /* Type the code to do when in the RUN FREEWHEEL sub-state */

    /* Clear actual speed values */
    g_sM1Drive.sScalarCtrl.fltFreqRamp = 0.0F;
    g_sM1Drive.sSpeed.fltSpeed         = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedFilt     = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedRamp     = 0.0F;
}

/*!
 * @brief Calibration process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateRunCalibSlow(void)
{
    if (--g_sM1Drive.ui16CounterState == 0U)
    {
	  /* Write calibrated offset values */
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
static void M1_StateRunStartupSlow(void)
{
    if (g_sM1Drive.eControl == kControlMode_SpeedFOC)
    {
        /* Actual speed filter */
        g_sM1Drive.sSpeed.fltSpeedFilt =
            GDFLIB_FilterIIR1_FLT(g_sM1Drive.sSpeed.fltSpeed, &g_sM1Drive.sSpeed.sSpeedFilter);

        /* Pass required speed values lower than nominal speed */
        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) > g_sM1Drive.sFaultThresholds.fltSpeedNom))
        {
            /* Set required speed to nominal speed if over speed command > speed nominal */
            if (g_sM1Drive.sSpeed.fltSpeedCmd > 0.0F)
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = g_sM1Drive.sFaultThresholds.fltSpeedNom;
            }
            else
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_Neg_FLT(g_sM1Drive.sFaultThresholds.fltSpeedNom);
            }
        }
    }
}

/*!
 * @brief Spin state called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateRunSpinSlow(void)
{
    if (g_sM1Drive.eControl == kControlMode_SpeedFOC)
    {
        /* Actual speed filter */
        g_sM1Drive.sSpeed.fltSpeedFilt =
            GDFLIB_FilterIIR1_FLT(g_sM1Drive.sSpeed.fltSpeed, &g_sM1Drive.sSpeed.sSpeedFilter);

        /* Pass required speed values lower than nominal speed */
        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) > g_sM1Drive.sFaultThresholds.fltSpeedNom))
        {
            /* Set required speed to nominal speed if over speed command > speed nominal */
            if (g_sM1Drive.sSpeed.fltSpeedCmd > 0.0F)
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = g_sM1Drive.sFaultThresholds.fltSpeedNom;
            }
            else
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_Neg_FLT(g_sM1Drive.sFaultThresholds.fltSpeedNom);
            }

        }

        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedRamp) < g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
            (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_SENSORLESS_CTRL))
        {
        	M1_TransRunSpinFreewheel();
        }

        /* call PMSM speed control */
        g_sM1Drive.sSpeed.bIqPiLimFlag = g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag;
        MCS_PMSMFocCtrlSpeed(&g_sM1Drive.sSpeed);
        g_sM1Drive.sFocPMSM.sIDQReq.fltQ = g_sM1Drive.sSpeed.fltIqReq;
    }

    if (g_sM1Drive.eControl == kControlMode_PositionFOC)
    {
        /* actual speed filter */
        g_sM1Drive.sSpeed.fltSpeedFilt =
            GDFLIB_FilterIIR1_FLT(g_sM1Drive.sSpeed.fltSpeed, &g_sM1Drive.sSpeed.sSpeedFilter);

        /* pass required speed values lower than nominal speed */
        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) > g_sM1Drive.sFaultThresholds.fltSpeedNom))
        {
            /* set required speed to nominal speed if over speed command > speed nominal */
            if (g_sM1Drive.sSpeed.fltSpeedCmd > 0.0F)
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = g_sM1Drive.sFaultThresholds.fltSpeedNom;
            }
            else
            {
            	g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_Neg_FLT(g_sM1Drive.sFaultThresholds.fltSpeedNom);
            }
        }

        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedRamp) < g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
            (g_sM1Drive.sMCATctrl.ui16PospeSensor == MCAT_SENSORLESS_CTRL))
        {
        	M1_TransRunSpinFreewheel();
        }

        /* Actual position */
        g_sM1Drive.sPosition.a32Position = g_sM1Enc.a32PosMeReal;

        /* Call PMSM position control */
        MCS_PMSMFocCtrlPosition(&g_sM1Drive.sPosition);

        /* Speed command is equal to position controller output */
        g_sM1Drive.sSpeed.fltSpeedCmd = MLIB_ConvSc_FLTsf(g_sM1Drive.sPosition.f16SpeedReq, M1_SPEED_CONV_SCALE);

        /* Call PMSM speed control */
        g_sM1Drive.sSpeed.bIqPiLimFlag = g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag;
        MCS_PMSMFocCtrlSpeed(&g_sM1Drive.sSpeed);
        g_sM1Drive.sFocPMSM.sIDQReq.fltQ = g_sM1Drive.sSpeed.fltIqReq;
    }
}

/*!
 * @brief Free-wheel process called in slow state machine as Run sub state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_StateRunFreewheelSlow(void)
{
    /* Wait until free-wheel time passes */
    if (--g_sM1Drive.ui16CounterState == 0U)
    {
        /* Switch to sub state READY */
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
RAM_FUNC_LIB
static void M1_TransRunCalibReady(void)
{
    /* Type the code to do when going from the RUN CALIB to the RUN READY sub-state */

    /* Set 50% PWM duty cycle */
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);

    /* Switch to sub state READY */
    g_eM1StateRun = kRunState_Ready;
}

/*!
 * @brief Transition from Ready to Align state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunReadyAlign(void)
{
    /* Type the code to do when going from the RUN kRunState_Ready to the RUN kRunState_Align sub-state */
    /* Alignment duration set-up */
    g_sM1Drive.ui16CounterState = g_sM1Drive.sAlignment.ui16Time;
    /* Counter of half alignment duration */
    g_sM1Drive.sAlignment.ui16TimeHalf = (uint16_t)MLIB_ShR_F16((int16_t)g_sM1Drive.sAlignment.ui16Time, 1);

    /* set required alignment voltage to Ud */
    g_sM1Drive.sFocPMSM.sUDQReq.fltD = g_sM1Drive.sAlignment.fltUdReq;
    g_sM1Drive.sFocPMSM.sUDQReq.fltQ = 0.0F;

    /* enable passing required position to FOC */
    g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;

    /* disable current FOC */
    g_sM1Drive.sFocPMSM.bCurrentLoopOn = FALSE;

    /* enable Open loop mode in main control structure */
    g_sM1Drive.sFocPMSM.bOpenLoop = TRUE;

    /* Enable PWM output */
    M1_MCDRV_PWM3PH_EN(&g_sM1Pwm3ph);

    /* Sub-state RUN ALIGN */
    g_eM1StateRun = kRunState_Align;
}

/*!
 * @brief Transition from Ready to Spin state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunReadySpin(void)
{
    M1_ClearFOCVariables();
	g_sM1Drive.sFocPMSM.bCurrentLoopOn = g_sM1Drive.sOpenloop.bCurrentControl;
	g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;
	g_sM1Drive.sFocPMSM.bOpenLoop = TRUE;

	g_eM1StateRun = kRunState_Spin;
}

/*!
 * @brief Transition from Align to Startup state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunAlignStartup(void)
{
    /* Type the code to do when going from the RUN kRunState_Align to the RUN kRunState_Startup sub-state */
    /* initialize encoder driver */
    M1_MCDRV_QD_CLEAR(&g_sM1Enc);

    /* Clear application parameters */
    M1_ClearFOCVariables();

    /* Pass required speed to open loop start-up structure */
    if (g_sM1Drive.sSpeed.fltSpeedCmd > 0.0F)
    {
    	g_sM1Drive.sStartUp.fltSpeedReq = g_sM1Drive.sStartUp.fltSpeedCatchUp;
    }
    else
    {
    	g_sM1Drive.sStartUp.fltSpeedReq = MLIB_Neg_FLT(g_sM1Drive.sStartUp.fltSpeedCatchUp);
    }

    /* Enable Open loop mode in main control structure */
    g_sM1Drive.sStartUp.bOpenLoop = TRUE;
    g_sM1Drive.sFocPMSM.bOpenLoop = TRUE;

    /* Enable Open loop mode in FOC module */
    g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    GDFLIB_FilterIIR1Init_FLT(&g_sM1Drive.sSpeed.sSpeedFilter);

    /* Go to sub-state RUN STARTUP */
    g_eM1StateRun = kRunState_Startup;
}

/*!
 * @brief Transition from Align to Spin state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunAlignSpin(void)
{
    /* Type the code to do when going from the RUN STARTUP to the RUN SPIN sub-state */
    /* initialize encoder driver */
    M1_MCDRV_QD_CLEAR(&g_sM1Enc);

    g_sM1Drive.sFocPMSM.bPosExtOn = TRUE;  /* enable passing external electrical position from encoder to FOC */
    g_sM1Drive.sFocPMSM.bOpenLoop = FALSE; /* disable parallel runnig openloop and estimator */

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;
    g_sM1Drive.sFocPMSM.sIDQReq.fltD  = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQReq.fltQ  = 0.0F;

    M1_ClearFOCVariables();

    /* To switch to the RUN SPIN sub-state */
    g_eM1StateRun = kRunState_Spin;
}

/*!
 * @brief Transition from Align to Ready state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
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
RAM_FUNC_LIB
static void M1_TransRunStartupSpin(void)
{
    /* Type the code to do when going from the RUN kRunState_Startup to the RUN kRunState_Spin sub-state */
    /* For FOC control switch open loop off in DQ observer */
    if (g_sM1Drive.eControl != kControlMode_Scalar)
    {
        g_sM1Drive.sFocPMSM.bPosExtOn = FALSE; /* disable passing external electrical position to FOC */
        g_sM1Drive.sFocPMSM.bOpenLoop = FALSE; /* disable parallel running open-loop and estimator */
    }

    g_sM1Drive.sSpeed.sSpeedPiParams.fltIAccK_1 = g_sM1Drive.sFocPMSM.sIDQReq.fltQ;
    g_sM1Drive.sSpeed.sSpeedRampParams.fltState = g_sM1Drive.sStartUp.fltSpeedRampOpenLoop;

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
RAM_FUNC_LIB
static void M1_TransRunStartupFreewheel(void)
{
    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Free-wheel duration set-up */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFullSpeedFreeWheel;

    /* Enter FREEWHEEL sub-state */
    g_eM1StateRun = kRunState_Freewheel;
}

/*!
 * @brief Transition from Spin to Free-wheel state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunSpinFreewheel(void)
{
    /* Type the code to do when going from the RUN SPIN to the RUN FREEWHEEL sub-state */
    /* Set 50% PWM duty cycle */
    g_sM1Drive.sFocPMSM.sDutyABC.f16A = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C = FRAC16(0.5);

    g_sM1Drive.sFocPMSM.ui16SectorSVM = M1_SVM_SECTOR_DEFAULT;

    M1_MCDRV_PWM3PH_DIS(&g_sM1Pwm3ph);

    /* Generates a time gap before the alignment to assure the rotor is not rotating */
    g_sM1Drive.ui16CounterState = g_sM1Drive.ui16TimeFullSpeedFreeWheel;

    g_sM1Drive.sFocPMSM.sIDQReq.fltD = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQReq.fltQ = 0.0F;

    g_sM1Drive.sFocPMSM.sUDQReq.fltD = 0.0F;
    g_sM1Drive.sFocPMSM.sUDQReq.fltQ = 0.0F;

    g_sM1Drive.sFocPMSM.sIAlBe.fltAlpha    = 0.0F;
    g_sM1Drive.sFocPMSM.sIAlBe.fltBeta     = 0.0F;
    g_sM1Drive.sFocPMSM.sUAlBeReq.fltAlpha = 0.0F;
    g_sM1Drive.sFocPMSM.sUAlBeReq.fltBeta  = 0.0F;

    /* Enter FREEWHEEL sub-state */
    g_eM1StateRun = kRunState_Freewheel;
}

/*!
 * @brief Transition from Free-wheel to Ready state
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_TransRunFreewheelReady(void)
{
    /* Type the code to do when going from the RUN kRunState_FreeWheel to the RUN kRunState_Ready sub-state */
    /* Clear application parameters */
    M1_ClearFOCVariables();

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
RAM_FUNC_LIB
static void M1_ClearFOCVariables(void)
{
    g_sM1Drive.sAlignment.ui16TimeHalf = 0U;

    /* Clear FOC variables */
    g_sM1Drive.sFocPMSM.sIABC.fltA             = 0.0F;
    g_sM1Drive.sFocPMSM.sIABC.fltB             = 0.0F;
    g_sM1Drive.sFocPMSM.sIABC.fltC             = 0.0F;
    g_sM1Drive.sFocPMSM.sIAlBe.fltAlpha        = 0.0F;
    g_sM1Drive.sFocPMSM.sIAlBe.fltBeta         = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQ.fltD              = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQ.fltQ              = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQReq.fltD           = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQReq.fltQ           = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQError.fltD         = 0.0F;
    g_sM1Drive.sFocPMSM.sIDQError.fltQ         = 0.0F;
    g_sM1Drive.sFocPMSM.sDutyABC.f16A          = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16B          = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sDutyABC.f16C          = FRAC16(0.5);
    g_sM1Drive.sFocPMSM.sUAlBeReq.fltAlpha     = 0.0F;
    g_sM1Drive.sFocPMSM.sUAlBeReq.fltBeta      = 0.0F;
    g_sM1Drive.sFocPMSM.sUDQReq.fltD           = 0.0F;
    g_sM1Drive.sFocPMSM.sUDQReq.fltQ           = 0.0F;
    g_sM1Drive.sFocPMSM.sAnglePosEl.fltSin     = 0.0F;
    g_sM1Drive.sFocPMSM.sAnglePosEl.fltCos     = 0.0F;
    g_sM1Drive.sFocPMSM.sAnglePosEl.fltSin     = 0.0F;
    g_sM1Drive.sFocPMSM.sAnglePosEl.fltCos     = 0.0F;
    g_sM1Drive.sFocPMSM.sIdPiParams.bLimFlag   = FALSE;
    g_sM1Drive.sFocPMSM.sIqPiParams.bLimFlag   = FALSE;
    g_sM1Drive.sFocPMSM.sIdPiParams.fltIAccK_1 = 0.0F;
    g_sM1Drive.sFocPMSM.sIdPiParams.fltIAccK_1 = 0.0F;
    g_sM1Drive.sFocPMSM.sIqPiParams.fltIAccK_1 = 0.0F;
    g_sM1Drive.sFocPMSM.sIqPiParams.fltIAccK_1 = 0.0F;
    GDFLIB_FilterIIR1Init_FLT(&g_sM1Drive.sFocPMSM.sSpeedElEstFilt);
    g_sM1Drive.sFocPMSM.bIdPiStopInteg = FALSE;
    g_sM1Drive.sFocPMSM.bIqPiStopInteg = FALSE;

    /* Clear Speed control state variables */
    g_sM1Drive.sSpeed.sSpeedRampParams.fltState  = 0.0F;
    g_sM1Drive.sSpeed.fltSpeed                   = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedFilt               = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedError              = 0.0F;
    g_sM1Drive.sSpeed.fltSpeedRamp               = 0.0F;
    g_sM1Drive.sSpeed.sSpeedPiParams.fltIAccK_1  = 0.0F;
    g_sM1Drive.sSpeed.sSpeedPiParams.bLimFlag    = FALSE;
    g_sM1Drive.sSpeed.sSpeedFilter.fltFltBfrX[0] = 0.0F;
    g_sM1Drive.sSpeed.sSpeedFilter.fltFltBfrY[0] = 0.0F;
    g_sM1Drive.sSpeed.bSpeedPiStopInteg          = FALSE;
    GDFLIB_FilterIIR1Init_FLT(&g_sM1Drive.sSpeed.sSpeedFilter);

    /* Init Blocked rotor filter */
    GDFLIB_FilterMAInit_FLT(0.0F, &g_sM1Drive.msM1BlockedRotorUqFilt);

    /* Clear Scalar control variables */
    g_sM1Drive.sScalarCtrl.fltFreqRamp                 = 0.0F;
    g_sM1Drive.sScalarCtrl.f16PosElScalar              = FRAC16(0.0);
    g_sM1Drive.sScalarCtrl.sUDQReq.fltD                = 0.0F;
    g_sM1Drive.sScalarCtrl.sUDQReq.fltQ                = 0.0F;
    g_sM1Drive.sScalarCtrl.sFreqIntegrator.f32IAccK_1  = 0;
    g_sM1Drive.sScalarCtrl.sFreqIntegrator.f16InValK_1 = 0;
    g_sM1Drive.sScalarCtrl.sFreqRampParams.fltState    = 0.0F;

    /* Clear Open Loop control variables */
    GFLIB_IntegratorInit_F16(FRAC16(0.0F), &g_sM1Drive.sOpenloop.sFreqIntegrator);
    g_sM1Drive.sOpenloop.f16PosElExt				   = FRAC16(0.0);
    g_sM1Drive.sOpenloop.f16Theta                      = FRAC16(0);
    g_sM1Drive.sOpenloop.sUDQReq.fltD                  = 0.0F;
    g_sM1Drive.sOpenloop.sUDQReq.fltQ                  = 0.0F;
    g_sM1Drive.sOpenloop.sIDQReq.fltD                  = 0.0F;
    g_sM1Drive.sOpenloop.sIDQReq.fltQ                  = 0.0F;
    g_sM1Drive.sOpenloop.fltFreqReq                    = 0.0F;

    /* Clear Startup variables */
    g_sM1Drive.sStartUp.f16PosMerged                      = 0;
    g_sM1Drive.sStartUp.f16PosEst                         = 0;
    g_sM1Drive.sStartUp.f16PosGen                         = 0;
    g_sM1Drive.sStartUp.f16RatioMerging                   = 0;
    g_sM1Drive.sStartUp.fltSpeedRampOpenLoop              = 0.0F;
    g_sM1Drive.sStartUp.fltSpeedReq                       = 0.0F;
    g_sM1Drive.sStartUp.sSpeedIntegrator.f32IAccK_1       = 0;
    g_sM1Drive.sStartUp.sSpeedIntegrator.f16InValK_1      = 0;
    g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.fltState = 0.0F;

    /* Clear BEMF and Tracking observers state variables */
    AMCLIB_PMSMBemfObsrvDQInit_A32fff(&g_sM1Drive.sFocPMSM.sBemfObsrv);
    AMCLIB_TrackObsrvInit_A32af(ACC32(0.0), &g_sM1Drive.sFocPMSM.sTo);
}

/*!
 * @brief Fault detention routine - check various faults
 *
 * @param void  No input parameter
 *
 * @return None
 */
RAM_FUNC_LIB
static void M1_FaultDetection(void)
{
    /* Clearing actual faults before detecting them again  */
    /* Clear all pending faults */
    FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdPending);

    /* Clear fault captured manually if required. */
    if (g_sM1Drive.bFaultClearMan)
    {
        /* Clear fault captured */
        FAULT_CLEAR_ALL(g_sM1Drive.sFaultIdCaptured);
        g_sM1Drive.bFaultClearMan = FALSE;
    }

    /* Fault:   DC-bus over-current */
    if (M1_MCDRV_PWM3PH_FLT_GET(&g_sM1Pwm3ph))
    {
    	FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_I_DCBUS_OVER);
    }

    /* Fault:   DC-bus over-voltage */
    if (g_sM1Drive.sFocPMSM.fltUDcBusFilt > g_sM1Drive.sFaultThresholds.fltUDcBusOver)
    {
    	FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_U_DCBUS_OVER);
    }

    /* Fault:   DC-bus under-voltage */
    if (g_sM1Drive.sFocPMSM.fltUDcBusFilt < g_sM1Drive.sFaultThresholds.fltUDcBusUnder)
    {
    	FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_U_DCBUS_UNDER);
    }

    /* Check only in SPEED_FOC control, RUN state, kRunState_Spin and kRunState_FreeWheel sub-states */
    if ((g_sM1Drive.eControl == kControlMode_SpeedFOC) && (g_sM1Ctrl.eState == kSM_AppRun) &&
        (g_eM1StateRun == kRunState_Spin || g_eM1StateRun == kRunState_Freewheel))
    {
        /* Fault: Overload  */
        float_t fltSpeedFiltAbs = MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedFilt);
        float_t fltSpeedRampAbs = MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedRamp);

        if ((fltSpeedFiltAbs < g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
            (fltSpeedRampAbs > g_sM1Drive.sFaultThresholds.fltSpeedMin) &&
            (g_sM1Drive.sSpeed.bSpeedPiStopInteg == TRUE))
        {
        	FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_LOAD_OVER);
        }

        /* Fault: Over-speed  */
        if ((MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedFilt) > g_sM1Drive.sFaultThresholds.fltSpeedOver) &&
            (MLIB_Abs_FLT(g_sM1Drive.sSpeed.fltSpeedCmd) > g_sM1Drive.sFaultThresholds.fltSpeedMin))
        {
        	FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_SPEED_OVER);
        }

        /* Fault: Blocked rotor detection */
        if(g_sM1Drive.sMCATctrl.ui16PospeSensor == 0U)
        {
            /* Filter of bemf Uq voltage */
            g_sM1Drive.fltBemfUqAvg =
                MLIB_Abs_FLT(GDFLIB_FilterMA_FLT(g_sM1Drive.sFocPMSM.sBemfObsrv.sEObsrv.fltQ, &g_sM1Drive.msM1BlockedRotorUqFilt));

            /* Check the bemf Uq voltage threshold only in kRunState_Spin - RUN state */
            if ((g_sM1Drive.fltBemfUqAvg < g_sM1Drive.sFaultThresholds.fltUqBemf) && (g_eM1StateRun == kRunState_Spin))
            {
                    g_sM1Drive.ui16BlockRotorCnt++;
            }
            else
            {
                    g_sM1Drive.ui16BlockRotorCnt = 0U;
            }
        }

        /* For bemf voltage detected above limit longer than defined period number set blocked rotor fault*/
        if (g_sM1Drive.ui16BlockRotorCnt > g_sM1Drive.sFaultThresholds.ui16BlockedPerNum)
        {
            FAULT_SET(g_sM1Drive.sFaultIdPending, FAULT_ROTOR_BLOCKED);
            g_sM1Drive.ui16BlockRotorCnt = 0U;
        }
    }

    /* Mask pending faults by enable structure */
    g_sM1Drive.sFaultIdPending &= g_sM1Drive.sFaultIdEnable;

    /* Pass fault to Fault ID Captured */
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
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
RAM_FUNC_LIB
uint16_t M1_GetAppState(void)
{
    return ((uint16_t)g_sM1Ctrl.eState);
}

/*!
 * @brief Set spin speed of the motor in float value
 *
 * @param fltSpeedCmd  Speed command - set speed
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_SetSpeed(float_t fltSpeedCmd)
{
    if (g_bM1SwitchAppOnOff)
    {
        /* Set speed */
        if (MLIB_Abs_FLT(fltSpeedCmd / g_fltM1speedAngularScale) < g_sM1Drive.sStartUp.fltSpeedCatchUp)
        {
            g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;
        }
        else if (MLIB_Abs_FLT(fltSpeedCmd / g_fltM1speedAngularScale) > M1_N_NOM)
        {
            g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;
        }
        else
        {
            g_sM1Drive.sSpeed.fltSpeedCmd = fltSpeedCmd / g_fltM1speedAngularScale;
        }
    }
    else
    {
        /* Set zero speed */
        g_sM1Drive.sSpeed.fltSpeedCmd = 0.0F;
    }
}

/*!
 * @brief Set position of the motor
 *
 * @param a32PositionCmdDemo  Position command - set position
 *
 * @return None
 */
RAM_FUNC_LIB
void M1_SetPosition(acc32_t a32PositionCmdDemo)
{
    if (g_bM1SwitchAppOnOff)
    {
        /* Set position */
        g_sM1Drive.sPosition.a32PositionCmd = a32PositionCmdDemo;
    }
    else
    {
        /* Set zero position */
        g_sM1Drive.sPosition.a32PositionCmd = ACC32(0.0);
    }
}

/*!
 * @brief Get spin speed of the motor in float value
 *
 * @param void  No input parameter
 *
 * @return float_t Float value of the current speed
 */
RAM_FUNC_LIB
float_t M1_GetSpeed(void)
{
    /* Return speed */
    return g_sM1Drive.sSpeed.fltSpeedCmd;
}
