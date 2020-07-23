/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _PMSM_CONTROL_H_
#define _PMSM_CONTROL_H_

#include "char_pwrstg.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "gdflib_FP.h"
#include "amclib_FP.h"
#include "mlib.h"
#include "gflib.h"
#include "amclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief mcs alignment structure */
typedef struct _mcs_alignment_a1
{
    float_t fltUdReq;      /* Required D voltage at alignment */
    uint16_t ui16Time;     /* Alignment time duration */
    uint16_t ui16TimeHalf; /* Alignment half time duration */
    frac16_t f16PosAlign;  /* Position for alignment */
} mcs_alignment_t;         /* PMSM simple two-step Ud voltage alignment */

/*! @brief mcs foc structure */
typedef struct _mcs_pmsm_foc_a1
{
    GFLIB_CTRL_PI_P_AW_T_FLT sIdPiParams;     /* Id PI controller parameters */
    GFLIB_CTRL_PI_P_AW_T_FLT sIqPiParams;     /* Iq PI controller parameters */
    GDFLIB_FILTER_IIR1_T_FLT sUDcBusFilter;   /* Dc bus voltage filter */
    GMCLIB_3COOR_T_F16 sIABCFrac;             /* Measured 3-phase current (FRAC)*/
    GMCLIB_3COOR_T_FLT sIABC;                 /* Measured 3-phase current [A] */
    GMCLIB_2COOR_ALBE_T_FLT sIAlBe;           /* Alpha/Beta current */
    GMCLIB_2COOR_DQ_T_FLT sIDQ;               /* DQ current */
    GMCLIB_2COOR_DQ_T_FLT sIDQReq;            /* DQ required current */
    GMCLIB_2COOR_DQ_T_FLT sIDQError;          /* DQ current error */
    GMCLIB_3COOR_T_F16 sDutyABC;              /* Applied duty cycles ABC */
    GMCLIB_2COOR_ALBE_T_FLT sUAlBeReq;        /* Required Alpha/Beta voltage */
    GMCLIB_2COOR_ALBE_T_F16 sUAlBeCompFrac;   /* Compensated to DC bus Alpha/Beta voltage (FRAC) */
    GMCLIB_2COOR_ALBE_T_FLT sUAlBeDTComp;     /* Alpha/Beta stator voltage  */
    GMCLIB_2COOR_DQ_T_FLT sUDQReq;            /* Required DQ voltage */
    GMCLIB_2COOR_DQ_T_FLT sUDQEst;            /* BEMF observer input DQ voltages */
    GMCLIB_2COOR_SINCOS_T_FLT sAnglePosEl;    /* Electrical position sin/cos (at the moment of PWM current reading) */
    AMCLIB_BEMF_OBSRV_DQ_T_FLT sBemfObsrv;    /* BEMF observer in DQ */
    AMCLIB_TRACK_OBSRV_T_FLT sTo;             /* Tracking observer */
    GDFLIB_FILTER_IIR1_T_FLT sSpeedElEstFilt; /* Estimated speed filter */
    acc32_t acc32BemfErr;                     /* BEMF observer output */
    float_t fltSpeedElEst;                    /* Rotor electrical speed estimated */
    uint16_t ui16SectorSVM;                   /* SVM sector */
    frac16_t f16PosEl;                        /* Electrical position */
    frac16_t f16PosElExt;                     /* Electrical position set from external function - sensor, open loop */
    frac16_t f16PosElEst;                     /* Rotor electrical position estimated*/
    float_t fltDutyCycleLimit;                /* Maximum allowable duty cycle in frac */
    float_t fltUDcBus;                        /* DC bus voltage */
    frac16_t f16UDcBus;                       /* DC bus voltage */
    float_t fltUDcBusFilt;                    /* Filtered DC bus voltage */
    float_t fltPwrStgCharIRange;              /* Power Stage characteristic current range */
    float_t fltPwrStgCharLinCoeff;            /* Power Stage characteristic linear coefficient */
    bool_t bCurrentLoopOn;                    /* Flag enabling calculation of current control loop */
    bool_t bPosExtOn;                         /* Flag enabling use of electrical position passed from other functions */
    bool_t bOpenLoop;                         /* Position estimation loop is open */
    bool_t bIdPiStopInteg;                    /* Id PI controller manual stop integration */
    bool_t bIqPiStopInteg;                    /* Iq PI controller manual stop integration */
    bool_t bFlagDTComp;                       /* Enable/disable dead-time compensation flag */
} mcs_pmsm_foc_t;

/*! @brief mcs scalar structure */
typedef struct _mcs_pmsm_scalar_ctrl_a1
{
    GFLIB_RAMP_T_FLT sFreqRampParams;       /* Parameters of frequency ramp */
    GMCLIB_2COOR_DQ_T_FLT sUDQReq;          /* Required voltage vector in d,q coordinates */
    GFLIB_INTEGRATOR_T_A32 sFreqIntegrator; /* structure contains the integrator parameters (integrates the omega in
                                               order to get the position */
    float_t fltFreqCmd;                     /* Required electrical frequency from master system */
    float_t fltFreqMax;                     /* Frequency maximum scale calculated from speed max */
    float_t fltFreqRamp;                    /* Required frequency limited by ramp - the ramp output */
    frac16_t f16PosElScalar;                /* Electrical angle of the rotor */
    float_t fltVHzGain;                     /* VHz_factor constant gain for scalar control */
    float_t fltUqMin;                       /* Minimal Uq voltage for scalar control*/
} mcs_pmsm_scalar_ctrl_t;

/*! @brief mcs scalar structure */
typedef struct _mcs_speed_a1
{
    GDFLIB_FILTER_IIR1_T_FLT sSpeedFilter;   /* Speed filter */
    GFLIB_CTRL_PI_P_AW_T_FLT sSpeedPiParams; /* Speed PI controller parameters */
    GFLIB_RAMP_T_FLT sSpeedRampParams;       /* Speed ramp parameters */
    float_t fltSpeed;                        /* Speed */
    float_t fltSpeedFilt;                    /* Speed filtered */
    float_t fltSpeedError;                   /* Speed error */
    float_t fltSpeedRamp;                    /* Required speed (ramp output) */
    float_t fltSpeedCmd;                     /* Speed command (entered by user or master layer) */
    float_t fltIqReq;                        /* Output of ASR */
    bool_t bSpeedPiStopInteg;                /* Speed PI controller saturation flag */
    bool_t bIqPiLimFlag;                     /* Saturation flag of Iq controller */
} mcs_speed_t;

/*! @brief mcs position structure */
typedef struct _mcs_position_a1
{
    frac16_t f16PositionPGain; /* Position P gain */
    acc32_t a32Position;       /* Position */
    acc32_t a32PositionError;  /* Position error */
    acc32_t a32PositionCmd;    /* Position command (entered by user or master layer) */
    frac16_t f16SpeedReq;      /* Output from P position controller in fraction */
} mcs_position_t;

/*! @brief mcs mcat control structure */
typedef struct _mcs_mcat_ctrl_a1
{
    GMCLIB_2COOR_DQ_T_FLT sIDQReqMCAT; /* required dq current entered from MCAT tool */
    GMCLIB_2COOR_DQ_T_FLT sUDQReqMCAT; /* required dq voltage entered from MCAT tool */
    uint16_t ui16PospeSensor;          /* position sensor type information */
} mcs_mcat_ctrl_t;

/*! @brief mcs pmsm startup structure */
typedef struct _mcs_pmsm_startup_a1
{
    GFLIB_INTEGRATOR_T_A32 sSpeedIntegrator;   /* Speed integrator structure */
    GFLIB_RAMP_T_FLT sSpeedRampOpenLoopParams; /* Parameters of startup speed ramp */
    float_t fltSpeedReq;                       /* Required speed */
    float_t fltSpeedMax;                       /* Maximum speed scale */
    frac16_t f16PosEst;                        /* Fractional electrical position */
    float_t fltSpeedRampOpenLoop;              /* Open loop startup speed ramp */
    frac16_t f16CoeffMerging;                  /* increment of merging weight for position merging */
    frac16_t f16RatioMerging;                  /* merging weight coefficient */
    frac16_t f16PosGen;                        /* generated open loop position from the speed ramp integration */
    frac16_t f16PosMerged;                     /* merged position */
    float_t fltSpeedCatchUp;                   /* merging speed threshold */
    float_t fltCurrentStartup;                 /* required Iq current during open loop start up */
    uint16_t ui16TimeStartUpFreeWheel; /* Free-wheel duration if start-up aborted by user input (required zero speed) */
    bool_t bOpenLoop;                  /* Position estimation loop is open */
} mcs_pmsm_startup_t;

/* dead-time compensation voltage table */
extern float_t pfltUDtComp[DTCOMP_TABLE_SIZE];

/* dead-time compensation look-up table */
extern GFLIB_LUT1D_T_FLT sLUTUDtComp;

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
 * @param psSpeed       The pointer of the PMSM speed structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlSpeed(mcs_speed_t *psSpeed);

/*!
 * @brief PMSM field oriented position control.
 *
 * This function is used to compute PMSM field oriented position control.
 *
 * @param psSpeed       The pointer of the PMSM position structure
 *
 * @return None
 */
void MCS_PMSMFocCtrlPosition(mcs_position_t *psPosition);

/*!
 * @brief PMSM 2-step rotor alignment - 120deg in first step and 0deg in second.
 *
 * This function is used for alignment rotor in two steps - 120deg in first step and 0deg in second
 *
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
 * @param psScalarPMSM   The pointer of the PMSM scalar control structure
 *
 * @return None
 */
void MCS_PMSMScalarCtrl(mcs_pmsm_scalar_ctrl_t *psScalarPMSM);

#ifdef __cplusplus
}
#endif

#endif /* _PMSM_CONTROL_H_ */
