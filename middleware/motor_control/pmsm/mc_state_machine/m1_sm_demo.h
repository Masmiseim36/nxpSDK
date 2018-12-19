/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _M1_STATEMACHINE_H_
#define _M1_STATEMACHINE_H_

#include "m1_pmsm_appconfig.h"
#include "state_machine.h"
#include "pmsm_control.h"

/* library headers */
#include "gmclib.h"
#include "gflib.h"
#include "gdflib.h"
#include "amclib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MC_FAULT_I_DCBUS_OVER 0  /* OverCurrent fault flag */
#define MC_FAULT_U_DCBUS_UNDER 1 /* Undervoltage fault flag */
#define MC_FAULT_U_DCBUS_OVER 2  /* Overvoltage fault flag */
#define MC_FAULT_LOAD_OVER 3     /* Overload fault flag */
#define MC_FAULT_SPEED_OVER 4    /* Over speed fault flag */
#define MC_FAULT_ROTOR_BLOCKED 5 /* Blocked rotor fault flag */

/* Sets the fault bit defined by faultid in the faults variable */
#define MC_FAULT_SET(faults, faultid) (faults |= ((mcdef_fault_t)1 << faultid))

/* Clears the fault bit defined by faultid in the faults variable */
#define MC_FAULT_CLEAR(faults, faultid) (faults &= ~((mcdef_fault_t)1 << faultid))

/* Check the fault bit defined by faultid in the faults variable, returns 1 or 0 */
#define MC_FAULT_CHECK(faults, faultid) ((faults & ((mcdef_fault_t)1 << faultid)) >> faultid)

/* Clears all fault bits in the faults variable */
#define MC_FAULT_CLEAR_ALL(faults) (faults = 0)

/* Check if a fault bit is set in the faults variable, 0 = no fault */
#define MC_FAULT_ANY(faults) (faults > 0)

/* Update a fault bit defined by faultid in the faults variable according to the LSB of value */
#define MC_FAULT_UPDATE(faults, faultid, value)                     \
    {                                                               \
        MC_FAULT_CLEAR(faults, faultid);                            \
        faults |= (((MC_FAULT_T)value & (MC_FAULT_T)1) << faultid); \
    }

/*! @brief device fault typedef */
typedef uint16_t mcdef_fault_t;

/*! @brief States of machine enumeration */
typedef enum _m1_run_substate_t
{
    kRunState_Calib = 0,
    kRunState_Ready = 1,
    kRunState_Align = 2,
    kRunState_Startup = 3,
    kRunState_Spin = 4,
    kRunState_Freewheel = 5,
} m1_run_substate_t; /* Run sub-states */

/*! @brief Device fault thresholds */
typedef struct _mcdef_fault_thresholds_t
{
    frac16_t f16UDcBusOver;     /* DC bus over voltage level */
    frac16_t f16UDcBusUnder;    /* DC bus under voltage level */
    frac16_t f16SpeedOver;      /* Over speed level */
    frac16_t f16SpeedMin;       /* Minimum speed level */
    frac16_t f16SpeedNom;       /* Nominal speed */
    frac16_t f16UqBemf;         /* Blocked rotor U level */
    uint16_t ui16BlockedPerNum; /* Number of period to set blocked rotor fault */
} mcdef_fault_thresholds_t;

/*! @brief PMSM FOC with BEMF observer in DQ */
typedef struct _mcdef_pmsm_t
{
    mcs_pmsm_foc_t sFocPMSM;                       /* Field Oriented Control structure */
    mcs_speed_t sSpeed;                            /* Speed control loop structure  */
    mcs_pmsm_startup_t sStartUp;                   /* Open loop start-up */
    mcs_alignment_t sAlignment;                    /* PMSM simple two-step Ud voltage alignment */
    mcdef_fault_t sFaultIdCaptured;                /* Captured faults (must be cleared manually) */
    mcdef_fault_t sFaultIdPending;                 /* Fault pending structure */
    mcdef_fault_thresholds_t sFaultThresholds;     /* Fault thresholds */
    GDFLIB_FILTER_MA_T_A32 msM1BlockedRotorUqFilt; /* Blocked rotor detection filter */
    frac16_t f16AdcAuxSample;                      /* Auxiliary ADC sample  */
    uint16_t ui16CounterState;                     /* Main state counter */
    uint16_t ui16TimeSlowLoop;                     /* Slow control loop time count number */
    uint16_t ui16TimeFullSpeedFreeWheel;           /* Free-wheel time count number */
    uint16_t ui16TimeCalibration;                  /* Calibration time count number */
    uint16_t ui16TimeFaultRelease;                 /* Fault time count number */
    frac16_t f16BemfUqAvg;                         /* Blocked rotor filter output */
    uint16_t ui16BlockRotorCnt;                    /* Blocked rotor fault counter */
    bool_t bFaultClearMan;                         /* Manual fault clear detection */
} mcdef_pmsm_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_pmsm_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;

extern volatile float g_fltM1voltageScale;
extern volatile float g_fltM1DCBvoltageScale;
extern volatile float g_fltM1currentScale;
extern volatile float g_fltM1frequencyScale;
extern volatile float g_fltM1speedScale;

#ifdef __cplusplus
extern "C" {
#endif

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
void M1_SetAppSwitch(bool_t bValue);

/*!
 * @brief Get application switch value
 *
 * @param void  No input parameter
 *
 * @return bool_t Return bool value, true or false
 */
bool_t M1_GetAppSwitch(void);

/*!
 * @brief Get application state
 *
 * @param void  No input parameter
 *
 * @return uint16_t Return current application state
 */
uint16_t M1_GetAppState(void);

/*!
 * @brief Set spin speed of the motor in fractional value
 *
 * @param f16SpeedCmd  Speed command - set speed
 *
 * @return None
 */
void M1_SetSpeed(frac16_t f16SpeedCmd);

/*!
 * @brief Get spin speed of the motor in fractional value
 *
 * @param void  No input parameter
 *
 * @return frac16_t Fractional value of the current speed
 */
frac16_t M1_GetSpeed(void);

#ifdef __cplusplus
}
#endif

#endif /* STATEMACHINE */

