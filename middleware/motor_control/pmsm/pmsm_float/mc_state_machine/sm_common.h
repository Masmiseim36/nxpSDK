/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
 
#ifndef _SM_REF_SOL_COMM_H_
#define _SM_REF_SOL_COMM_H_

#include "pmsm_control.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*! @brief device fault typedef */
typedef uint16_t mcdef_fault_t;

/*! @brief States of machine enumeration */
typedef enum _run_substate_t
{
    kRunState_Calib = 0,
    kRunState_Ready = 1,
    kRunState_Align = 2,
    kRunState_Startup = 3,
    kRunState_Spin = 4,
    kRunState_Freewheel = 5,
    kRunState_Measure = 6,
} run_substate_t; /* Run sub-states */

/*! @brief Control modes of the motor */
typedef enum _mcs_ctrl_mode_t
{
    kControlMode_Scalar = 0,
    kControlMode_VoltageFOC = 1,
    kControlMode_CurrentFOC = 2,
    kControlMode_SpeedFOC = 3,
} mcs_ctrl_mode_t;

/*! @brief Device fault thresholds */
typedef struct _mcdef_fault_thresholds_t
{
    float_t fltUDcBusOver;     /* DC bus over voltage level */
    float_t fltUDcBusUnder;    /* DC bus under voltage level */
    float_t fltUDcBusTrip;     /* DC bus voltage level to start braking */
    float_t fltSpeedOver;      /* Over speed level */
    float_t fltSpeedMin;       /* Minimum speed level */
    float_t fltSpeedNom;       /* Nominal speed */
    float_t fltUqBemf;         /* Blocked rotor U level */
    uint16_t ui16BlockedPerNum; /* Number of period to set blocked rotor fault */
} mcdef_fault_thresholds_t;

/*! @brief PMSM FOC with BEMF observer in DQ */
typedef struct _mcdef_pmsm_t
{
    mcs_pmsm_foc_t sFocPMSM;                    /* Field Oriented Control structure */
    mcs_speed_t sSpeed;                         /* Speed control loop structure */
    mcs_pmsm_startup_t sStartUp;                /* Open loop start-up */
    mcs_alignment_t sAlignment;                 /* PMSM simple two-step Ud voltage alignment */
    mcs_mcat_ctrl_t sMCATctrl;                  /* Structure containing control variables directly updated from MCAT */
    mcs_pmsm_scalar_ctrl_t sScalarCtrl;         /* Scalar control structure */
    mcdef_fault_t sFaultIdCaptured;                /* Captured faults (must be cleared manually) */
    mcdef_fault_t sFaultIdPending;                 /* Fault pending structure */
    mcdef_fault_thresholds_t sFaultThresholds;     /* Fault thresholds */
    mcs_ctrl_mode_t eControl;                      /* MCAT control modes */
    GDFLIB_FILTER_MA_T_FLT msM1BlockedRotorUqFilt; /* Blocked rotor detection filter */
    GDFLIB_FILTER_MA_T_FLT msM2BlockedRotorUqFilt; /* Blocked rotor detection filter */
    frac16_t f16AdcAuxSample;                      /* Auxiliary ADC sample  */
    uint16_t ui16CounterState;                     /* Main state counter */
    uint16_t ui16TimeFullSpeedFreeWheel;           /* Free-wheel time count number */
    uint16_t ui16TimeCalibration;                  /* Calibration time count number */
    uint16_t ui16TimeFaultRelease;                 /* Fault time count number */
    float_t fltBemfUqAvg;                          /* Blocked rotor filter output */
    uint16_t ui16BlockRotorCnt;                    /* Blocked rotor fault counter */
    uint16_t ui16FastCtrlLoopFreq;                 /* Pass fast loop frequency to FreeMASTER */
    uint16_t ui16SlowCtrlLoopFreq;                 /* Pass slow loop frequency to FreeMASTER */
    bool_t bFaultClearMan;                         /* Manual fault clear detection */
} mcdef_pmsm_t;

#define FAULT_I_DCBUS_OVER 0  /* OverCurrent fault flag */
#define FAULT_U_DCBUS_UNDER 1 /* Undervoltage fault flag */
#define FAULT_U_DCBUS_OVER 2  /* Overvoltage fault flag */
#define FAULT_LOAD_OVER 3     /* Overload fault flag */
#define FAULT_SPEED_OVER 4    /* Over speed fault flag */
#define FAULT_ROTOR_BLOCKED 5 /* Blocked rotor fault flag */

/* Sets the fault bit defined by faultid in the faults variable */
#define FAULT_SET(faults, faultid) (faults |= ((mcdef_fault_t)1 << faultid))

/* Clears the fault bit defined by faultid in the faults variable */
#define FAULT_CLEAR(faults, faultid) (faults &= ~((mcdef_fault_t)1 << faultid))

/* Check the fault bit defined by faultid in the faults variable, returns 1 or 0 */
#define FAULT_CHECK(faults, faultid) ((faults & ((mcdef_fault_t)1 << faultid)) >> faultid)

/* Clears all fault bits in the faults variable */
#define FAULT_CLEAR_ALL(faults) (faults = 0)

/* Check if a fault bit is set in the faults variable, 0 = no fault */
#define FAULT_ANY(faults) (faults > 0)

/*******************************************************************************
 * Variables
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * API
 ******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif /* _SM_REF_SOL_COMM_H_ */

