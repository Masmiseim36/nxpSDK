/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _PMSM_CONTROL_H_
#define _PMSM_CONTROL_H_

#include "mlib.h"
#include "gflib.h"
#include "gmclib.h"
#include "gdflib.h"
#include "amclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief mcs alignment structure */
typedef struct _mcs_alignment
{
    frac16_t f16UdReq;     /* Required D voltage at alignment */
    uint16_t ui16Time;     /* Alignment time duration */
    uint16_t ui16TimeHalf; /* Alignment half time duration */
    frac16_t f16PosAlign;   /* Position for alignment */
    frac16_t f16PosStep1;   /* Alignment position for first step */
    frac16_t f16PosStep2;   /* Alignment position for second step */
    bool_t   bDirection;    /* Motor direction */
} mcs_alignment_t;       /* PMSM simple two-step Ud voltage alignment */

/*! @brief mcs foc structure */
typedef struct _mcs_pmsm_foc
{
    GFLIB_CTRL_PI_P_AW_T_A32 sIdPiParams;     /* Id PI controller parameters */
    GFLIB_CTRL_PI_P_AW_T_A32 sIqPiParams;     /* Iq PI controller parameters */
    GDFLIB_FILTER_IIR1_T_F32 sUDcBusFilter;   /* Dc bus voltage filter */
    GMCLIB_3COOR_T_F16 sIABC;                 /* Measured 3-phase current */
    GMCLIB_2COOR_ALBE_T_F16 sIAlBe;           /* Alpha/Beta current */
    GMCLIB_2COOR_DQ_T_F16 sIDQ;               /* DQ current */
    GMCLIB_2COOR_DQ_T_F16 sIDQReq;            /* DQ required current */
    GMCLIB_2COOR_DQ_T_F16 sIDQError;          /* DQ current error */
    GMCLIB_3COOR_T_F16 sDutyABC;              /* Applied duty cycles ABC */
    GMCLIB_2COOR_ALBE_T_F16 sUAlBeReq;        /* Required Alpha/Beta voltage */
    GMCLIB_2COOR_ALBE_T_F16 sUAlBeComp;       /* Compensated to DC bus Alpha/Beta voltage */
    GMCLIB_2COOR_DQ_T_F16 sUDQReq;            /* Required DQ voltage */
    GMCLIB_2COOR_DQ_T_F16 sUDQEst;            /* BEMF observer input DQ voltages */
    GMCLIB_2COOR_SINCOS_T_F16 sAnglePosEl;    /* Electrical position sin/cos (at the moment of PWM current reading) */
    AMCLIB_BEMF_OBSRV_DQ_T_A32 sBemfObsrv;    /* BEMF observer in DQ */
    AMCLIB_TRACK_OBSRV_T_F32 sTo;             /* Tracking observer */
    GDFLIB_FILTER_IIR1_T_F32 sSpeedElEstFilt; /* Estimated speed filter */
    acc32_t acc32BemfErr;                     /* BEMF observer output */
    frac16_t f16SpeedElEst;                   /* Rotor electrical speed estimated */
    uint16_t ui16SectorSVM;                   /* SVM sector */
    frac16_t f16PosEl;                        /* Electrical position */
    frac16_t f16PosElExt;                     /* Electrical position set from external function - sensor, open loop */
    frac16_t f16PosElEst;                     /* Rotor electrical position estimated*/
    frac16_t f16DutyCycleLimit;               /* Maximum allowable duty cycle in frac */
    frac16_t f16UDcBus;                       /* DC bus voltage */
    frac16_t f16UDcBusFilt;                   /* Filtered DC bus voltage */
    bool_t bCurrentLoopOn;                    /* Flag enabling calculation of current control loop */
    bool_t bPosExtOn;                         /* Flag enabling use of electrical position passed from other functions */
    bool_t bOpenLoop;                         /* Position estimation loop is open */
    bool_t bIdPiStopInteg;                    /* Id PI controller manual stop integration */
    bool_t bIqPiStopInteg;                    /* Iq PI controller manual stop integration */
} mcs_pmsm_foc_t;

/*! @brief mcs scalar structure */
typedef struct _mcs_pmsm_scalar_ctrl
{
    GFLIB_RAMP_T_F32 sFreqRampParams;       /* Parameters of frequency ramp */
    GMCLIB_2COOR_DQ_T_F16 sUDQReq;          /* Required voltage vector in d,q coordinates */
    GFLIB_INTEGRATOR_T_A32 sFreqIntegrator; /* structure contains the integrator parameters (integrates the omega in
                                               order to get the position */
    frac16_t f16FreqCmd;                    /* Required electrical frequency from master system */
    frac16_t f16FreqRamp;                   /* Required frequency limited by ramp - the ramp output */
    frac16_t f16PosElScalar;                /* Electrical angle of the rotor */
    frac16_t f16VHzGain;                    /* VHz_factor constant gain for scalar control */
    int16_t f16VHzGainShift;                /* VHz_factor constant shift for scalar control */
} mcs_pmsm_scalar_ctrl_t;

/*! @brief mcs scalar structure */
typedef struct _mcs_speed
{
    GDFLIB_FILTER_IIR1_T_F32 sSpeedFilter;   /* Speed filter */
    GFLIB_CTRL_PI_P_AW_T_A32 sSpeedPiParams; /* Speed PI controller parameters */
    GFLIB_RAMP_T_F32 sSpeedRampParams;       /* Speed ramp parameters */
    frac16_t f16Speed;                       /* Speed */
    frac16_t f16SpeedFilt;                   /* Speed filtered */
    frac16_t f16SpeedError;                  /* Speed error */
    frac16_t f16SpeedRamp;                   /* Required speed (ramp output) */
    frac16_t f16SpeedCmd;                    /* Speed command (entered by user or master layer) */
    frac16_t f16IqReq;                       /* Output of ASR */
    bool_t bSpeedPiStopInteg;                /* Speed PI controller saturation flag */
    bool_t bIqPiLimFlag;                     /* Saturation flag of Iq controller */
} mcs_speed_t;

/*! @brief mcs mcat control structure */
typedef struct _mcs_mcat_ctrl
{
    GMCLIB_2COOR_DQ_T_F16 sIDQReqMCAT; /* required dq current entered from MCAT tool */
    GMCLIB_2COOR_DQ_T_F16 sUDQReqMCAT; /* required dq voltage entered from MCAT tool */
    uint16_t ui16PospeSensor;          /* position sensor type information */
} mcs_mcat_ctrl_t;

/*! @brief mcs pmsm startup structure */
typedef struct _mcs_pmsm_startup
{
    GFLIB_INTEGRATOR_T_A32 sSpeedIntegrator;   /* Speed integrator structure */
    GFLIB_RAMP_T_F32 sSpeedRampOpenLoopParams; /* Parameters of startup speed ramp */
    frac16_t f16SpeedReq;                      /* Required speed */
    frac16_t f16PosEst;                        /* Fractional electrical position */
    frac16_t f16SpeedRampOpenLoop;             /* Open loop startup speed ramp */
    frac16_t f16CoeffMerging;                  /* increment of merging weight for position merging */
    frac16_t f16RatioMerging;                  /* merging weight coefficient */
    frac16_t f16PosGen;                        /* generated open loop position from the speed ramp integration */
    frac16_t f16PosMerged;                     /* merged position */
    frac16_t f16SpeedCatchUp;                  /* merging speed threshold */
    frac16_t f16CurrentStartup;                /* required Iq current during open loop start up */
    uint16_t ui16TimeStartUpFreeWheel; 		   /* Free-wheel duration if start-up aborted by user input (required zero speed) */
    bool_t bOpenLoop;                          /* Position estimation loop is open */
} mcs_pmsm_startup_t; 

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @name Motor control PMSM  functions
 * @{
 */

/*******************************************************************************
 * API
 ******************************************************************************/

/*!
* @brief PMSM field oriented current control.
*
* This function is used to compute PMSM field oriented current control.
*
* @param psFocPMSM     The pointer of the PMSM FOC structure
*
* @return None
*/
void MCS_PMSMFocCtrl(mcs_pmsm_foc_t *psFocPMSM);

/*!
 * @brief PMSM field oriented speed control.
 *
 * This function is used to compute PMSM field oriented speed control.
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 * @param psSpeed       The pointer of the PMSM speed structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlSpeed(mcs_speed_t *psSpeed);

/*!
 * @brief PMSM 2-step rotor alignment - 120deg in first step and 0deg in second.
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psFocPMSM     The pointer of the PMSM FOC structure
 * @param psAlignment   The pointer of the motor control alignment structure
 *
 * @return None
 */
void MCS_PMSMAlignment(mcs_alignment_t *psAlignment);

/*!
 * @brief PMSM Open Loop Start-up
 *
 * This function is used to PMSM Open Loop Start-up
 *
 * @param psStartUp     The pointer of the PMSM open loop start up parameters structure
 *
 * @return None
 */
void MCS_PMSMOpenLoopStartUp(mcs_pmsm_startup_t *psStartUp);

/*!
 * @brief PMSM scalar control, voltage is set based on required speed
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
 * @param psFocPMSM      The pointer of the PMSM FOC structure
 * @param psScalarPMSM   The pointer of the PMSM scalar control structure
 *
 * @return None
 */
void MCS_PMSMScalarCtrl(mcs_pmsm_scalar_ctrl_t *psScalarPMSM);

#ifdef __cplusplus
}
#endif

#endif /* _PMSM_CONTROL_H_ */

