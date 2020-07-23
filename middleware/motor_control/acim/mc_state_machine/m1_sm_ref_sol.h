/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _M1_SM_REF_SOL_H_
#define _M1_SM_REF_SOL_H_

#include "m1_acim_appconfig.h"
#include "state_machine.h"
#include "acim_control.h"

/* library headers */
#include "gmclib.h"
#include "gflib.h"
#include "gdflib.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define M1_FAULT_I_DCBUS_OVER 0  /* OverCurrent fault flag */
#define M1_FAULT_U_DCBUS_UNDER 1 /* Undervoltage fault flag */
#define M1_FAULT_U_DCBUS_OVER 2  /* Overvoltage fault flag */
#define M1_FAULT_SPEED_OVER 3    /* Over speed fault flag */

/* Sets the fault bit defined by faultid in the faults variable */
#define M1_FAULT_SET(faults, faultid) (faults |= ((mcdef_fault_t)1 << faultid))

/* Clears the fault bit defined by faultid in the faults variable */
#define M1_FAULT_CLEAR(faults, faultid) (faults &= ~((mcdef_fault_t)1 << faultid))

/* Check the fault bit defined by faultid in the faults variable, returns 1 or 0 */
#define M1_FAULT_CHECK(faults, faultid) ((faults & ((mcdef_fault_t)1 << faultid)) >> faultid)

/* Clears all fault bits in the faults variable */
#define M1_FAULT_CLEAR_ALL(faults) (faults = 0)

/* Check if a fault bit is set in the faults variable, 0 = no fault */
#define M1_FAULT_ANY(faults) (faults > 0)

/*! @brief device fault typedef */
typedef uint16_t mcdef_fault_t;

/*! @brief States of machine enumeration */
typedef enum _m1_run_substate_t
{
    kRunState_Calib     = 0,
    kRunState_Ready     = 1,
    kRunState_Startup   = 2,
    kRunState_Spin      = 3,
    kRunState_Freewheel = 4,
    kRunState_Measure   = 5,
} m1_run_substate_t; /* Run sub-states */

/*! @brief Control modes of the motor */
typedef enum _m1_ctrl_mode_t
{
    kControlMode_Scalar     = 0,
    kControlMode_VoltageFOC = 1,
    kControlMode_CurrentFOC = 2,
    kControlMode_SpeedFOC   = 3,
} m1_ctrl_mode_t;

/*! @brief Device fault thresholds */
typedef struct _mcdef_fault_thresholds_t
{
    float_t fltUDcBusOver;  /* DC-bus over voltage level */
    float_t fltUDcBusUnder; /* DC-bus under voltage level */
    float_t fltUDcBusTrip;  /* DC bus voltage level to start braking */
    float_t fltSpeedOver;   /* Over-speed level */
} mcdef_fault_thresholds_t;

/*! @brief ACIM FOC with RFO observer */
typedef struct _mcdef_acim_t
{
    mcs_acim_foc_a1_t sFocACIM;                /* field Oriented Control structure */
    mcs_speed_flux_a1_t sSpdFlux;              /* Speed and rotor flux control structure */
    mcs_scalar_a1_t sScalarCtrl;               /* Scalar control structure */
    mcdef_fault_t sFaultIdCaptured;            /* Captured faults (must be cleared manually) */
    mcdef_fault_t sFaultIdPending;             /* Fault pending structure */
    mcdef_fault_thresholds_t sFaultThresholds; /* Fault thresholds */
    m1_ctrl_mode_t eControl;                   /* MCAT control modes */
    int32_t ui32CounterState;                  /* Main state counter */
    int32_t ui32TimeFullSpeedFreeWheel;        /* Freewheel time in count of fast loop calls */
    int32_t ui32TimeCalibration;               /* Calibration time in count of fast loop calls */
    int32_t ui32TimeFaultRelease;              /* Fault time in count of fast loop calls */
    frac16_t f16AdcAuxSample;                  /* Auxiliary ADC sample  */
    float_t fltIPMTemperature;                 /* IPM temperature in Celsius degrees */
    bool_t bFaultClearMan;                     /* Manual fault clear detection */
    float_t fltTsFastLoop;                     /* Fast loop sampling time */
    float_t fltTsSlowLoop;                     /* Slow loop sampling time */
} mcdef_acim_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool_t g_bM1SwitchAppOnOff;
extern mcdef_acim_t g_sM1Drive;
extern sm_app_ctrl_t g_sM1Ctrl;
extern m1_run_substate_t g_eM1StateRun;

extern volatile float g_fltM1voltageScale;
extern volatile float g_fltM1DCBvoltageScale;
extern volatile float g_fltM1currentScale;
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
void M1_SetSpeed(float_t fltSpeedCmd);

/*!
 * @brief Get spin speed of the motor in fractional value
 *
 * @param void  No input parameter
 *
 * @return frac16_t Fractional value of the current speed
 */
float_t M1_GetSpeed(void);

#ifdef __cplusplus
}
#endif

#endif /* _M1_SM_REF_SOL_H_ */
