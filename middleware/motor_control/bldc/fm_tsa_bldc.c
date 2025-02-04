/*
* Copyright 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */


#include "freemaster.h"
#include "freemaster_tsa.h"

#include "m1_sm_snsless.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* global control variables */
extern bool_t g_bM1SwitchAppOnOff;
extern bool_t bDemoMode;

/* global used misc variables */
extern uint32_t g_ui32NumberOfCycles;
extern uint32_t g_ui32MaxNumberOfCycles;

/* Application and board ID  */
extern app_ver_t g_sAppIdFM;

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief g_sM1Drive table structure
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(gsM1Drive_table)

/* gsM1Drive structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.bFaultClearMan, FMSTR_TSA_UINT16)           /* Fault Clear */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdEnable, FMSTR_TSA_UINT16)           /* M1 Fault Enable */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdCaptured, FMSTR_TSA_UINT16)         /* M1 Captured Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdPending, FMSTR_TSA_UINT16)          /* M1 Pending Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeAlignment, FMSTR_TSA_UINT16)        /* Align Duration */
FMSTR_TSA_RW_VAR(g_sM1Drive.f32UBemfIntegThreshold, FMSTR_TSA_FRAC32)   /* Back-EMF Integration Threshold */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeCalibration, FMSTR_TSA_UINT16)      /* Calib Counter */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui32FreqCmtTimer, FMSTR_TSA_UINT32)         /* Cmt Timer Freq */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeFaultRelease, FMSTR_TSA_UINT16)             /* M1 Fault time count number */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16CounterCmtError, FMSTR_TSA_UINT16)      /* Commutation Error Counter */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FreqCtrlLoop, FMSTR_TSA_UINT16)         /* Ctrl loop freq */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16PeriodFreewheelLong, FMSTR_TSA_UINT16)  /* Freewheel Period Long */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16PeriodFreewheelShort, FMSTR_TSA_UINT16) /* Freewheel Period Short */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16PeriodCmtNextInit, FMSTR_TSA_UINT16)    /* Init Commutation Period */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16PeriodCmtNext, FMSTR_TSA_UINT16)    /* Next Commutation Period */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16PeriodToffInit, FMSTR_TSA_UINT16)       /* Nominal Speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FreqPwm, FMSTR_TSA_UINT16)              /* Pwm Freq */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16CounterStartCmt, FMSTR_TSA_UINT16)      /* Start Commutation Counter */
FMSTR_TSA_RW_VAR(g_sM1Drive.f16StartCmtAcceleration, FMSTR_TSA_FRAC16)  /* Start Commution Acceleration */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16SlowCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Slow Control Loop Frequency */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FastCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Fast Control Loop Frequency */

/* g_sM1Drive.sFaultThresholds definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UDcBusOver, FMSTR_TSA_FRAC16)  /* Fault Threshold U DcBusOver */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UDcBusUnder, FMSTR_TSA_FRAC16) /* Fault Threshold U DcBusUnder */

/* g_sM1Drive.sCtrlBLDC DC-bus current measurement */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32B0, FMSTR_TSA_FRAC32) /* IDC-bus B0 filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32B1, FMSTR_TSA_FRAC32) /* IDC-bus B1 filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusFilter.sFltCoeff.f32A1, FMSTR_TSA_FRAC32) /* IDC-bus A1 filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32B0, FMSTR_TSA_FRAC32) /* UDC-bus B0 filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32B1, FMSTR_TSA_FRAC32) /* UDC-bus B1 filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sUDcBusFilter.sFltCoeff.f32A1, FMSTR_TSA_FRAC32) /* UDC-bus A1 filter */


/* g_sM1Drive.sCtrlBLDC definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedMeasured, FMSTR_TSA_FRAC16)    /* Actual Speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16IDcBusAlign, FMSTR_TSA_FRAC16)      /* Align Current */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16UPhaseBemf, FMSTR_TSA_FRAC16)       /* Back-EMF Voltage */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.ui32PeriodSixCmtSum, FMSTR_TSA_UINT32) /* Commutation Periods Sum */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.i16SectorCmt, FMSTR_TSA_UINT16)        /* Commutation Sector */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16IDcBusPiErr, FMSTR_TSA_FRAC16)      /* Current PI Ctrl Error */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16IDcBusNoFilt, FMSTR_TSA_FRAC16)     /* DC-bus Current (no filtered) */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16IDcBus, FMSTR_TSA_FRAC16)           /* DC-bus Current */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16IDcBusLim, FMSTR_TSA_FRAC16)        /* DC-bus Current Limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16UDcBus, FMSTR_TSA_FRAC16)           /* DC-bus Voltage */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16DutyCycle, FMSTR_TSA_FRAC16)        /* Duty Cycle */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedNominal, FMSTR_TSA_FRAC16)     /* Nominal Speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedCmd, FMSTR_TSA_FRAC16)         /* Required Speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedPiErr, FMSTR_TSA_FRAC16)       /* Speed Error */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedMinimal, FMSTR_TSA_FRAC16)     /* Speed Minimal */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.f16SpeedRamp, FMSTR_TSA_FRAC16)        /* Speed Ramp */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.i16SpeedScaleConst, FMSTR_TSA_UINT16)  /* Speed Scale Constant */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16StartCmtNumber, FMSTR_TSA_UINT16)            /* Start Commutation Number */




/* sCtrlBLDC.sCtrlBLDC.sIDcBusPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.bLimFlag, FMSTR_TSA_FRAC16)         /* Current Limit Flag */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.a32IGain, FMSTR_TSA_FRAC_Q(16, 15)) /* Torque Loop Ki */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.a32PGain, FMSTR_TSA_FRAC_Q(16, 15)) /* Torque Loop Kp */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16UpperLim, FMSTR_TSA_FRAC16)      /* Torque Loop Limit High */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusPiParams.f16LowerLim, FMSTR_TSA_FRAC16)      /* Torque Loop Limit Low */

/* sCtrlBLDC.sSpeedPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedPiParams.a32IGain, FMSTR_TSA_FRAC_Q(16, 15)) /* Speed Loop Ki */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedPiParams.a32PGain, FMSTR_TSA_FRAC_Q(16, 15)) /* Speed Loop Kp */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16UpperLim, FMSTR_TSA_FRAC16)      /* Speed Loop Limit High */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedPiParams.f16LowerLim, FMSTR_TSA_FRAC16)      /* Speed Loop Limit Low */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedPiParams.bLimFlag, FMSTR_TSA_FRAC16)         /* Speed PI Ctrl Limit Flag */

/* sCtrlBLDC.sSpeedRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32RampDown, FMSTR_TSA_FRAC32) /* Speed Ramp Down */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sSpeedRampParams.f32RampUp, FMSTR_TSA_FRAC32)   /* Speed Ramp Up */

/* sCtrlBLDC.sIDcBusMaFilter definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sCtrlBLDC.sIDcBusMaFilter.u16Sh, FMSTR_TSA_UINT16) /* Torque Filter Order */

FMSTR_TSA_TABLE_END()

/*!
 * @brief Global table with global variables used in TSA
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(global_table)

/* global variable read */
FMSTR_TSA_RW_VAR(g_bM1SwitchAppOnOff, FMSTR_TSA_UINT16)     /* Application Switch */
FMSTR_TSA_RW_VAR(g_ui32NumberOfCycles, FMSTR_TSA_UINT32)    /* Cycle Number */
FMSTR_TSA_RW_VAR(g_ui32MaxNumberOfCycles, FMSTR_TSA_UINT32) /* Cycle Number Maximum */
FMSTR_TSA_RW_VAR(bDemoMode, FMSTR_TSA_UINT16)               /* Demo Mode */
FMSTR_TSA_RW_VAR(s_eM1StateRun, FMSTR_TSA_UINT16)           /* State Run */

/* Global control variables */
FMSTR_TSA_RW_VAR(g_sM1Ctrl.eState, FMSTR_TSA_UINT16) /* Application State */

/* float fault states */
FMSTR_TSA_RW_VAR(s_fltM1currentScale, FMSTR_TSA_FLOAT)    /* FMSTR_M1_currentScale */
FMSTR_TSA_RW_VAR(s_fltM1DCBvoltageScale, FMSTR_TSA_FLOAT) /* FMSTR_M1_DCBvoltageScale */
FMSTR_TSA_RW_VAR(s_fltM1speedScale, FMSTR_TSA_FLOAT)      /* FMSTR_M1_speedScale */

/* msM1AdcSensor.ui16OffsetDcCurr */
FMSTR_TSA_RW_VAR(g_sM1AdcSensor.ui16OffsetDcCurr, FMSTR_TSA_FRAC16) /* msM1AdcSensor.ui16OffsetDcCurr */

FMSTR_TSA_TABLE_END()

/*!
 * @brief Structure used in FM to get required ID's
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(sAppIdFM_table)

/* Board ID structure definition */
FMSTR_TSA_RO_MEM(g_sAppIdFM.cBoardID, FMSTR_TSA_UINT8, &g_sAppIdFM.cBoardID[0], 20)
FMSTR_TSA_RO_MEM(g_sAppIdFM.cExampleID, FMSTR_TSA_UINT8, &g_sAppIdFM.cExampleID[0], 30)
FMSTR_TSA_RO_MEM(g_sAppIdFM.cAppVer, FMSTR_TSA_UINT8, &g_sAppIdFM.cAppVer[0], 5)

FMSTR_TSA_TABLE_END()

/*!
 * @brief TSA Table list required if TSA macro is enabled
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(gsM1Drive_table)
FMSTR_TSA_TABLE(global_table)
FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE_LIST_END()
