/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
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
extern bool_t bDemoMode;

/* global used misc variables */
extern uint32_t g_ui32NumberOfCycles;
extern uint32_t g_ui32MaxNumberOfCycles;

extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_pmsm_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;

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
    FMSTR_TSA_RW_VAR(g_fltM1currentScale, FMSTR_TSA_FLOAT)          /* FMSTR_M1_currentScale */
    FMSTR_TSA_RW_VAR(g_fltM1DCBvoltageScale, FMSTR_TSA_FLOAT)       /* FMSTR_M1_DCBvoltageScale */
    FMSTR_TSA_RW_VAR(g_fltM1frequencyScale, FMSTR_TSA_FLOAT)        /* FMSTR_M1_frequencyScale */
    FMSTR_TSA_RW_VAR(g_fltM1speedScale, FMSTR_TSA_FLOAT)            /* FMSTR_M1_speedScale */
    FMSTR_TSA_RW_VAR(g_fltM1voltageScale, FMSTR_TSA_FLOAT)          /* FMSTR_M1_voltageScale */

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
FMSTR_TSA_TABLE_LIST_END()
