/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fm_tsa_acim.h"

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

/* Application and board ID */
extern app_ver_t g_sAppIdFM;

extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_acim_t g_sM1Drive;
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
FMSTR_TSA_TABLE_BEGIN(g_sM1Drive_table)

/* g_sM1Drive structure definition */
#if (DEMO == 0)                                                    // Add variables used in non-demo project
FMSTR_TSA_RW_VAR(g_sM1Drive.eControl, FMSTR_TSA_UINT16)            /* MCAT control */
FMSTR_TSA_RW_VAR(g_sClockSetup.ui16SlowLoopFreq, FMSTR_TSA_UINT16) /* Slow control loop Frequency */
FMSTR_TSA_RW_VAR(g_sClockSetup.ui16FastLoopFreq, FMSTR_TSA_UINT16) /* Fast control loop Frequency */
#endif

FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdCaptured, FMSTR_TSA_UINT16) /* Captured fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultIdPending, FMSTR_TSA_UINT16)  /* Pending fault */
FMSTR_TSA_RW_VAR(g_sM1Drive.bFaultClearMan, FMSTR_TSA_UINT16)   /* Manual fault clearing */
FMSTR_TSA_RW_VAR(g_sM1Drive.fltIPMTemperature, FMSTR_TSA_FLOAT) /* IPM temperature */
FMSTR_TSA_RW_VAR(g_sM1Drive.fltTsFastLoop, FMSTR_TSA_FLOAT)     /* Fast loop sampling time */
FMSTR_TSA_RW_VAR(g_sM1Drive.fltTsSlowLoop, FMSTR_TSA_FLOAT)     /* Slow loop sampling time */

/* g_sM1Drive.sFocACIM structure definition */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.fltDutyLim, FMSTR_TSA_FLOAT)     /* Duty cycle limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.fltUDcBus, FMSTR_TSA_FLOAT)      /* DCB voltage float format */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.f16UDcBus, FMSTR_TSA_FRAC16)     /* DCB voltage frac16 format */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.fltUDcBusFilt, FMSTR_TSA_FLOAT)  /* DCB voltage filtered value*/
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.ui16SectorSVM, FMSTR_TSA_UINT16) /* SVM sector*/
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.bFlagDTComp, FMSTR_TSA_UINT16)   /* Dead time compensation */

/* sFocACIM.sFaultThresholds structure */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltSpeedOver, FMSTR_TSA_FLOAT)   /* Overspeed fault threshold */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusOver, FMSTR_TSA_FLOAT)  /* DCbus Overvoltage fault threshold */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusTrip, FMSTR_TSA_FLOAT)  /* Braking resistor threshold */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFaultThresholds.fltUDcBusUnder, FMSTR_TSA_FLOAT) /* DCbus Undervoltage fault threshold */

/* sFocACIM.sRFO structure */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiRotRDQ.fltD, FMSTR_TSA_FLOAT)          /* Rotor flux D-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiRotRDQ.fltQ, FMSTR_TSA_FLOAT)          /* Rotor flux Q-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiRotSAlBe.fltAlpha, FMSTR_TSA_FLOAT)    /* Rotor flux alpha-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiRotSAlBe.fltBeta, FMSTR_TSA_FLOAT)     /* Rotor flux beta-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiStatSAlBe.fltAlpha, FMSTR_TSA_FLOAT)   /* Stator flux alpha-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sPsiStatSAlBe.fltBeta, FMSTR_TSA_FLOAT)    /* Stator flux beta-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltTorque, FMSTR_TSA_FLOAT)                /* Torque */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.a32RotFluxPos, FMSTR_TSA_UFRAC_UQ(16, 15)) /* Rotor flux position gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sCtrl.fltIGain, FMSTR_TSA_FLOAT)           /* Integral gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.sCtrl.fltPGain, FMSTR_TSA_FLOAT)           /* Proportional gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltPsiRA1Gain, FMSTR_TSA_FLOAT)            /* Rotor flux A1 gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltPsiRB1Gain, FMSTR_TSA_FLOAT)            /* Rotor flux B1 gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltPsiSA1Gain, FMSTR_TSA_FLOAT)            /* Stator flux A1 gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltPsiSA2Gain, FMSTR_TSA_FLOAT)            /* Stator flux B1 gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltKrInvGain, FMSTR_TSA_FLOAT)             /* Rotor constant inverse gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltKrLsTotLeakGain, FMSTR_TSA_FLOAT)       /* Leak gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltRsEst, FMSTR_TSA_FLOAT)                 /* Stator resistance estimated */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sRFO.fltTorqueGain, FMSTR_TSA_FLOAT)            /* Torque gain */

/* sFocACIM.sSpdObs definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltA1,
                 FMSTR_TSA_FLOAT) /* Speed filter A1 coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltB0,
                 FMSTR_TSA_FLOAT) /* Speed filter B0 coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1Param.sFltCoeff.fltB1,
                 FMSTR_TSA_FLOAT)                                                   /* Speed filter B1 coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.sPsiRotRDQ.fltD, FMSTR_TSA_FLOAT)      /* Rotor flux observer d-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.sPsiRotRDQ.fltD, FMSTR_TSA_FLOAT)      /* Rotor flux observer q-axes */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedEl, FMSTR_TSA_FLOAT)           /* Speed electrical */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedElIIR1, FMSTR_TSA_FLOAT)       /* Speed filtered */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.a32RotPos, FMSTR_TSA_UFRAC_UQ(16, 15)) /* Rotor estimated position */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.sCtrl.fltIGain, FMSTR_TSA_FLOAT)       /* Integral gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.sCtrl.fltPGain, FMSTR_TSA_FLOAT)       /* Proportional gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltPsiRA1Gain, FMSTR_TSA_FLOAT)        /* Constant A1 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltPsiRB1Gain, FMSTR_TSA_FLOAT)        /* Constant B1 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltTs, FMSTR_TSA_FLOAT)                /* Sampling time */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sSpdObs.fltSpeedMeGain, FMSTR_TSA_FLOAT)       /* Speed gain constant */

/* sFocACIM.sPIParId definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sPIParId.fltIGain, FMSTR_TSA_FLOAT) /* Id Ki gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sPIParId.fltPGain, FMSTR_TSA_FLOAT) /* Id Kp gain */

/* sFocPMSM.sIqPiParams definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sPIParIq.fltIGain, FMSTR_TSA_FLOAT) /* Iq Ki gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sPIParIq.fltPGain, FMSTR_TSA_FLOAT) /* Iq Kp gain */

/* sFocACIM.sIABCFrac definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIABCFrac.f16A, FMSTR_TSA_FRAC16) /* Phase current Ia in frac16 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIABCFrac.f16B, FMSTR_TSA_FRAC16) /* Phase current Ib in frac16 */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIABCFrac.f16C, FMSTR_TSA_FRAC16) /* Phase current Ic in frac16 */

/* sFocACIM.sIAlBe definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIAlBe.fltAlpha, FMSTR_TSA_FLOAT) /* I alpha */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIAlBe.fltBeta, FMSTR_TSA_FLOAT)  /* I beta */

/* sFocACIM.sIDQ definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIDQ.fltD, FMSTR_TSA_FLOAT) /* Id */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIDQ.fltQ, FMSTR_TSA_FLOAT) /* Iq */

/* sFocACIM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIDQReq.fltD, FMSTR_TSA_FLOAT) /* Id req */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sIDQReq.fltQ, FMSTR_TSA_FLOAT) /* Iq req */

/* sFocPMSM.sIDQReq definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sUDQReq.fltD, FMSTR_TSA_FLOAT) /* Ud req */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sUDQReq.fltQ, FMSTR_TSA_FLOAT) /* Uq req */

/* sFocACIM.sDutyABC definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sDutyABC.f16A, FMSTR_TSA_FRAC16) /* Duty cycle A */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sDutyABC.f16B, FMSTR_TSA_FRAC16) /* Duty cycle B */
FMSTR_TSA_RW_VAR(g_sM1Drive.sFocACIM.sDutyABC.f16C, FMSTR_TSA_FRAC16) /* Duty cycle C */

/* g_sM1Drive.sSpdFlux definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltSpdMeFilt, FMSTR_TSA_FLOAT)      /* Actual filterer value of speed mechanical */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltSpdMeRamp, FMSTR_TSA_FLOAT)      /* Ramped speed mechanical */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltSpdMeReq, FMSTR_TSA_FLOAT)       /* Required value of speed mechanical */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltIdStart, FMSTR_TSA_FLOAT)        /* Id value for motor start-up */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltIdStartMinPct, FMSTR_TSA_FLOAT)  /* Id value for motor start-up in percentage */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltIdMTPAFiltCoef, FMSTR_TSA_FLOAT) /* Filter coeffient of MTPA filter */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltIdReqMax, FMSTR_TSA_FLOAT)       /* Maximal required Id value */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltIdReqMin, FMSTR_TSA_FLOAT)       /* Minimal required Id value */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltSpdMeReqMax, FMSTR_TSA_FLOAT)    /* Maximum limit of required speed */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.fltSpdMeReqMin, FMSTR_TSA_FLOAT)    /* Minimum limit of required speed */

/* sSpdFlux.sPIParSpdMe definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sPIParSpdMe.fltPGain, FMSTR_TSA_FLOAT)    /* Speed controller proportional gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sPIParSpdMe.fltIGain, FMSTR_TSA_FLOAT)    /* Speed controller integral gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sPIParSpdMe.fltLowerLim, FMSTR_TSA_FLOAT) /* Speed controller lower limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sPIParSpdMe.fltUpperLim, FMSTR_TSA_FLOAT) /* Speed controller upper limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sPIParSpdMe.fltBetaGain, FMSTR_TSA_FLOAT) /* Speed controller beta gain */

/* sSpdFlux.sFluxWkng definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltIGain,
                 FMSTR_TSA_FLOAT) /* Flux weakening controller integral gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltPGain,
                 FMSTR_TSA_FLOAT) /* Flux weakening controller proportional gain */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltLowerLim,
                 FMSTR_TSA_FLOAT) /* Flux weakening controller lower limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sFWPiParam.fltUpperLim,
                 FMSTR_TSA_FLOAT) /* Flux weakening controller upper limit */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltA1,
                 FMSTR_TSA_FLOAT) /* Flux weakening error filter A1 coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltB0,
                 FMSTR_TSA_FLOAT) /* Flux weakening error filter B0 coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sFluxWkng.sIqErrIIR1Param.sFltCoeff.fltB1,
                 FMSTR_TSA_FLOAT) /* Flux weakening error filter B1 coeff */

/* sSpdFlux.sFluxWkng definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sRampParSpdMe.fltDA, FMSTR_TSA_FLOAT)      /* Speed s-ramp coeff */
FMSTR_TSA_RW_VAR(g_sM1Drive.sSpdFlux.sRampParSpdMe.fltIncrMax, FMSTR_TSA_FLOAT) /* Speed s-ramp maximum increment */

/* g_sM1Drive.sScalarCtrl definitions */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sRampParSpdMe.fltRampUp, FMSTR_TSA_FLOAT)   /* Scalar ramp speed increment */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.sRampParSpdMe.fltRampDown, FMSTR_TSA_FLOAT) /* Scalar ramp speed decrement */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltSpdMeReq, FMSTR_TSA_FLOAT)  /* Required speed in scalar control */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltSpdMeRamp, FMSTR_TSA_FLOAT) /* Rampped motor speed mechanical  */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltPosEl, FMSTR_TSA_FLOAT)     /* Electrical position in scalar control */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltGainSpdMe2PosEl,
                 FMSTR_TSA_FLOAT) /* Scalar gain speed to electrical motor position */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltGainRpm2Volt, FMSTR_TSA_FLOAT) /* Scalar gain speed to voltage */
FMSTR_TSA_RW_VAR(g_sM1Drive.sScalarCtrl.fltUMin, FMSTR_TSA_FLOAT)         /* Minimal speed threshole */

FMSTR_TSA_TABLE_END()

/*!
 * @brief MID table structure - usable if DEMO mode is not used
 *
 * @param None
 *
 * @return None
 */
#if !defined(DEMO)
FMSTR_TSA_TABLE_BEGIN(g_sMID_table)
/* MID control variables */
FMSTR_TSA_RW_VAR(g_sMID.eCalcElPar, FMSTR_TSA_UINT8)      /* Status bit of control electrical parameter calculation */
FMSTR_TSA_RW_VAR(g_sMID.eCalcMechPar, FMSTR_TSA_UINT8)    /* Status bit of control mechanical parameter calculation */
FMSTR_TSA_RW_VAR(g_sMID.bRotBlocked, FMSTR_TSA_UINT16)    /* Blocked rotor status bit */
FMSTR_TSA_RW_VAR(g_sMID.bCalib, FMSTR_TSA_UINT16)         /* Calibration state bit */
FMSTR_TSA_RW_VAR(g_sMID.bMeasSuccDone, FMSTR_TSA_UINT16)  /* Measurement success bit */
FMSTR_TSA_RW_VAR(g_sMID.bParRestoreOld, FMSTR_TSA_UINT16) /* Restore previous data status bit */
FMSTR_TSA_RW_VAR(g_sMID.bParUpdateNew, FMSTR_TSA_UINT16)  /* Update new data status bit */
FMSTR_TSA_RW_VAR(g_sMID.fltFreqCLoop, FMSTR_TSA_FLOAT)    /* Frequency of current control loop */
FMSTR_TSA_RW_VAR(g_sMID.fltFreqSLoop, FMSTR_TSA_FLOAT)    /* Frequency of speed control loop */
FMSTR_TSA_RW_VAR(g_sMID.ui32LoopCntr, FMSTR_TSA_UINT32)   /* Loop counter */
FMSTR_TSA_RW_VAR(g_sMID.ui16FaultMID, FMSTR_TSA_UINT16)   /* Fault state variable */
FMSTR_TSA_RW_VAR(g_sMID.ui16WarningMID, FMSTR_TSA_UINT16) /* Warning state variable */
FMSTR_TSA_RW_VAR(g_sMID.ui16EnableMeas, FMSTR_TSA_UINT16) /* State variable for measurement enable */

/* g_sMID.sPar definitions */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltPwrN, FMSTR_TSA_FLOAT)    /* Rated motor power */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltIphN, FMSTR_TSA_FLOAT)    /* Rated motor phase current */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltUphN, FMSTR_TSA_FLOAT)    /* Rated motor pahse voltage */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltFreqN, FMSTR_TSA_FLOAT)   /* Rated motor frequency */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltPp, FMSTR_TSA_FLOAT)      /* Motor pole-pair number*/
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltRs, FMSTR_TSA_FLOAT)      /* Motor stator resistance */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltRr, FMSTR_TSA_FLOAT)      /* Motor rotor resistance */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltLs, FMSTR_TSA_FLOAT)      /* Motor stator inductance */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltLr, FMSTR_TSA_FLOAT)      /* Motor rotor inductance */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltLm, FMSTR_TSA_FLOAT)      /* Motor mutual inductance */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltJ, FMSTR_TSA_FLOAT)       /* Motor inertia */
FMSTR_TSA_RW_VAR(g_sMID.sPar.fltTauMech, FMSTR_TSA_FLOAT) /* Motor mechanical time constant */

/* g_sMID.sAlgNew definitions */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltSpdMeReqMin, FMSTR_TSA_FLOAT) /* Speed minimum value for updated structure */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltSpdMeReqMax, FMSTR_TSA_FLOAT) /* Speed maximum value for updated structure */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltIMax, FMSTR_TSA_FLOAT)        /* Maximum motor current */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltIdStart, FMSTR_TSA_FLOAT)     /* Id start-up value */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltIdMin, FMSTR_TSA_FLOAT)       /* Id minimum value for updated structure */
FMSTR_TSA_RW_VAR(g_sMID.sAlgNew.fltIdMax, FMSTR_TSA_FLOAT)       /* Id maximum value for updated structure */

/* g_sMID.sPwrStgCh definitions */
FMSTR_TSA_RW_VAR(g_sMID.sPwrStgCh.fltICal, FMSTR_TSA_FLOAT) /* Calibration current */
FMSTR_TSA_RW_VAR(g_sMID.sPwrStgCh.fltIInc, FMSTR_TSA_FLOAT) /* Current increment */
FMSTR_TSA_RW_MEM(g_sMID.sPwrStgCh.pfltUErrLUT,
                 FMSTR_TSA_FRAC16,
                 &g_sMID.sPwrStgCh.pfltUErrLUT[0],
                 (65 << 1))                                          /* MID TransCharError */
FMSTR_TSA_RW_VAR(g_sMID.sPwrStgCh.fltRs, FMSTR_TSA_FLOAT)            /* Stator resistance */
FMSTR_TSA_RW_VAR(g_sMID.sPwrStgCh.ui16NumOfChPnts, FMSTR_TSA_UINT16) /* Active time counter */
FMSTR_TSA_RW_VAR(g_sMID.sPwrStgCh.ui16Active, FMSTR_TSA_UINT16)      /* Active time counter */

/* g_sMID.sRs definitions */
FMSTR_TSA_RW_VAR(g_sMID.sRs.fltILPF, FMSTR_TSA_FLOAT)     /* Current low-pass filter */
FMSTR_TSA_RW_VAR(g_sMID.sRs.fltIMeas, FMSTR_TSA_FLOAT)    /* Current measured */
FMSTR_TSA_RW_VAR(g_sMID.sRs.ui16Active, FMSTR_TSA_UINT16) /* Active time counter */

/* g_sMID.sNoLoad definitions */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltUMeas, FMSTR_TSA_FLOAT)    /* Measured motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltUMeasMax, FMSTR_TSA_FLOAT) /* Maximum value of motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltFMeas, FMSTR_TSA_FLOAT)    /* Frequency applied during measurement */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltUrms, FMSTR_TSA_FLOAT)     /* Rms value of motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltIrms, FMSTR_TSA_FLOAT)     /* Rms value of motor current */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltP, FMSTR_TSA_FLOAT)        /* Active power */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.fltQ, FMSTR_TSA_FLOAT)        /* Reactive power */
FMSTR_TSA_RW_VAR(g_sMID.sNoLoad.ui16Active, FMSTR_TSA_UINT16) /* Active time counter */

/* g_sMID.sBlocked definitions */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltIrmsMeas, FMSTR_TSA_FLOAT) /* Rms value of measured motor current */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltUMeas, FMSTR_TSA_FLOAT)    /* Rms value of measured motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltFMeas, FMSTR_TSA_FLOAT)    /* Frequency used for measurement */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltUrms, FMSTR_TSA_FLOAT)     /* Rms value of motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltIrms, FMSTR_TSA_FLOAT)     /* Rms value of motor current */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltP, FMSTR_TSA_FLOAT)        /* Active power */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.fltQ, FMSTR_TSA_FLOAT)        /* Reactive power */
FMSTR_TSA_RW_VAR(g_sMID.sBlocked.ui16Active, FMSTR_TSA_UINT16) /* Active time counter */

/* g_sMID.sMech definitions */
FMSTR_TSA_RW_VAR(g_sMID.sMech.eState,
                 FMSTR_TSA_UINT8) /* State variable for mechnical parameter identification routine */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltTrqAccl, FMSTR_TSA_FLOAT)    /* Required torque value */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltIqAccl, FMSTR_TSA_FLOAT)     /* Iq required */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltSpdElEnd, FMSTR_TSA_FLOAT)   /* End point of speed profile */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltSpdElStart, FMSTR_TSA_FLOAT) /* Start-up speed */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltSpdElMax, FMSTR_TSA_FLOAT)   /* Maximum value of electrical speed */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltSpdLPF, FMSTR_TSA_FLOAT)     /* Speed low pass filter */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltTrqLPF, FMSTR_TSA_FLOAT)     /* Torque low pass filter */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltLPFCoeff, FMSTR_TSA_FLOAT)   /* Low pass filter coefficient */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltTrqInteg, FMSTR_TSA_FLOAT)   /* Integration constant */
FMSTR_TSA_RW_VAR(g_sMID.sMech.fltGainTrq, FMSTR_TSA_FLOAT)    /* Torque gain */
FMSTR_TSA_RW_VAR(g_sMID.sMech.ui16Active, FMSTR_TSA_UINT16)   /* Active time counter */

/* g_sMID.sCalcPwr definitions */
FMSTR_TSA_RW_VAR(g_sMID.sCalcPwr.fltUrmsAvg, FMSTR_TSA_FLOAT) /* Rms avarage value of motor voltage */
FMSTR_TSA_RW_VAR(g_sMID.sCalcPwr.fltIrmsAvg, FMSTR_TSA_FLOAT) /* Rms avarage value of motor current */

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
FMSTR_TSA_RW_VAR(bDemoMode, FMSTR_TSA_UINT16)               /* Demo Mode */
FMSTR_TSA_RW_VAR(g_bM1SwitchAppOnOff, FMSTR_TSA_UINT16)     /* Application Switch */
FMSTR_TSA_RW_VAR(g_sM1Ctrl.eState, FMSTR_TSA_UINT16)        /* Application State */

/* global freemaster float variables */
FMSTR_TSA_RW_VAR(g_fltM1currentScale, FMSTR_TSA_FLOAT)    /* FMSTR_M1_currentScale */
FMSTR_TSA_RW_VAR(g_fltM1DCBvoltageScale, FMSTR_TSA_FLOAT) /* FMSTR_M1_DCBvoltageScale */
FMSTR_TSA_RW_VAR(g_fltM1speedScale, FMSTR_TSA_FLOAT)      /* FMSTR_M1_speedScale */
FMSTR_TSA_RW_VAR(g_fltM1voltageScale, FMSTR_TSA_FLOAT)    /* FMSTR_M1_voltageScale */

FMSTR_TSA_RW_VAR(g_eM1StateRun, FMSTR_TSA_UINT8) /* State Run */

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
#if defined(DEMO) /* If demo mode is selected */
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(g_sM1Drive_table)
FMSTR_TSA_TABLE(global_table)
FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE_LIST_END()
#else /* If demo mode is NOT selected */
FMSTR_TSA_TABLE_LIST_BEGIN()
FMSTR_TSA_TABLE(g_sM1Drive_table)
FMSTR_TSA_TABLE(g_sMID_table)
FMSTR_TSA_TABLE(global_table)
FMSTR_TSA_TABLE(sAppIdFM_table)
FMSTR_TSA_TABLE_LIST_END()
#endif
