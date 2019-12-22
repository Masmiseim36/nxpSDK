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
extern bool_t               bDemoMode;

/* global used misc variables */
extern uint32_t             g_ui32NumberOfCycles;
extern uint32_t             g_ui32MaxNumberOfCycles;

/* Application and board ID  */
extern app_ver_t            g_sAppIdFM;

extern bool_t               g_bM1SwitchAppOnOff;
extern mcdef_pmsm_t         g_sM1Drive;
extern sm_app_ctrl_t        g_sM1Ctrl;

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
    FMSTR_TSA_RW_VAR(g_sM1Drive.bFaultClearMan,   FMSTR_TSA_UINT16)         /* Fault Clear */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdCaptured,   FMSTR_TSA_UINT16)       /* Captured Fault */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdPending,   FMSTR_TSA_UINT16)        /* Pending Fault */
    FMSTR_TSA_RW_VAR(g_sM1Drive.eControl,   FMSTR_TSA_UINT16)               /* MCAT Control */
    FMSTR_TSA_RW_VAR(g_sM1Drive.ui16SlowCtrlLoopFreq,   FMSTR_TSA_UINT16)   /* Slow Control Loop Frequency */
    FMSTR_TSA_RW_VAR(g_sM1Drive.ui16FastCtrlLoopFreq,   FMSTR_TSA_UINT16)   /* Fast Control Loop Frequency */


    /* gsM1Drive.sSpeed structure definition */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.f16SpeedFilt,   FMSTR_TSA_FRAC16)    /* Speed filtered */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.f16Speed,   FMSTR_TSA_FRAC16)        /* Speed Estimated */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.f16SpeedRamp,   FMSTR_TSA_FRAC16)    /* Speed Ramp */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.f16SpeedCmd,   FMSTR_TSA_FRAC16)     /* Speed Required */

    /* sSpeed.sSpeedFilter.sSpeedFilter definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32A1,   FMSTR_TSA_FRAC32)        /* Speed Filter A1 */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32B0,   FMSTR_TSA_FRAC32)        /* Speed Filter B0 */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedFilter.sFltCoeff.f32B1,   FMSTR_TSA_FRAC32)        /* Speed Filter B1 */

    /* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedRampParams.f32RampDown,   FMSTR_TSA_FRAC32)        /* Speed Ramp Down */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedRampParams.f32RampUp,   FMSTR_TSA_FRAC32)          /* Speed Ramp Up */

    /* sSpeed.sSpeedFilter.sSpeedRampParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.a32IGain,   FMSTR_TSA_UFRAC_UQ(16, 15))       /* Speed Loop Ki Gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.a32PGain,   FMSTR_TSA_UFRAC_UQ(16, 15))       /* Speed Loop Kp Gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.f16UpperLim,   FMSTR_TSA_FRAC16)              /* Speed Loop Limit High */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sSpeed.sSpeedPiParams.f16LowerLim,   FMSTR_TSA_FRAC16)              /* Speed Loop Limit Low */

    /* sSpeed.sAlignment definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sAlignment.ui16Time,   FMSTR_TSA_UINT16)        /* Alignment Duration */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sAlignment.f16UdReq,   FMSTR_TSA_FRAC16)        /* Alignment Voltage */

    /* gsM1Drive.sFocPMSM structure definition */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16DutyCycleLimit,  FMSTR_TSA_FRAC16)      /* Current Loop Limit */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16UDcBus,   FMSTR_TSA_FRAC16)             /* DCB Voltage */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16UDcBusFilt,   FMSTR_TSA_SINT16)         /* DCB Voltage Filtered */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosElExt,   FMSTR_TSA_FRAC16)           /* Posirtion External */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosEl,   FMSTR_TSA_FRAC16)              /* Position Electrical */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.f16PosElEst,   FMSTR_TSA_FRAC16)           /* Position Estimated */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.ui16SectorSVM,   FMSTR_TSA_UINT16)         /* SVM Sector */

    /* sFocPMSM.sIAlBe definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIAlBe.f16Alpha,   FMSTR_TSA_FRAC16)       /* I alpha */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIAlBe.f16Beta,   FMSTR_TSA_FRAC16)        /* I beta */

    /* sFocPMSM.sIDQ definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQ.f16D,   FMSTR_TSA_FRAC16)     /* Id */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQ.f16Q,   FMSTR_TSA_FRAC16)     /* Iq */

    /* sFocPMSM.sIDQReq definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQReq.f16D,   FMSTR_TSA_FRAC16)      /* Id req */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIDQReq.f16Q,   FMSTR_TSA_FRAC16)      /* Iq req */

    /* sFocPMSM.sIDQReq definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDQReq.f16D,   FMSTR_TSA_FRAC16)      /* Ud req */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sUDQReq.f16Q,   FMSTR_TSA_FRAC16)      /* Uq req */

    /* sFocPMSM.sIdPiParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIdPiParams.a32IGain,   FMSTR_TSA_UFRAC_UQ(16, 15))        /* Id Ki Gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIdPiParams.a32PGain,   FMSTR_TSA_UFRAC_UQ(16, 15))        /* Id Kp Gain */

    /* sFocPMSM.sBemfObsrv definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.a32EGain,   FMSTR_TSA_UFRAC_UQ(16, 15))         /* Obsrv E gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.a32IGain,   FMSTR_TSA_UFRAC_UQ(16, 15))         /* Obsrv I gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.a32IGain,   FMSTR_TSA_UFRAC_UQ(16, 15))   /* Obsrv Ki gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.sCtrl.a32PGain,   FMSTR_TSA_UFRAC_UQ(16, 15))   /* Obsrv Kp gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.a32UGain,   FMSTR_TSA_UFRAC_UQ(16, 15))         /* Obsrv U gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sBemfObsrv.a32WIGain,   FMSTR_TSA_UFRAC_UQ(16, 15))        /* Obsrv WI gain */

    /* sFocPMSM.sTo definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.f16IGain,   FMSTR_TSA_FRAC16)          /* Obsrv To Ki gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.i16IGainSh,   FMSTR_TSA_SINT16)        /* Obsrv To Ki shift */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.f16PGain,   FMSTR_TSA_FRAC16)          /* Obsrv To Kp gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.i16PGainSh,   FMSTR_TSA_SINT16)        /* Obsrv To Kp shift */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.f16ThGain ,   FMSTR_TSA_FRAC16)        /* Obsrv To Theta gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sTo.i16ThGainSh ,   FMSTR_TSA_SINT16)      /* Obsrv To Theta shift */

    /* sFocPMSM.sIqPiParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIqPiParams.a32IGain,   FMSTR_TSA_UFRAC_UQ(16, 15))        /* Iq Ki Gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIqPiParams.a32PGain,   FMSTR_TSA_UFRAC_UQ(16, 15))        /* Iq Kp Gain */

    /* sFocPMSM.sIABC definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.f16A,   FMSTR_TSA_FRAC16)        /* Phase Current A */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.f16B,   FMSTR_TSA_FRAC16)        /* Phase Current B */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFocPMSM.sIABC.f16C,   FMSTR_TSA_FRAC16)        /* Phase Current C */

    /* sFaultThresholds definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UqBemf,   FMSTR_TSA_FRAC16)         /* Fault Threshold BemfBlocked */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UDcBusOver,   FMSTR_TSA_FRAC16)     /* Fault Threshold DcBusOver */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UDcBusTrip,   FMSTR_TSA_FRAC16)     /* Fault Threshold DcBusTrip */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16UDcBusUnder,   FMSTR_TSA_FRAC16)    /* Fault Threshold DcBusUnder */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16SpeedMin,   FMSTR_TSA_FRAC16)       /* Fault Threshold SpeedMin */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16SpeedNom,   FMSTR_TSA_FRAC16)       /* Fault Threshold SpeedNom */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.f16SpeedOver,   FMSTR_TSA_FRAC16)      /* Fault Threshold SpeedOver */

    /* sStartUp definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16CoeffMerging,   FMSTR_TSA_FRAC16)       /* Merging Coefficient */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16RatioMerging,   FMSTR_TSA_FRAC16)       /* Merging Ratio */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16SpeedCatchUp,   FMSTR_TSA_FRAC16)       /* Merging Speed Catch Up */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16PosGen,   FMSTR_TSA_FRAC16)             /* Position Open Loop */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16SpeedCatchUp,   FMSTR_TSA_FRAC16)       /* Speed Merging Catch Up  */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16SpeedRampOpenLoop,   FMSTR_TSA_FRAC16)  /* Speed Ramp Open Loop  */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.f16CurrentStartup,   FMSTR_TSA_FRAC16)     /* Startup Current  */

    /* sStartUp.sSpeedRampOpenLoopParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.f32RampDown,   FMSTR_TSA_FRAC32)      /* Startup Ramp Dec */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sStartUp.sSpeedRampOpenLoopParams.f32RampUp,   FMSTR_TSA_FRAC32)        /* Startup Ramp Inc */

    /* sScalarCtrl definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16PosElScalar,   FMSTR_TSA_FRAC16)     /* Position Electrical Scalar */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16FreqRamp,   FMSTR_TSA_FRAC16)        /* Scalar Frequency Ramp */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16FreqCmd,   FMSTR_TSA_FRAC16)         /* Scalar speed */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16VHzGain,   FMSTR_TSA_FRAC16)         /* VHz Factor Gain */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.f16VHzGainShift,   FMSTR_TSA_FRAC16)    /* VHz Factor Gain Shift */

    /* sScalarCtrl.sFreqRampParams definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sFreqRampParams.f32RampDown,   FMSTR_TSA_FRAC32)    /* Scalar Ramp Down */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sFreqRampParams.f32RampUp,   FMSTR_TSA_FRAC32)      /* Scalar Ramp Up */

    /* sScalarCtrl.sUDQReq definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sUDQReq.f16Q,   FMSTR_TSA_FRAC16)       /* Scalar volt */

    /* sMCATctrl definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.ui16PospeSensor,   FMSTR_TSA_UINT16)      /* MCAT POSPE Sensor */

    /* sMCATctrl.sIDQReqMCAT definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sIDQReqMCAT.f16D,   FMSTR_TSA_FRAC16)     /* MCAT Id Required */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sIDQReqMCAT.f16Q,   FMSTR_TSA_FRAC16)     /* MCAT Iq Required */

    /* sMCATctrl.sUDQReqMCAT definitions */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sUDQReqMCAT.f16D,   FMSTR_TSA_FRAC16)     /* MCAT Ud Required */
    FMSTR_TSA_RW_VAR(g_sM1Drive.sMCATctrl.sUDQReqMCAT.f16Q,   FMSTR_TSA_FRAC16)     /* MCAT Uq Required */

FMSTR_TSA_TABLE_END()

/*!
 * @brief MID table structure - usable if DEMO mode is not used
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(sMID_table)

    /* sMIDAlignment structure definition */
    FMSTR_TSA_RW_VAR(sMIDAlignment.ui16Active,   FMSTR_TSA_UINT16)          /* MID Align Active */
    FMSTR_TSA_RW_VAR(sMIDAlignment.ui16AlignDuration,   FMSTR_TSA_UINT16)   /* MID Align AlignDuration */
    FMSTR_TSA_RW_VAR(sMIDAlignment.f16CurrentAlign,   FMSTR_TSA_FRAC16)     /* MID Align CurrentAlign */

    /* sMIDKe structure definition */
    FMSTR_TSA_RW_VAR(sMIDKe.ui16Active,   FMSTR_TSA_UINT16)         /* MID Ke Active */
    FMSTR_TSA_RW_VAR(sMIDKe.f16IdReqOpenLoop,   FMSTR_TSA_FRAC16)   /* MID Ke IdReqOpenLoop */
    FMSTR_TSA_RW_VAR(sMIDKe.f16Ke,   FMSTR_TSA_FRAC16)              /* MID Ke Ke */
    FMSTR_TSA_RW_VAR(sMIDKe.ui16MCATObsrvDone,   FMSTR_TSA_UINT16)  /* MID Ke MCATObsrvDone */
    FMSTR_TSA_RW_VAR(sMIDKe.i16ShiftKeMax,   FMSTR_TSA_SINT16)      /* MID Ke ShiftKeMax */
    FMSTR_TSA_RW_VAR(sMIDKe.f16SpeedElReq,   FMSTR_TSA_FRAC16)      /* MID Ke SpeedElReq */

    /* sMIDKe.sSpeedIntegrator definitions */
    FMSTR_TSA_RW_VAR(sMIDKe.sSpeedIntegrator.a32Gain,   FMSTR_TSA_FRAC_Q(16, 15))       /* MID Ke SpeedIntegrator.f16C1 */

    /* sMIDLs structure definition */
    FMSTR_TSA_RW_VAR(sMIDLs.ui16Active,   FMSTR_TSA_UINT16)             /* MID Ke Active */
    FMSTR_TSA_RW_VAR(sMIDLs.f16FreqDecrement,   FMSTR_TSA_FRAC16)       /* MID Ls FreqDecrement */
    FMSTR_TSA_RW_VAR(sMIDLs.f16FreqMin,   FMSTR_TSA_FRAC16)             /* MID Ls FreqMin */
    FMSTR_TSA_RW_VAR(sMIDLs.f16FreqStart,   FMSTR_TSA_FRAC16)           /* MID Ls FreqStart */
    FMSTR_TSA_RW_VAR(sMIDLs.f16IdAmplitudeReq,   FMSTR_TSA_FRAC16)      /* MID Ls IdAmplitudeReq */
    FMSTR_TSA_RW_VAR(sMIDLs.f16Ld,   FMSTR_TSA_FRAC16)                  /* MID Ls Ld */
    FMSTR_TSA_RW_VAR(sMIDLs.f16Lq,   FMSTR_TSA_FRAC16)                  /* MID Ls Lq */
    FMSTR_TSA_RW_VAR(sMIDLs.f16Ls,   FMSTR_TSA_FRAC16)                  /* MID Ls Ls */
    FMSTR_TSA_RW_VAR(sMIDLs.f16Rs,   FMSTR_TSA_FRAC16)                  /* MID Ls Rs */
    FMSTR_TSA_RW_VAR(sMIDLs.i16ShiftZqMax,   FMSTR_TSA_SINT16)          /* MID Ls ShiftZqMax */
    FMSTR_TSA_RW_VAR(sMIDLs.i16ShiftLdMax,   FMSTR_TSA_SINT16)          /* MID Ls ShiftLdMax */
    FMSTR_TSA_RW_VAR(sMIDLs.i16ShiftLqMax,   FMSTR_TSA_SINT16)          /* MID Ls ShiftLqMax */
    FMSTR_TSA_RW_VAR(sMIDLs.i16ShiftZdMax,   FMSTR_TSA_SINT16)          /* MID Ls ShiftZdMax */
    FMSTR_TSA_RW_VAR(sMIDLs.f16UdIncrement,   FMSTR_TSA_FRAC16)         /* MID Ls UdIncrement */

    /* sMIDPp structure definition */
    FMSTR_TSA_RW_VAR(sMIDPp.ui16Active,   FMSTR_TSA_UINT16)             /* MID Pp Active */
    FMSTR_TSA_RW_VAR(sMIDPp.f16IdReqOpenLoop,   FMSTR_TSA_FRAC16)       /* MID Pp IdReqOpenLoop */
    FMSTR_TSA_RW_VAR(sMIDPp.ui16PpDetermined,   FMSTR_TSA_UINT16)       /* MID Pp PpDetermined */
    FMSTR_TSA_RW_VAR(sMIDPp.f16SpeedElReq,   FMSTR_TSA_FRAC16)          /* MID Pp SpeedElReq */

    /* sMIDRs structure definition */
    FMSTR_TSA_RW_VAR(sMIDRs.ui16Active,   FMSTR_TSA_UINT16)             /* MID Rs Active */
    FMSTR_TSA_RW_VAR(sMIDRs.f16IdMeas,   FMSTR_TSA_FRAC16)              /* MID Rs IdMeas */
    FMSTR_TSA_RW_VAR(sMIDRs.f16RescaleIdLUTGain,   FMSTR_TSA_FRAC16)    /* MID Rs RescaleIdLUTGain */
    FMSTR_TSA_RW_VAR(sMIDRs.i16RescaleIdLUTShift,   FMSTR_TSA_SINT16)   /* MID Rs RescaleIdLUTShift */
    FMSTR_TSA_RW_VAR(sMIDRs.f16Rs,   FMSTR_TSA_FRAC16)                  /* MID Rs Rs */
    FMSTR_TSA_RW_VAR(sMIDRs.i16ShiftRsMax,   FMSTR_TSA_SINT16)          /* MID Rs ShiftRsMax */

    /* MID Trans Char ERROR */
    FMSTR_TSA_RW_MEM(f16TransferCharError, FMSTR_TSA_FRAC16, &f16TransferCharError[0], (65 << 1)) /* MID TransCharError */

    /* sMIDKe structure definition */
    FMSTR_TSA_RW_VAR(sMIDPwrStgChar.ui16Active,   FMSTR_TSA_UINT16)             /* MID PwrStg Active */
    FMSTR_TSA_RW_VAR(sMIDPwrStgChar.f16IdCalib,   FMSTR_TSA_FRAC16)             /* MID PwrStg IdCalib */
    FMSTR_TSA_RW_VAR(sMIDPwrStgChar.f16IdIncrement,   FMSTR_TSA_FRAC16)         /* MID PwrStg IdIncrement */
    FMSTR_TSA_RW_VAR(sMIDPwrStgChar.f16Rs,   FMSTR_TSA_FRAC16)                  /* MID PwrStg Rs */
    FMSTR_TSA_RW_VAR(sMIDPwrStgChar.i16ShiftRsMax,   FMSTR_TSA_SINT16)          /* MID PwrStg ShiftRsMax */

    /* MIDPwrStgChar Char ERROR */
    FMSTR_TSA_RW_MEM(sMIDPwrStgChar.f16UdErrorLookUp, FMSTR_TSA_FRAC16, &sMIDPwrStgChar.f16UdErrorLookUp[0], (65 << 1)) /* MID Ud Error Lookup */

FMSTR_TSA_TABLE_END()

/*!
 * @brief Global table with global variables used in TSA
 *
 * @param None
 *
 * @return None
 */
FMSTR_TSA_TABLE_BEGIN(global_table)

    /* global variables & control */
    FMSTR_TSA_RW_VAR(g_ui32NumberOfCycles, FMSTR_TSA_UINT32)        /* Cycle Number */
    FMSTR_TSA_RW_VAR(g_ui32MaxNumberOfCycles, FMSTR_TSA_UINT32)     /* Cycle Number Maximum */
    FMSTR_TSA_RW_VAR(bDemoMode, FMSTR_TSA_UINT16)                   /* Demo Mode */
    FMSTR_TSA_RW_VAR(g_bM1SwitchAppOnOff, FMSTR_TSA_UINT16)         /* Application Switch */
    FMSTR_TSA_RW_VAR(g_sM1Ctrl.eState, FMSTR_TSA_UINT16)            /* Application State */

    /* global freemaster float variables */
    FMSTR_TSA_RW_VAR(s_fltM1currentScale, FMSTR_TSA_FLOAT)          /* FMSTR_M1_currentScale */
    FMSTR_TSA_RW_VAR(s_fltM1DCBvoltageScale, FMSTR_TSA_FLOAT)       /* FMSTR_M1_DCBvoltageScale */
    FMSTR_TSA_RW_VAR(s_fltM1frequencyScale, FMSTR_TSA_FLOAT)        /* FMSTR_M1_frequencyScale */
    FMSTR_TSA_RW_VAR(s_fltM1speedScale, FMSTR_TSA_FLOAT)            /* FMSTR_M1_speedScale */
    FMSTR_TSA_RW_VAR(s_fltM1voltageScale, FMSTR_TSA_FLOAT)          /* FMSTR_M1_voltageScale */


    FMSTR_TSA_RW_VAR(s_eM1StateRun, FMSTR_TSA_UINT16)               /* State Run */

    /* global freemaster float variables */
    FMSTR_TSA_RW_VAR(s_fltM1bemfScale, FMSTR_TSA_FLOAT)             /* FMSTR_M1_bemfScale */

    /* global freemaster float variables - MID */
    FMSTR_TSA_RW_VAR(fltMIDresistanceScale, FMSTR_TSA_FLOAT)        /* FMSTR_M1_bemfScale */
    FMSTR_TSA_RW_VAR(fltMIDfrequencyScale, FMSTR_TSA_FLOAT)         /* FMSTR_M1_currentScale */
    FMSTR_TSA_RW_VAR(fltMIDimpedanceScale, FMSTR_TSA_FLOAT)         /* FMSTR_M1_DCBvoltageScale */
    FMSTR_TSA_RW_VAR(fltMIDinductanceDScale, FMSTR_TSA_FLOAT)       /* FMSTR_M1_frequencyScale */
    FMSTR_TSA_RW_VAR(fltMIDinductanceQScale, FMSTR_TSA_FLOAT)       /* FMSTR_M1_speedScale */
    FMSTR_TSA_RW_VAR(fltMIDbemfConstScale, FMSTR_TSA_FLOAT)         /* FMSTR_M1_voltageScale */

    /* global MCAT variables */
    FMSTR_TSA_RW_VAR(ui16Calibration, FMSTR_TSA_UINT16)             /* MID Calibration */
    FMSTR_TSA_RW_VAR(ui16ElectricalParameters, FMSTR_TSA_UINT16)    /* MID ElectricalParameters */
    FMSTR_TSA_RW_VAR(ui16EnableMeasurement, FMSTR_TSA_UINT16)       /* MID EnableMeasurement */
    FMSTR_TSA_RW_VAR(ui16FaultMID, FMSTR_TSA_UINT16)                /* MID FaultMID */

    /* MID variables */
    FMSTR_TSA_RW_VAR(ui16PolePairs, FMSTR_TSA_UINT16)               /* MID PolePairs */
    FMSTR_TSA_RW_VAR(g_sMIDCtrl.eState, FMSTR_TSA_UINT16)           /* MID State */

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
    FMSTR_TSA_TABLE(gsM1Drive_table)
    FMSTR_TSA_TABLE(sMID_table)
    FMSTR_TSA_TABLE(global_table)
    FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE_LIST_END()


