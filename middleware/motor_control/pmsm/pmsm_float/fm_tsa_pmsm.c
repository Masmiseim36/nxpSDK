/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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

/* global control variables */
#ifdef PMSM_SNSLESS_ENC
extern bool_t bDemoModePosition;
#endif
extern bool_t bDemoModeSpeed;

/* global used misc variables */
extern uint32_t g_ui32NumberOfCycles;
extern uint32_t g_ui32MaxNumberOfCycles;

/* Application and board ID  */
extern app_ver_t g_sAppIdFM;

extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_pmsm_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;

#ifdef PMSM_SNSLESS_ENC
extern mcdrv_qd_enc_t g_sM1Enc;
#endif
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
FMSTR_TSA_RW_VAR(g_sM1Drive.bFaultClearMan, FMSTR_TSA_UINT16)       /* M1 Fault Clear */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdCaptured, FMSTR_TSA_UINT16)     /* M1 Captured Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdPending, FMSTR_TSA_UINT16)      /* M1 Pending Fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.eControl, FMSTR_TSA_UINT16)             /* M1 MCAT Control */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16SlowCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Slow Control Loop Frequency */
FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FastCtrlLoopFreq, FMSTR_TSA_UINT16) /* M1 Fast Control Loop Frequency */

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

/* sFaultThresholds definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUqBemf, FMSTR_TSA_FLOAT)      /* M1 Fault Threshold BemfBlocked */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusOver, FMSTR_TSA_FLOAT)  /* M1 Fault Threshold DcBusOver */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusTrip, FMSTR_TSA_FLOAT)  /* M1 Fault Threshold DcBusTrip */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusUnder, FMSTR_TSA_FLOAT) /* M1 Fault Threshold DcBusUnder */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedMin, FMSTR_TSA_FLOAT)    /* M1 Fault Threshold SpeedMin */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedNom, FMSTR_TSA_FLOAT)    /* M1 Fault Threshold SpeedNom */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedOver, FMSTR_TSA_FLOAT)   /* M1 Fault Threshold SpeedOver */

/* sStartUp definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16CoeffMerging, FMSTR_TSA_FRAC16)     /* M1 Merging Coefficient */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16RatioMerging, FMSTR_TSA_FRAC16)     /* M1 Merging Ratio */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M1 Merging Speed Catch Up */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16PosGen, FMSTR_TSA_FRAC16)           /* M1 Position Open Loop */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedCatchUp, FMSTR_TSA_FLOAT)      /* M1 Speed Merging Catch Up  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltSpeedRampOpenLoop, FMSTR_TSA_FLOAT) /* M1 Speed Ramp Open Loop  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.fltCurrentStartup, FMSTR_TSA_FLOAT)    /* M1 Startup Current  */

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

/*!
 * @brief MID table structure
 *
 * @param None
 *
 * @return None
 */

FMSTR_TSA_TABLE_BEGIN(sMID_table)

/* sMIDAlignment structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDAlignment.ui16Active, FMSTR_TSA_UINT16)        /* MID Align Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDAlignment.ui16AlignDuration, FMSTR_TSA_UINT16) /* MID Align AlignDuration */
FMSTR_TSA_RW_VAR(g_sMID.sMIDAlignment.fltCurrentAlign, FMSTR_TSA_FLOAT)    /* MID Align CurrentAlign */

/* sMIDKe structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDKe.ui16Active, FMSTR_TSA_UINT16)        /* MID Ke Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDKe.fltIdReqOpenLoop, FMSTR_TSA_FLOAT)   /* MID Ke IdReqOpenLoop */
FMSTR_TSA_RW_VAR(g_sMID.sMIDKe.fltKe, FMSTR_TSA_FLOAT)              /* MID Ke Ke */
FMSTR_TSA_RW_VAR(g_sMID.sMIDKe.ui16MCATObsrvDone, FMSTR_TSA_UINT16) /* MID Ke MCATObsrvDone */
FMSTR_TSA_RW_VAR(g_sMID.sMIDKe.fltFreqElReq, FMSTR_TSA_FLOAT)       /* MID Ke SpeedElReq */

/* sMIDLs structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.ui16Active, FMSTR_TSA_UINT16)       /* MID Ke Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltFreqMax, FMSTR_TSA_FLOAT)        /* MID Ls FreqMax */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltFreqDecrement, FMSTR_TSA_FLOAT)  /* MID Ls FreqDecrement */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltFreqMin, FMSTR_TSA_FLOAT)        /* MID Ls FreqMin */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltFreqStart, FMSTR_TSA_FLOAT)      /* MID Ls FreqStart */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltIdAmplitudeReq, FMSTR_TSA_FLOAT) /* MID Ls IdAmplitudeReq */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltLd, FMSTR_TSA_FLOAT)             /* MID Ls Ld */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltLq, FMSTR_TSA_FLOAT)             /* MID Ls Lq */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltLs, FMSTR_TSA_FLOAT)             /* MID Ls Ls */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltRs, FMSTR_TSA_FLOAT)             /* MID Ls Rs */
FMSTR_TSA_RW_VAR(g_sMID.sMIDLs.fltUdIncrement, FMSTR_TSA_FLOAT)    /* MID Ls UdIncrement */

/* sMIDPp structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPp.ui16Active, FMSTR_TSA_UINT16)       /* MID Pp Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPp.fltIdReqOpenLoop, FMSTR_TSA_FLOAT)  /* MID Pp IdReqOpenLoop */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPp.ui16PpDetermined, FMSTR_TSA_UINT16) /* MID Pp PpDetermined */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPp.fltFreqElReq, FMSTR_TSA_FLOAT)      /* MID Pp FreqElReq */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPp.fltFreqMax, FMSTR_TSA_FLOAT)        /* MID Pp FreqMax */

/* sMIDRs structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDRs.ui16Active, FMSTR_TSA_UINT16) /* MID Rs Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDRs.fltIdMeas, FMSTR_TSA_FLOAT)   /* MID Rs IdMeas */
FMSTR_TSA_RW_VAR(g_sMID.sMIDRs.fltRs, FMSTR_TSA_FLOAT)       /* MID Rs Rs */

/* sMIDKe structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPwrStgChar.ui16Active, FMSTR_TSA_UINT16)      /* MID PwrStg Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPwrStgChar.fltIdCalib, FMSTR_TSA_FLOAT)       /* MID PwrStg IdCalib */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPwrStgChar.fltIdIncrement, FMSTR_TSA_FLOAT)   /* MID PwrStg IdIncrement */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPwrStgChar.fltRs, FMSTR_TSA_FLOAT)            /* MID PwrStg Rs */
FMSTR_TSA_RW_VAR(g_sMID.sMIDPwrStgChar.ui16NumOfChPnts, FMSTR_TSA_UINT16) /* MID PwrStg Num of Points */

/* Power Stage characteristic data */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltPwrStgCharLinCoeff, FMSTR_TSA_FLOAT) /* MID PwrStg Char Linear Coefficient */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.fltPwrStgCharIRange, FMSTR_TSA_FLOAT)   /* MID PwrStg Char Range */

/* sMIDMech structure definition */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.ui16Active, FMSTR_TSA_UINT16)       /* MID Mech Active */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltB, FMSTR_TSA_FLOAT)              /* MID Mech B */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltIqAccelerate, FMSTR_TSA_FLOAT)   /* MID Mech IqAccelerate */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltIqStartup, FMSTR_TSA_FLOAT)      /* MID Mech IqStartup */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltJ, FMSTR_TSA_FLOAT)              /* MID Mech J */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltKt, FMSTR_TSA_FLOAT)             /* MID Mech Kt */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltPp, FMSTR_TSA_FLOAT)             /* MID Mech Pp */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltSpeedThrsAccel, FMSTR_TSA_FLOAT) /* MID Mech SpeedAccelThreshold */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltSpeedThrsDecel, FMSTR_TSA_FLOAT) /* MID Mech SpeedDecelThreshold */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltSpeedThrsInteg, FMSTR_TSA_FLOAT) /* MID Mech SpeedIntegThreshold */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.sStartup.sSpeedRampOpenLoopParams.fltRampDown,
                 FMSTR_TSA_FLOAT) /* MID Mech StartupRampDown */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.sStartup.sSpeedRampOpenLoopParams.fltRampUp,
                 FMSTR_TSA_FLOAT)                                  /* MID Mech StartupRampUp */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.ui32TimeMeasMax, FMSTR_TSA_FLOAT) /* MID Mech TimeoutCount */
FMSTR_TSA_RW_VAR(g_sMID.sMIDMech.fltTauMech, FMSTR_TSA_FLOAT)      /* MID Mech Tm */

/* MIDPwrStgChar Char ERROR */
FMSTR_TSA_RW_MEM(g_sMID.sMIDPwrStgChar.fltUdErrorLookUp,
                 FMSTR_TSA_FLOAT,
                 &g_sMID.sMIDPwrStgChar.fltUdErrorLookUp[0],
                 (65 << 2)) /* MID Ud Error Lookup */

/* dead-time compensation voltage table */
FMSTR_TSA_RW_MEM(pfltUDtComp, FMSTR_TSA_FLOAT, &pfltUDtComp[0], (65 << 2)) /* pfltUDtComp[] */

FMSTR_TSA_TABLE_END()

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

FMSTR_TSA_TABLE_END()
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
FMSTR_TSA_RW_VAR(bDemoModePosition, FMSTR_TSA_UINT16) /* Demo Mode Position */
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

/* global MCAT variables */
FMSTR_TSA_RW_VAR(g_sMID.eMeasurementType, FMSTR_TSA_UINT16)      /* MID Calibration */
FMSTR_TSA_RW_VAR(g_sMID.ui16EnableMeasurement, FMSTR_TSA_UINT16) /* MID EnableMeasurement */
FMSTR_TSA_RW_VAR(g_sMID.ui16FaultMID, FMSTR_TSA_UINT16)          /* MID FaultMID */
FMSTR_TSA_RW_VAR(g_sMID.ui16WarnMID, FMSTR_TSA_UINT16)           /* MID WarnMID */

/* MID variables */
FMSTR_TSA_RW_VAR(g_sMIDCtrl.eState, FMSTR_TSA_UINT16) /* MID State */

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
FMSTR_TSA_RO_MEM(g_sAppIdFM.cBoardID, FMSTR_TSA_UINT8, &g_sAppIdFM.cBoardID[0], 15)
FMSTR_TSA_RO_MEM(g_sAppIdFM.cMotorType, FMSTR_TSA_UINT8, &g_sAppIdFM.cMotorType[0], 4)
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
FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE(global_table)
FMSTR_TSA_TABLE(M1_global_table)
FMSTR_TSA_TABLE(gsM1Drive_table)

#ifdef PMSM_SNSLESS_ENC
FMSTR_TSA_TABLE(gsM1Enc_table)
#endif

FMSTR_TSA_TABLE(sMID_table)
FMSTR_TSA_TABLE_LIST_END()
