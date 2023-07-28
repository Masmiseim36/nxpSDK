/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fm_tsa_pmsm.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/

/* Control FOC and MID */
extern mid_pmsm_t g_sMidDrive;
extern ctrl_m1_mid_t g_sSpinMidSwitch;

/* MID 2.0 control */
extern mid_app_cmd_t g_eMidCmd;

/* AP identification */
extern MCAA_ESTIMRL_T_FLT g_sEstimRLStruct;
extern MCAA_ESTIMRL_RUN_T_FLT g_sEstimRLCtrlRun;
extern rl_estim_cfg_params_t g_sEstimRLInitFMSTR;
extern float_t	fltIDcPlot;
extern float_t	fltLdPlot;
extern float_t	fltLqPlot;
extern uint8_t u8ModeEstimRL;

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

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_RW_VAR(g_sM1Drive.fltSpeedEnc, FMSTR_TSA_FLOAT) /* M1 Speed from encoder */
#endif

/* gsM1Drive structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdEnable, FMSTR_TSA_UINT16)       /* M1 Fault Enable */
FMSTR_TSA_RW_VAR(g_sM1Drive.bFaultClearMan, FMSTR_TSA_UINT16)       /* M1 Fault Clear */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdCaptured, FMSTR_TSA_UINT16)     /* M1 Captured Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdPending, FMSTR_TSA_UINT16)      /* M1 Pending Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.eControl, FMSTR_TSA_UINT16)             /* M1 MCAT Control */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16SlowCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Slow Control Loop Frequency */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FastCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Fast Control Loop Frequency */

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_RW_VAR(g_sM1Drive.ui32CpuFrequency, FMSTR_TSA_UINT32) /* M1 Speed from encoder */
#endif

/* gsM1Drive.sOpenloop structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.fltFreqReq, FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.sUDQReq.fltQ, FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.sUDQReq.fltD, FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.sIDQReq.fltQ, FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.sIDQReq.fltD, FMSTR_TSA_FLOAT)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.bCurrentControl, FMSTR_TSA_UINT16)
FMSTR_TSA_RW_VAR(g_sM1Drive.sOpenloop.f16Theta, FMSTR_TSA_FRAC16)


/* gsM1Drive.sSpeed structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.fltSpeedFilt, FMSTR_TSA_FLOAT) /* M1 Speed filtered */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.fltSpeed, FMSTR_TSA_FLOAT)     /* M1 Speed Estimated */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.fltSpeedRamp, FMSTR_TSA_FLOAT) /* M1 Speed Ramp */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.fltSpeedCmd, FMSTR_TSA_FLOAT)  /* M1 Speed Required */

/* sSpeed.sSpeedFilter.sSpeedFilter definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M1 Speed Filter A1 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M1 Speed Filter B0 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M1 Speed Filter B1 */

/* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedRampParams.fltRampDown, FMSTR_TSA_FLOAT) /* M1 Speed Ramp Down */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedRampParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M1 Speed Ramp Up */

/* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.fltIGain, FMSTR_TSA_FLOAT)    /* M1 Speed Loop Ki Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.fltPGain, FMSTR_TSA_FLOAT)    /* M1 Speed Loop Kp Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.fltUpperLim, FMSTR_TSA_FLOAT) /* M1 Speed Loop Limit High */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.fltLowerLim, FMSTR_TSA_FLOAT) /* M1 Speed Loop Limit Low */

#ifdef PMSM_SNSLESS_ENC
/* gsM1Drive.sPosition structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sPosition.f16PositionPGain, FMSTR_TSA_FRAC16) /* M1 Position P conroller P Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sPosition.a32Position, FMSTR_TSA_FRAC32)      /* M1 Position Actual */
FMSTR_TSA_RW_VAR(g_sM1Drive.sPosition.a32PositionError, FMSTR_TSA_FRAC32) /* M1 Position Error */
FMSTR_TSA_RW_VAR(g_sM1Drive.sPosition.a32PositionCmd, FMSTR_TSA_FRAC32)   /* M1 Position Required */
#endif

/* sSpeed.sAlignment definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sAlignment.ui16Time, FMSTR_TSA_UINT16) /* M1 Alignment Duration */
FMSTR_TSA_RW_VAR(g_sM1Drive.sAlignment.fltUdReq, FMSTR_TSA_FLOAT)  /* M1 Alignment Voltage */

/* gsM1Drive.sFocPMSM structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltDutyCycleLimit, FMSTR_TSA_FLOAT) /* M1 Current Loop Limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltUDcBus, FMSTR_TSA_FLOAT)         /* M1 DCB Voltage */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltUDcBusFilt, FMSTR_TSA_FLOAT)     /* M1 DCB Voltage Filtered */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosElExt, FMSTR_TSA_UINT16)      /* M1 Position External */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosEl, FMSTR_TSA_UINT16)         /* M1 Position Electrical */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosElEst, FMSTR_TSA_UINT16)      /* M1 Position Estimated */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.ui16SectorSVM, FMSTR_TSA_UINT16)    /* M1 SVM Sector */

/* sFocPMSM.sIAlBe definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIAlBe.fltAlpha, FMSTR_TSA_FLOAT) /* M1 I alpha */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIAlBe.fltBeta, FMSTR_TSA_FLOAT)  /* M1 I beta */

/* sFocPMSM.sIDQ definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQ.fltD, FMSTR_TSA_FLOAT) /* M1 Id */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQ.fltQ, FMSTR_TSA_FLOAT) /* M1 Iq */

/* sFocPMSM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQReq.fltD, FMSTR_TSA_FLOAT) /* M1 Id req */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQReq.fltQ, FMSTR_TSA_FLOAT) /* M1 Iq req */

/* sFocPMSM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDQReq.fltD, FMSTR_TSA_FLOAT) /* M1 Ud req */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDQReq.fltQ, FMSTR_TSA_FLOAT) /* M1 Uq req */

/* sFocPMSM.sIdPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIdPiParams.fltIGain, FMSTR_TSA_FLOAT) /* M1 Id Ki Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIdPiParams.fltPGain, FMSTR_TSA_FLOAT) /* M1 Id Kp Gain */

/* sFocPMSM.sBemfObsrv definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.fltEGain, FMSTR_TSA_FLOAT)       /* M1 Obsrv E gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.fltIGain, FMSTR_TSA_FLOAT)       /* M1 Obsrv I gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain, FMSTR_TSA_FLOAT) /* M1 Obsrv Ki gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain, FMSTR_TSA_FLOAT) /* M1 Obsrv Kp gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.fltUGain, FMSTR_TSA_FLOAT)       /* M1 Obsrv U gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.fltWIGain, FMSTR_TSA_FLOAT)      /* M1 Obsrv WI gain */

/* sFocPMSM.sTo definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.fltIGain, FMSTR_TSA_FLOAT)  /* M1 Obsrv To Ki gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.fltPGain, FMSTR_TSA_FLOAT)  /* M1 Obsrv To Kp gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.fltThGain, FMSTR_TSA_FLOAT) /* M1 Obsrv To Theta gain */

/* sFocPMSM.sIqPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIqPiParams.fltIGain, FMSTR_TSA_FLOAT) /* M1 Iq Ki Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIqPiParams.fltPGain, FMSTR_TSA_FLOAT) /* M1 Iq Kp Gain */

/* sFocPMSM.sIABC definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.fltA, FMSTR_TSA_FLOAT) /* M1 Phase Current A */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.fltB, FMSTR_TSA_FLOAT) /* M1 Phase Current B */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.fltC, FMSTR_TSA_FLOAT) /* M1 Phase Current C */

/* sFocPMSM.fltSpeedElEst definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltSpeedElEst, FMSTR_TSA_FLOAT) /* M1 Speed Estimated */

/* g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M1 DcBus IIR B0 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M1 DcBus IIR B1 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M1 DcBus IIR A1 */

/* g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M1 Obsrv To IIR B0 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M1 Obsrv To IIR B1 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M1 Obsrv To IIR A1 */

/* g_sM1Drive.sFocPMSM.sDutyABC */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sDutyABC.f16A, FMSTR_TSA_FRAC16)         /* M1 Duty Cycle A */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sDutyABC.f16B, FMSTR_TSA_FRAC16)         /* M1 Duty Cycle B */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sDutyABC.f16C, FMSTR_TSA_FRAC16)         /* M1 Duty Cycle C */

/* sFaultThresholds definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUqBemf, FMSTR_TSA_FLOAT)      /* M1 Fault Threshold BemfBlocked */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusOver, FMSTR_TSA_FLOAT)  /* M1 Fault Threshold DcBusOver */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusTrip, FMSTR_TSA_FLOAT)  /* M1 Fault Threshold DcBusTrip */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusUnder, FMSTR_TSA_FLOAT) /* M1 Fault Threshold DcBusUnder */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedMin, FMSTR_TSA_FLOAT)    /* M1 Fault Threshold SpeedMin */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedNom, FMSTR_TSA_FLOAT)    /* M1 Fault Threshold SpeedNom */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedOver, FMSTR_TSA_FLOAT)   /* M1 Fault Threshold SpeedOver */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.ui16BlockedPerNum, FMSTR_TSA_UINT16)   /* M1 Fault Threshold Blocked rotor period */

/* sStartUp definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16CoeffMerging, FMSTR_TSA_FRAC16)     /* M1 Merging Coefficient */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16RatioMerging, FMSTR_TSA_FRAC16)     /* M1 Merging Ratio */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M1 Merging Speed Catch Up */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16PosGen, FMSTR_TSA_FRAC16)           /* M1 Position Open Loop */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M1 Speed Merging Catch Up  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedRampOpenLoop, FMSTR_TSA_FLOAT) /* M1 Speed Ramp Open Loop  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltCurrentStartup, FMSTR_TSA_FLOAT)    /* M1 Startup Current  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.sSpeedIntegrator.a32Gain, FMSTR_TSA_FRAC32)      /* M1 StartUp Integ Ki */

/* sStartUp.sSpeedRampOpenLoopParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampDown, FMSTR_TSA_FLOAT) /* M1 Startup Ramp Dec */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M1 Startup Ramp Inc */

/* sScalarCtrl definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16PosElScalar, FMSTR_TSA_FRAC16) /* M1 Position Electrical Scalar */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltFreqRamp, FMSTR_TSA_FLOAT)     /* M1 Scalar Frequency Ramp */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltFreqCmd, FMSTR_TSA_FLOAT)      /* M1 Scalar speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltVHzGain, FMSTR_TSA_FLOAT)      /* M1 VHz Factor Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltUqMin, FMSTR_TSA_FLOAT)        /* M1 VHz Factor Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sUDQReq.fltQ, FMSTR_TSA_FLOAT)    /* M1 VHz Factor Gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltFreqMax, FMSTR_TSA_FLOAT)      /* M1 FMSTR_M1_frequencyScale */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sFreqIntegrator.a32Gain, FMSTR_TSA_FRAC32)      /* M1 Scalar Integ Ki */

/* sScalarCtrl.sFreqRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sFreqRampParams.fltRampDown, FMSTR_TSA_FLOAT) /* M1 Scalar Ramp Down */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sFreqRampParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M1 Scalar Ramp Up */

/* sMCATctrl definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.ui16PospeSensor, FMSTR_TSA_UINT16) /* M1 MCAT POSPE Sensor */

/* sMCATctrl.sIDQReqMCAT definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltD, FMSTR_TSA_FLOAT) /* M1 MCAT Id Required */
FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sIDQReqMCAT.fltQ, FMSTR_TSA_FLOAT) /* M1 MCAT Iq Required */

/* sMCATctrl.sUDQReqMCAT definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltD, FMSTR_TSA_FLOAT) /* M1 MCAT Ud Required */
FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sUDQReqMCAT.fltQ, FMSTR_TSA_FLOAT) /* M1 MCAT Uq Required */

FMSTR_TSA_TABLE_END()

#ifdef DUAL_MOTOR
/*!
 * @brief g_sM2Drive table structure
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(gsM2Drive_table)

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_RW_VAR(g_sM2Drive.fltSpeedEnc, FMSTR_TSA_FLOAT) /* M2 Speed from encoder */
#endif

/* gsM2Drive structure definition */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultIdEnable, FMSTR_TSA_UINT16)       /* M2 Fault Enable */
FMSTR_TSA_RW_VAR(g_sM2Drive.bFaultClearMan, FMSTR_TSA_UINT16)       /* M2 Fault Clear */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultIdCaptured, FMSTR_TSA_UINT16)     /* M2 Captured Fault */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultIdPending, FMSTR_TSA_UINT16)      /* M2 Pending Fault */
FMSTR_TSA_RW_VAR(g_sM2Drive.eControl, FMSTR_TSA_UINT16)             /* M2 MCAT Control */
FMSTR_TSA_RW_VAR(g_sM2Drive.ui16SlowCtrlLoopFreq, FMSTR_TSA_UINT16) /* M2 Slow Control Loop Frequency */
FMSTR_TSA_RW_VAR(g_sM2Drive.ui16FastCtrlLoopFreq, FMSTR_TSA_UINT16) /* M2 Fast Control Loop Frequency */

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_RW_VAR(g_sM2Drive.ui32CpuFrequency, FMSTR_TSA_UINT32)     /* M2 Speed from encoder */
#endif

/* gsM2Drive.sSpeed structure definition */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.fltSpeedFilt, FMSTR_TSA_FLOAT) /* M2 Speed filtered */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.fltSpeed, FMSTR_TSA_FLOAT)     /* M2 Speed Estimated */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.fltSpeedRamp, FMSTR_TSA_FLOAT) /* M2 Speed Ramp */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.fltSpeedCmd, FMSTR_TSA_FLOAT)  /* M2 Speed Required */

/* sSpeed.sSpeedFilter.sSpeedFilter definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedFilter.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M2 Speed Filter A1 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M2 Speed Filter B0 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedFilter.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M2 Speed Filter B1 */

/* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedRampParams.fltRampDown, FMSTR_TSA_FLOAT) /* M2 Speed Ramp Down */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedRampParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M2 Speed Ramp Up */

/* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedPiParams.fltIGain, FMSTR_TSA_FLOAT)    /* M2 Speed Loop Ki Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedPiParams.fltPGain, FMSTR_TSA_FLOAT)    /* M2 Speed Loop Kp Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedPiParams.fltUpperLim, FMSTR_TSA_FLOAT) /* M2 Speed Loop Limit High */
FMSTR_TSA_RW_VAR(g_sM2Drive.sSpeed.sSpeedPiParams.fltLowerLim, FMSTR_TSA_FLOAT) /* M2 Speed Loop Limit Low */

#ifdef PMSM_SNSLESS_ENC
/* gsM2Drive.sPosition structure definition */
FMSTR_TSA_RW_VAR(g_sM2Drive.sPosition.f16PositionPGain, FMSTR_TSA_FRAC16) /* M2 Position P conroller P Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sPosition.a32Position, FMSTR_TSA_FRAC32)      /* M2 Position Actual */
FMSTR_TSA_RW_VAR(g_sM2Drive.sPosition.a32PositionError, FMSTR_TSA_FRAC32) /* M2 Position Error */
FMSTR_TSA_RW_VAR(g_sM2Drive.sPosition.a32PositionCmd, FMSTR_TSA_FRAC32)   /* M2 Position Required */
#endif

/* sSpeed.sAlignment definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sAlignment.ui16Time, FMSTR_TSA_UINT16) /* M2 Alignment Duration */
FMSTR_TSA_RW_VAR(g_sM2Drive.sAlignment.fltUdReq, FMSTR_TSA_FLOAT)  /* M2 Alignment Voltage */

/* gsM1Drive.sFocPMSM structure definition */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.fltDutyCycleLimit, FMSTR_TSA_FLOAT) /* M2 Current Loop Limit */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.fltUDcBus, FMSTR_TSA_FLOAT)         /* M2 DCB Voltage */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.fltUDcBusFilt, FMSTR_TSA_FLOAT)     /* M2 DCB Voltage Filtered */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.f16PosElExt, FMSTR_TSA_UINT16)      /* M2 Position External */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.f16PosEl, FMSTR_TSA_UINT16)         /* M2 Position Electrical */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.f16PosElEst, FMSTR_TSA_UINT16)      /* M2 Position Estimated */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.ui16SectorSVM, FMSTR_TSA_UINT16)    /* M2 SVM Sector */

/* sFocPMSM.sIAlBe definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIAlBe.fltAlpha, FMSTR_TSA_FLOAT) /* M2 I alpha */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIAlBe.fltBeta, FMSTR_TSA_FLOAT)  /* M2 I beta */

/* sFocPMSM.sIDQ definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIDQ.fltD, FMSTR_TSA_FLOAT) /* M2 Id */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIDQ.fltQ, FMSTR_TSA_FLOAT) /* M2 Iq */

/* sFocPMSM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIDQReq.fltD, FMSTR_TSA_FLOAT) /* M2 Id req */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIDQReq.fltQ, FMSTR_TSA_FLOAT) /* M2 Iq req */

/* sFocPMSM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sUDQReq.fltD, FMSTR_TSA_FLOAT) /* M2 Ud req */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sUDQReq.fltQ, FMSTR_TSA_FLOAT) /* M2 Uq req */

/* sFocPMSM.sIdPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIdPiParams.fltIGain, FMSTR_TSA_FLOAT) /* M2 Id Ki Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIdPiParams.fltPGain, FMSTR_TSA_FLOAT) /* M2 Id Kp Gain */

/* sFocPMSM.sBemfObsrv definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.fltEGain, FMSTR_TSA_FLOAT)       /* M2 Obsrv E gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.fltIGain, FMSTR_TSA_FLOAT)       /* M2 Obsrv I gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain, FMSTR_TSA_FLOAT) /* M2 Obsrv Ki gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain, FMSTR_TSA_FLOAT) /* M2 Obsrv Kp gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.fltUGain, FMSTR_TSA_FLOAT)       /* M2 Obsrv U gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sBemfObsrv.fltWIGain, FMSTR_TSA_FLOAT)      /* M2 Obsrv WI gain */

/* sFocPMSM.sTo definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sTo.fltIGain, FMSTR_TSA_FLOAT)  /* M2 Obsrv To Ki gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sTo.fltPGain, FMSTR_TSA_FLOAT)  /* M2 Obsrv To Kp gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sTo.fltThGain, FMSTR_TSA_FLOAT) /* M2 Obsrv To Theta gain */

/* sFocPMSM.sIqPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIqPiParams.fltIGain, FMSTR_TSA_FLOAT) /* M2 Iq Ki Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIqPiParams.fltPGain, FMSTR_TSA_FLOAT) /* M2 Iq Kp Gain */

/* sFocPMSM.sIABC definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIABC.fltA, FMSTR_TSA_FLOAT) /* M2 Phase Current A */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIABC.fltB, FMSTR_TSA_FLOAT) /* M2 Phase Current B */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sIABC.fltC, FMSTR_TSA_FLOAT) /* M2 Phase Current C */

/* sFocPMSM.fltSpeedElEst definition */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.fltSpeedElEst, FMSTR_TSA_FLOAT) /* M2 Speed Estimated */

/* g_sM2Drive.sFocPMSM.sUDcBusFilter.sFltCoeff */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M2 DcBus IIR B0 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M2 DcBus IIR B1 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sUDcBusFilter.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M2 DcBus IIR A1 */

/* g_sM2Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB0, FMSTR_TSA_FLOAT) /* M2 Obsrv To IIR B0 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltB1, FMSTR_TSA_FLOAT) /* M2 Obsrv To IIR B1 */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sSpeedElEstFilt.sFltCoeff.fltA1, FMSTR_TSA_FLOAT) /* M2 Obsrv To IIR A1 */

/* g_sM2Drive.sFocPMSM.sDutyABC */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sDutyABC.f16A, FMSTR_TSA_FRAC16)         /* M2 Duty Cycle A */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sDutyABC.f16B, FMSTR_TSA_FRAC16)         /* M2 Duty Cycle B */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFocPMSM.sDutyABC.f16C, FMSTR_TSA_FRAC16)         /* M2 Duty Cycle C */

/* sFaultThresholds definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltUqBemf, FMSTR_TSA_FLOAT)      /* M2 Fault Threshold BemfBlocked */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltUDcBusOver, FMSTR_TSA_FLOAT)  /* M2 Fault Threshold DcBusOver */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltUDcBusTrip, FMSTR_TSA_FLOAT)  /* M2 Fault Threshold DcBusTrip */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltUDcBusUnder, FMSTR_TSA_FLOAT) /* M2 Fault Threshold DcBusUnder */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltSpeedMin, FMSTR_TSA_FLOAT)    /* M2 Fault Threshold SpeedMin */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltSpeedNom, FMSTR_TSA_FLOAT)    /* M2 Fault Threshold SpeedNom */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.fltSpeedOver, FMSTR_TSA_FLOAT)   /* M2 Fault Threshold SpeedOver */
FMSTR_TSA_RW_VAR(g_sM2Drive.sFaultThresholds.ui16BlockedPerNum, FMSTR_TSA_UINT16)   /* M2 Fault Threshold Blocked rotor period */

/* sStartUp definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.f16CoeffMerging, FMSTR_TSA_FRAC16)     /* M2 Merging Coefficient */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.f16RatioMerging, FMSTR_TSA_FRAC16)     /* M2 Merging Ratio */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M2 Merging Speed Catch Up */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.f16PosGen, FMSTR_TSA_FRAC16)           /* M2 Position Open Loop */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M2 Speed Merging Catch Up  */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.fltSpeedRampOpenLoop, FMSTR_TSA_FLOAT) /* M2 Speed Ramp Open Loop  */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.fltCurrentStartup, FMSTR_TSA_FLOAT)    /* M2 Startup Current  */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.sSpeedIntegrator.a32Gain, FMSTR_TSA_FRAC32)      /* M2 StartUp Integ Ki */

/* sStartUp.sSpeedRampOpenLoopParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampDown, FMSTR_TSA_FLOAT) /* M2 Startup Ramp Dec */
FMSTR_TSA_RW_VAR(g_sM2Drive.sStartUp.sSpeedRampOpenLoopParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M2 Startup Ramp Inc */

/* sScalarCtrl definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.f16PosElScalar, FMSTR_TSA_FRAC16) /* M2 Position Electrical Scalar */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.fltFreqRamp, FMSTR_TSA_FLOAT)     /* M2 Scalar Frequency Ramp */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.fltFreqCmd, FMSTR_TSA_FLOAT)      /* M2 Scalar speed */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.fltVHzGain, FMSTR_TSA_FLOAT)      /* M2 VHz Factor Gain */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.fltUqMin, FMSTR_TSA_FLOAT)        /* M2 Uq minimal */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.sUDQReq.fltQ, FMSTR_TSA_FLOAT)    /* M2 Required voltage in q-axis */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.fltFreqMax, FMSTR_TSA_FLOAT)      /* M2 FMSTR_M2_frequencyScale */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.sFreqIntegrator.a32Gain, FMSTR_TSA_FRAC32)      /* M2 Scalar Integ Ki */

/* sScalarCtrl.sFreqRampParams definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.sFreqRampParams.fltRampDown, FMSTR_TSA_FLOAT) /* M2 Scalar Ramp Down */
FMSTR_TSA_RW_VAR(g_sM2Drive.sScalarCtrl.sFreqRampParams.fltRampUp, FMSTR_TSA_FLOAT)   /* M2 Scalar Ramp Up */
//
/* sMCATctrl definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sMCATctrl.ui16PospeSensor, FMSTR_TSA_UINT16) /* M2 MCAT POSPE Sensor */

/* sMCATctrl.sIDQReqMCAT definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sMCATctrl.sIDQReqMCAT.fltD, FMSTR_TSA_FLOAT) /* M2 MCAT Id Required */
FMSTR_TSA_RW_VAR(g_sM2Drive.sMCATctrl.sIDQReqMCAT.fltQ, FMSTR_TSA_FLOAT) /* M2 MCAT Iq Required */

/* sMCATctrl.sUDQReqMCAT definitions */
FMSTR_TSA_RW_VAR(g_sM2Drive.sMCATctrl.sUDQReqMCAT.fltD, FMSTR_TSA_FLOAT) /* M2 MCAT Ud Required */
FMSTR_TSA_RW_VAR(g_sM2Drive.sMCATctrl.sUDQReqMCAT.fltQ, FMSTR_TSA_FLOAT) /* M2 MCAT Uq Required */

FMSTR_TSA_TABLE_END()

#endif

/*!
 * @brief g_sM1Enc driver structure
 *
 * @param None
 *
 * @return None
 */
#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_TABLE_BEGIN(gsM1Enc_table)

/* gsM1Enc structure definition */
FMSTR_TSA_RW_VAR(g_sM1Enc.fltSpdMeEst, FMSTR_TSA_FLOAT)   /* M1 Measured Mechanical Speed */
FMSTR_TSA_RW_VAR(g_sM1Enc.f16PosMe, FMSTR_TSA_FRAC16)     /* M1 Meassured Mechanical Position */
FMSTR_TSA_RW_VAR(g_sM1Enc.f16PosMeEst, FMSTR_TSA_FRAC16)  /* M1 Position Encoder Mechanical */
FMSTR_TSA_RW_VAR(g_sM1Enc.sTo.fltThGain, FMSTR_TSA_FLOAT) /* M1 POSPE Integ Gain */
FMSTR_TSA_RW_VAR(g_sM1Enc.sTo.fltIGain, FMSTR_TSA_FLOAT)  /* M1 POSPE Ki Gain */
FMSTR_TSA_RW_VAR(g_sM1Enc.sTo.fltPGain, FMSTR_TSA_FLOAT)  /* M1 POSPE Kp Gain */
FMSTR_TSA_RW_VAR(g_sM1Enc.bDirection, FMSTR_TSA_UINT16)   /* M1 Encoder direction */
FMSTR_TSA_RW_VAR(g_sM1Enc.fltSpdEncMin, FMSTR_TSA_FLOAT)  /* M1 Encoder minimal speed */
FMSTR_TSA_RW_VAR(g_sM1Enc.ui16Pp, FMSTR_TSA_UINT16)       /* M1 Pole pairs */
FMSTR_TSA_RW_VAR(g_sM1Enc.ui16PulseNumber, FMSTR_TSA_UINT16)    /* M1 Encoder pulses */
FMSTR_TSA_RW_VAR(g_sM1Enc.a32PosMeGain, FMSTR_TSA_FRAC32)       /* M1 Position mechanical gain */

FMSTR_TSA_TABLE_END()
#endif

#ifdef DUAL_MOTOR

/*!
 * @brief g_sM2Enc driver structure
 *
 * @param None
 *
 * @return None
 */
#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_TABLE_BEGIN(gsM2Enc_table)

/* gsM2Enc structure definition */
FMSTR_TSA_RW_VAR(g_sM2Enc.fltSpdMeEst, FMSTR_TSA_FLOAT)   /* M2 Measured Mechanical Speed */
FMSTR_TSA_RW_VAR(g_sM2Enc.f16PosMe, FMSTR_TSA_FRAC16)     /* M2 Meassured Mechanical Position */
FMSTR_TSA_RW_VAR(g_sM2Enc.f16PosMeEst, FMSTR_TSA_FRAC16)  /* M2 Position Encoder Mechanical */
FMSTR_TSA_RW_VAR(g_sM2Enc.sTo.fltThGain, FMSTR_TSA_FLOAT) /* M2 POSPE Integ Gain */
FMSTR_TSA_RW_VAR(g_sM2Enc.sTo.fltIGain, FMSTR_TSA_FLOAT)  /* M2 POSPE Ki Gain */
FMSTR_TSA_RW_VAR(g_sM2Enc.sTo.fltPGain, FMSTR_TSA_FLOAT)  /* M2 POSPE Kp Gain */
FMSTR_TSA_RW_VAR(g_sM2Enc.bDirection, FMSTR_TSA_UINT16)   /* M2 Encoder direction */
FMSTR_TSA_RW_VAR(g_sM2Enc.fltSpdEncMin, FMSTR_TSA_FLOAT)  /* M2 Encoder minimal speed */
FMSTR_TSA_RW_VAR(g_sM2Enc.ui16Pp, FMSTR_TSA_UINT16)       /* M2 Pole pairs */
FMSTR_TSA_RW_VAR(g_sM2Enc.ui16PulseNumber, FMSTR_TSA_UINT16)    /* M2 Encoder pulses */
FMSTR_TSA_RW_VAR(g_sM2Enc.a32PosMeGain, FMSTR_TSA_FRAC32)       /* M2 Position mechanical gain */

FMSTR_TSA_TABLE_END()
#endif

#endif

/*!
 * @brief Global table with global variables used in TSA
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(global_table)

/* global variables & control */
FMSTR_TSA_RW_VAR(g_ui32NumberOfCycles, FMSTR_TSA_UINT32)    /* Cycle Number */
FMSTR_TSA_RW_VAR(g_ui32MaxNumberOfCycles, FMSTR_TSA_UINT32) /* Cycle Number Maximum */
FMSTR_TSA_RW_VAR(bDemoModeSpeed, FMSTR_TSA_UINT16)          /* Demo Mode Speed */

#ifdef PMSM_SNSLESS_ENC
#ifdef DAPENG_TEST
FMSTR_TSA_RW_VAR(ui32FastIsrCount, FMSTR_TSA_UINT32)  /* Fast loop ISR counter */
FMSTR_TSA_RW_VAR(ui32SlowIsrCount, FMSTR_TSA_UINT32)  /* Slow loop ISR counter */
#endif
FMSTR_TSA_RW_VAR(bDemoModePosition, FMSTR_TSA_UINT16) /* Demo Mode Position */
#endif

#ifdef DUAL_MOTOR
FMSTR_TSA_RW_VAR(g_ui32M2NumberOfCycles, FMSTR_TSA_UINT32)    /* M2 Cycle Number */
FMSTR_TSA_RW_VAR(g_ui32M2MaxNumberOfCycles, FMSTR_TSA_UINT32) /* M2 Cycle Number Maximum */
FMSTR_TSA_RW_VAR(bM2DemoModeSpeed, FMSTR_TSA_UINT16)          /* M2 Demo Mode Speed */

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_RW_VAR(bM2DemoModePosition, FMSTR_TSA_UINT16) /* M2 Demo Mode Position */
#endif
#endif

FMSTR_TSA_TABLE_END()

/*!
 * @brief M1 Global table with global variables used in TSA
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(M1_global_table)

/* global variables & control */
FMSTR_TSA_RW_VAR(g_bM1SwitchAppOnOff, FMSTR_TSA_UINT16) /* M1 Application Switch */
FMSTR_TSA_RW_VAR(g_sM1Ctrl.eState, FMSTR_TSA_UINT16)    /* M1 Application State */

/* global freemaster float variables */
FMSTR_TSA_RW_VAR(g_fltM1currentScale, FMSTR_TSA_FLOAT)         /* FMSTR_M1_currentScale */
FMSTR_TSA_RW_VAR(g_fltM1DCBvoltageScale, FMSTR_TSA_FLOAT)      /* FMSTR_M1_DCBvoltageScale */
FMSTR_TSA_RW_VAR(g_fltM1speedScale, FMSTR_TSA_FLOAT)           /* FMSTR_M1_speedScale */
FMSTR_TSA_RW_VAR(g_fltM1voltageScale, FMSTR_TSA_FLOAT)         /* FMSTR_M1_voltageScale */
FMSTR_TSA_RW_VAR(g_fltM1speedAngularScale, FMSTR_TSA_FLOAT)    /* FMSTR_M1_speedAngularScale */
FMSTR_TSA_RW_VAR(g_fltM1speedMechanicalScale, FMSTR_TSA_FLOAT) /* FMSTR_M1_speedMechanicalScale */

FMSTR_TSA_RW_VAR(g_eM1StateRun, FMSTR_TSA_UINT16) /* M1 State Run */

FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeFullSpeedFreeWheel, FMSTR_TSA_UINT16)       /* M1 Free-wheel time count number */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeCalibration, FMSTR_TSA_UINT16)              /* M1 Calibration time count number */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16TimeFaultRelease, FMSTR_TSA_UINT16)             /* M1 Fault time count number */

FMSTR_TSA_TABLE_END()

#ifdef DUAL_MOTOR
/*!
 * @brief M2 Global table with global variables used in TSA
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(M2_global_table)

/* global variables & control */
FMSTR_TSA_RW_VAR(g_bM2SwitchAppOnOff, FMSTR_TSA_UINT16) /* M2 Application Switch */
FMSTR_TSA_RW_VAR(g_sM2Ctrl.eState, FMSTR_TSA_UINT16)    /* M2 Application State */

/* global freemaster float variables */
FMSTR_TSA_RW_VAR(g_fltM2currentScale, FMSTR_TSA_FLOAT)         /* FMSTR_M2_currentScale */
FMSTR_TSA_RW_VAR(g_fltM2DCBvoltageScale, FMSTR_TSA_FLOAT)      /* FMSTR_M2_DCBvoltageScale */
FMSTR_TSA_RW_VAR(g_fltM2speedScale, FMSTR_TSA_FLOAT)           /* FMSTR_M2_speedScale */
FMSTR_TSA_RW_VAR(g_fltM2voltageScale, FMSTR_TSA_FLOAT)         /* FMSTR_M2_voltageScale */
FMSTR_TSA_RW_VAR(g_fltM2speedAngularScale, FMSTR_TSA_FLOAT)    /* FMSTR_M2_speedAngularScale */
FMSTR_TSA_RW_VAR(g_fltM2speedMechanicalScale, FMSTR_TSA_FLOAT) /* FMSTR_M2_speedMechanicalScale */

FMSTR_TSA_RW_VAR(g_eM2StateRun, FMSTR_TSA_UINT16) /* M2 State Run */

FMSTR_TSA_RW_VAR(g_sM2Drive.ui16TimeFullSpeedFreeWheel, FMSTR_TSA_UINT16)       /* M2 Free-wheel time count number */
FMSTR_TSA_RW_VAR(g_sM2Drive.ui16TimeCalibration, FMSTR_TSA_UINT16)              /* M2 Calibration time count number */
FMSTR_TSA_RW_VAR(g_sM2Drive.ui16TimeFaultRelease, FMSTR_TSA_UINT16)             /* M2 Fault time count number */

FMSTR_TSA_TABLE_END()

#endif

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
FMSTR_TSA_RO_VAR(g_sAppIdFM.ui16FeatureSet, FMSTR_TSA_UINT16)

FMSTR_TSA_TABLE_END()

/*!
 * @brief MID 2.0 table structure
 *
 * @param None
 *
 * @return None
 */
/* MID TSA table */
FMSTR_TSA_TABLE_BEGIN(TSA_MID)

    /* Measurement control from application */
    FMSTR_TSA_RW_VAR(eUserMIDMeasType,       FMSTR_TSA_UINT16)
    FMSTR_TSA_RO_VAR(ui32UserMIDStartResult, FMSTR_TSA_UINT32)

    /* The measurement configuration setup. */
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltKeIdReqOpenLoop,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltKeFreqElReq,        FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPpIdReqOpenLoop,    FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltPpFreqElReq,        FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechKt,             FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqStartup,      FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechMergingCoeff,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqAccelerate,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechIqDecelerate,   FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsAccel, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsDecel, FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMeasConfig.fltMechSpeedThrsInteg, FMSTR_TSA_FLOAT)

    /* MID Status */
    FMSTR_TSA_RW_VAR(sUserMIDStatus.eMIDState,        FMSTR_TSA_UINT16)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui32FinishedMeas, FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui16FaultMID,     FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.ui16WarnMID,      FMSTR_TSA_UINT32)
    FMSTR_TSA_RW_VAR(sUserMIDStatus.fltSpeedAngScale, FMSTR_TSA_FLOAT)

    /* MID Config result */
    FMSTR_TSA_RW_VAR(ui16MeasConfigResult,        FMSTR_TSA_UINT16)

    /* MID known motor parameters. */
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltRs,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltLd,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltLq,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltKe,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltJ,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.fltB,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsKnown.ui32Pp,  FMSTR_TSA_UINT32)

    /* MID measured and/or known motor parameters. */
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltRs,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltLd,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltLq,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltKe,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltJ,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.fltB,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(sUserMIDMotorParamsMeas.ui32Pp,  FMSTR_TSA_UINT32)

    /* MID control */
    FMSTR_TSA_RW_VAR(g_eMidCmd,     FMSTR_TSA_UINT16)

    /* MID external and estimated position */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.f16PosElEst,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.f16PosElExt,    FMSTR_TSA_FRAC16)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.fltSpeedElEst,  FMSTR_TSA_FLOAT)

    /* FOC signals */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIDQ.fltD,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIDQ.fltQ,  FMSTR_TSA_FLOAT)

    /* Bemf observer parameters */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltIGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltUGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltEGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltWIGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain,  FMSTR_TSA_FLOAT)

    /* Tracking observer */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltPGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltIGain,  FMSTR_TSA_FLOAT)
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltThGain,  FMSTR_TSA_FLOAT)

    /* global freemaster float variables */
    FMSTR_TSA_RW_VAR(g_fltMIDcurrentScale, FMSTR_TSA_FLOAT)         /* FMSTR_MID_currentScale */
    FMSTR_TSA_RW_VAR(g_fltMIDDCBvoltageScale, FMSTR_TSA_FLOAT)      /* FMSTR_MID_DCBvoltageScale */
    FMSTR_TSA_RW_VAR(g_fltMIDspeedScale, FMSTR_TSA_FLOAT)           /* FMSTR_MID_speedScale */
    FMSTR_TSA_RW_VAR(g_fltMIDvoltageScale, FMSTR_TSA_FLOAT)         /* FMSTR_MID_voltageScale */
    FMSTR_TSA_RW_VAR(g_fltMIDspeedAngularScale, FMSTR_TSA_FLOAT)    /* FMSTR_MID_speedAngularScale */

    FMSTR_TSA_RW_VAR(g_sMidDrive.ui16TimeCalibration, FMSTR_TSA_UINT16)              /* MID Calibration time count number */

    /* sFocPMSM.sIdPiParams definitions */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIdPiParams.fltIGain, FMSTR_TSA_FLOAT) /* MID Id Ki Gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIdPiParams.fltPGain, FMSTR_TSA_FLOAT) /* MID Id Kp Gain */

    /* sFocPMSM.sIqPiParams definitions */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIqPiParams.fltIGain, FMSTR_TSA_FLOAT) /* MID Iq Ki Gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIqPiParams.fltPGain, FMSTR_TSA_FLOAT) /* MID Iq Kp Gain */

    /* sFocPMSM.sBemfObsrv definitions */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltEGain, FMSTR_TSA_FLOAT)       /* MID Obsrv E gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltIGain, FMSTR_TSA_FLOAT)       /* MID Obsrv I gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltIGain, FMSTR_TSA_FLOAT) /* MID Obsrv Ki gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.sCtrl.fltPGain, FMSTR_TSA_FLOAT) /* MID Obsrv Kp gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltUGain, FMSTR_TSA_FLOAT)       /* MID Obsrv U gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sBemfObsrv.fltWIGain, FMSTR_TSA_FLOAT)      /* MID Obsrv WI gain */

    /* sFocPMSM.sTo definitions */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltIGain, FMSTR_TSA_FLOAT)  /* MID Obsrv To Ki gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltPGain, FMSTR_TSA_FLOAT)  /* MID Obsrv To Kp gain */
    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sTo.fltThGain, FMSTR_TSA_FLOAT) /* MID Obsrv To Theta gain */

    FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.fltDutyCycleLimit, FMSTR_TSA_FLOAT) /* MID Current Loop Limit */

FMSTR_TSA_TABLE_END()

/* AP identification variables */
/*!
* @brief AP MID table structure
*
* @param None
*
* @return None
*/
/* MID_AP TSA table */
FMSTR_TSA_TABLE_BEGIN(TSA_AP_MID)

   FMSTR_TSA_RW_VAR(g_sEstimRLCtrlRun.fltIDcDReq, FMSTR_TSA_FLOAT)                     /* MID AP manual - Required DC current in d-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLCtrlRun.fltIDcQReq, FMSTR_TSA_FLOAT)                     /* MID AP manual - Required DC current in q-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLCtrlRun.fltIAcReq, FMSTR_TSA_FLOAT)                      /* MID AP manual - Required AC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLCtrlRun.u16FAc, FMSTR_TSA_UINT16)                        /* MID AP manual - Required frequency of AC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLCtrlRun.u8LdqSwitch, FMSTR_TSA_UINT8)                    /* MID AP manual - Swicth between Ld/Lq */
   FMSTR_TSA_RW_VAR(u8ModeEstimRL, FMSTR_TSA_UINT8)                                    /* MID AP - Selected mode */

   FMSTR_TSA_RW_VAR(fltIDcPlot , FMSTR_TSA_FLOAT)                                      /* MID AP identification  - plot DC current */
   FMSTR_TSA_RW_VAR(fltLdPlot, FMSTR_TSA_FLOAT)                                        /* MID AP identification  - plot inductance in d-axis */
   FMSTR_TSA_RW_VAR(fltLqPlot, FMSTR_TSA_FLOAT)                                        /* MID AP identification  - plot inductance in q-axis */


   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltIDcDReq, FMSTR_TSA_FLOAT)          /* MID AP - Required DC current in d-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltIDcD, FMSTR_TSA_FLOAT)             /* MID AP - DC current in d-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltIDcQReq, FMSTR_TSA_FLOAT)          /* MID AP - Required DC current in q-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltIDcQ, FMSTR_TSA_FLOAT)             /* MID AP - DC current in q-axis */

   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltIAcReq, FMSTR_TSA_FLOAT)           /* MID AP - Required AC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.pIDQAcFilt.fltD, FMSTR_TSA_FLOAT)     /* MID AP - Filtered d-part of the estimated AC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.pIDQAcFilt.fltQ, FMSTR_TSA_FLOAT)     /* MID AP - Filtered q-part of the estimated AC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltUDcDAcc, FMSTR_TSA_FLOAT)          /* MID AP - DC voltage integrator in d-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.pInnerState.fltUDcQAcc, FMSTR_TSA_FLOAT)          /* MID AP - DC voltage integrator in q-axis */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.fltLd, FMSTR_TSA_FLOAT)                           /* MID AP - estimated d-axis inductance */
   FMSTR_TSA_RW_VAR(g_sEstimRLStruct.fltLq, FMSTR_TSA_FLOAT)                           /* MID AP - estimated q-axis inductance  */

   FMSTR_TSA_RW_VAR(g_sEstimRLInitFMSTR.fltIDcNom, FMSTR_TSA_FLOAT)                    /* MID AP - Nominal DC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLInitFMSTR.fltIDcPosMax, FMSTR_TSA_FLOAT)                 /* MID AP - Maximum positive DC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLInitFMSTR.fltIDcNegMax, FMSTR_TSA_FLOAT)                 /* MID AP - Maximum negative DC current */
   FMSTR_TSA_RW_VAR(g_sEstimRLInitFMSTR.fltIDcLd, FMSTR_TSA_FLOAT)                     /* MID AP - Current used for Ld measurement */
   FMSTR_TSA_RW_VAR(g_sEstimRLInitFMSTR.fltIDcLq, FMSTR_TSA_FLOAT)                     /* MID AP - Current used for Lq measurement */
FMSTR_TSA_TABLE_END()

/*!
 * @brief g_sMidDrive table structure
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(gsMidDrive_table)

  /* gsM1Drive structure definition */
  FMSTR_TSA_RW_VAR(g_sMidDrive.bFaultClearMan, FMSTR_TSA_UINT16)       /* MC MID Fault Clear */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultIdCaptured, FMSTR_TSA_UINT16)     /* MC MID Captured Fault */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultIdPending, FMSTR_TSA_UINT16)      /* MC MID Pending Fault */

  /* sFaultThresholds definitions */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltUqBemf, FMSTR_TSA_FLOAT)      /* MC MID Fault Threshold BemfBlocked */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltUDcBusOver, FMSTR_TSA_FLOAT)  /* MC MID Fault Threshold DcBusOver */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltUDcBusTrip, FMSTR_TSA_FLOAT)  /* MC MID Fault Threshold DcBusTrip */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltUDcBusUnder, FMSTR_TSA_FLOAT) /* MC MID Fault Threshold DcBusUnder */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltSpeedMin, FMSTR_TSA_FLOAT)    /* MC MID Fault Threshold SpeedMin */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltSpeedNom, FMSTR_TSA_FLOAT)    /* MC MID Fault Threshold SpeedNom */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFaultThresholds.fltSpeedOver, FMSTR_TSA_FLOAT)   /* MC MID Fault Threshold SpeedOver */

  FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.fltUDcBusFilt, FMSTR_TSA_FLOAT)           /* MC MID DCB Voltage Filtered */

  /* sFocPMSM.sIABC definitions */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIABC.fltA, FMSTR_TSA_FLOAT)              /* MC MID Phase Current A */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIABC.fltB, FMSTR_TSA_FLOAT)              /* MC MID Phase Current B */
  FMSTR_TSA_RW_VAR(g_sMidDrive.sFocPMSM.sIABC.fltC, FMSTR_TSA_FLOAT)              /* MC MID Phase Current C */

FMSTR_TSA_TABLE_END()

/*!
 * @brief Table for control switching between M1 and MID
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(gsM1MidSwitch_table)

FMSTR_TSA_RW_VAR(g_sSpinMidSwitch.bCmdRunM1, FMSTR_TSA_UINT16)            /* Request to run M1 */
FMSTR_TSA_RW_VAR(g_sSpinMidSwitch.bCmdRunMid, FMSTR_TSA_UINT16)           /* Request to run MID */
FMSTR_TSA_RW_VAR(g_sSpinMidSwitch.eAppState, FMSTR_TSA_UINT16)            /* MID/Spin or transition between them */
FMSTR_TSA_RW_VAR(g_sSpinMidSwitch.sFaultCtrlM1_Mid, FMSTR_TSA_UINT16)

FMSTR_TSA_TABLE_END()

/*!
 * @brief TSA Table list required if TSA macro is enabled
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE(global_table)
FMSTR_TSA_TABLE(M1_global_table)
FMSTR_TSA_TABLE(gsM1Drive_table)
#ifdef DUAL_MOTOR
FMSTR_TSA_TABLE(M2_global_table)
FMSTR_TSA_TABLE(gsM2Drive_table)
#endif

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_TABLE(gsM1Enc_table)
#ifdef DUAL_MOTOR
FMSTR_TSA_TABLE(gsM2Enc_table)
#endif
#endif

FMSTR_TSA_TABLE(TSA_MID)
FMSTR_TSA_TABLE(TSA_AP_MID)
FMSTR_TSA_TABLE(gsMidDrive_table)
FMSTR_TSA_TABLE(gsM1MidSwitch_table)
FMSTR_TSA_TABLE_LIST_END()
