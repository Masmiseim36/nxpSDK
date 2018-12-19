/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#ifndef _M1_STATEMACHINE_H_
#define _M1_STATEMACHINE_H_

#include "m1_bldc_appconfig.h"
#include "state_machine.h"
#include "bldc_control.h"

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
    frac16_t f16IDcBusOver;  /* DC bus over current level */
    frac16_t f16UDcBusOver;  /* DC bus over voltage level */
    frac16_t f16UDcBusUnder; /* DC bus under voltage level */
} mcdef_fault_thresholds_t;

/*! @brief BLDC sensorless with BEMF integration method */
typedef struct _mcdef_bldc_t
{
    mcs_bldc_ctrl_t sCtrlBLDC;                 /* Main BLDC control structure */
    mcdef_fault_t sFaultId;                    /* Application motor faults */
    mcdef_fault_t sFaultIdPending;             /* Fault pending structure */
    mcdef_fault_thresholds_t sFaultThresholds; /* Fault thresholds */
    uint16_t ui16PeriodCmt[6];                 /* commutation periods */
    uint16_t ui16TimeCurrent;                  /* current time */
    uint16_t ui16TimeCurrentEvent;             /* time of current event */
    uint16_t ui16TimeNextEvent;                /* time of next event */
    uint16_t ui16TimeOfCmt;                    /* current commutation time */
    uint16_t ui16TimeOfCmtOld;                 /* previous commutation time */
    uint16_t ui16PeriodCmtNext;                /* next commutation period */
    uint16_t ui16PeriodToff;                   /* Toff period */
    uint16_t ui16CounterStartCmt;              /* startup commutations counter */
    uint16_t ui16CounterCmtError;              /* commutation error counter */
    frac32_t f32UBemfIntegSum;                 /* BEMF integrator */
    frac32_t f32UBemfIntegThreshold;           /* BEMF integration threshold */
    bool_t bCommutatedSnsless;                 /* commutated by sensorless algorithm flag */
    uint16_t ui16PeriodFreewheelLong;          /* long free-wheel period */
    uint16_t ui16PeriodFreewheelShort;         /* short free-wheel period */
    uint16_t ui16TimeAlignment;                /* alignment period */
    uint16_t ui16TimeCalibration;              /* Calibration time count number */
    uint16_t ui16TimeFaultRelease;             /* Fault time count number */
    uint16_t ui16CounterState;                 /* Main state counter */
    bool_t bFaultClearMan;                     /* Manual fault clear detection */
    frac16_t f16StartCmtAcceleration;          /* Startup commutation acceleration init value*/
    uint16_t ui16StartCmtNumber;               /* Startup commutation counter init value */
    uint16_t ui16PeriodCmtNextInit;            /* Next commutation period init value */
    uint16_t ui16PeriodToffInit;               /* Toff period init value */
    uint16_t ui16Aux;                          /* auxiliary quantity measured value */
    uint16_t ui16FreqCtrlLoop;                 /* Pass defined loop frequency to FreeMASTER */
    uint32_t ui32FreqCmtTimer;                 /* Pass defined commutation timer frequency to FreeMASTER */
    uint32_t ui16FreqPwm;                      /* Pass defined PWM frequency to FreeMASTER */

} mcdef_bldc_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_bldc_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;

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

/*!
 * @brief Forced commutation if regular commutation not detected using BEMF method
 *
 * @param void  No input parameter
 *
 * @return No return value
 */
void M1_TimeEvent(void);

#ifdef __cplusplus
}
#endif

#endif /* STATEMACHINE */

