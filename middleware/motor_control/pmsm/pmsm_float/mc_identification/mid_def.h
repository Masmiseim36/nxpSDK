/*
* Copyright 2020, 2024 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_pmsm_appconfig.h"
#include "mc_periph_init.h"

/* RTCESL fix libraries. */
#include "mlib.h"
#include "gflib.h"
#include "gdflib.h"
#include "gmclib.h"
#include "amclib.h"

/* RTCESL float libraries. */
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gdflib_FP.h"
#include "gmclib_FP.h"
#include "amclib_FP.h"

#include "mcaa_lib_fp.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MID common parameters. */
#define I_NOMINAL 	      M1_I_PH_NOM                 /* AP MID - Nominal current [A]. */
#define F_SAMPLING 		    M1_FAST_LOOP_FREQ           /* AP MID - Fast loop sampling frequency [Hz] */

/* Pp Assist measurement parameters. */
#define I_PP_ASSIST       I_NOMINAL * 0.2F            /* AP MID - Current for pole-pair assistant measurement [A] */
#define F_EL_REQ          N_NOMINAL / 60.0F * 0.1F    /* AP MID - Electrical Speed [rpm] -> [Hz] */

/* RL Estim measurement parameters. */ 
#define NUM_MEAS 		      20U                         /* AP MID - Number of measurement. */
#define I_RL_ESTIM        I_NOMINAL  * 0.5F           /* AP MID - Current for electrical parameters measurement [A] */
#define I_POSMAX 		      I_NOMINAL  * 1.1F           /* AP MID - Maximum positive current [A]. */
#define I_NEGMAX 		      -I_NOMINAL * 1.1F           /* AP MID - Minimum positive current [A]. */
#define I_LD			        0.0F                        /* AP MID - Current to determine inductance in d-axis [A]. */
#define I_LQ			        I_NOMINAL                   /* AP MID - Current to determine inductance in q-axis [A]. */
#define ESTIMRL_TIMEOUT   30.0F                       /* AP MID - Estimation timeout time [s]. */

/* BJ Estim measurement parameters. */ 
#define I_BJ_ESTIM        I_NOMINAL                   /* AP MID - Current for mechanical parameters measurement [A] */
#define N_NOMINAL     M1_N_NOM * M1_N_ANGULAR_MAX     /* AP MID - Nominal speed [rpm]. */       

/* MID measurements faults flags. */
#define MID_START_SUCCESSFUL  (0UL)
#define MID_PP_INIT_FAIL      (1UL << 0U)
#define MID_PP_MEAS_FAIL      (1UL << 1U)
#define MID_RL_INIT_FAIL      (1UL << 2U)
#define MID_RL_ESTIM_FAIL     (1UL << 3U)
#define MID_BJ_INIT_FAIL      (1UL << 4U)
#define MID_BJ_ESTIM_FAIL     (1UL << 5U)

/* MID measurements finished flags. */
#define MID_PP_FINISH   (1UL << 0U)
#define MID_RL_FINISH   (1UL << 1U)
#define MID_BJ_FINISH   (1UL << 2U)

/* Sets the fault bit defined by faultid in the faults variable */
#define MID_FAULT_SET(faults, faultid) ((faults) |= ((middef_fault_t)1 << (faultid)))

/* Clears all fault bits in the faults variable */
#define MID_FAULT_CLEAR_ALL(faults) ((faults) = 0U)

/* Check if a fault bit is set in the faults variable, 0 = no fault */
#define MID_FAULT_ANY(faults) ((faults) > 0U)

#define MID_FAULT_I_DCBUS_OVER    0   /* OverCurrent  fault flag */
#define MID_FAULT_U_DCBUS_UNDER   1   /* Undervoltage fault flag */
#define MID_FAULT_U_DCBUS_OVER    2   /* Overvoltage  fault flag */

/* States of machine enumeration. */
typedef enum _mid_sm_app_state_t{
    kMID_Start = 0U,
    kMID_Pp    = 1U,
    kMID_RL    = 2U,
    kMID_BJ    = 3U,
    kMID_Stop  = 4U,
    kMID_Fault = 5U,
    kMID_Calib = 6U,
} mid_sm_app_state_t;

/* Pointer to function with a pointer to state machine control structure. */
typedef void (*mid_pfcn_void_pms)(void);

/* Device fault typedef. */
typedef uint16_t middef_fault_t;

/* Device fault thresholds. */
typedef struct _middef_fault_thresholds_t
{
    float_t fltUDcBusOver;        /* DC bus over voltage level */
    float_t fltUDcBusUnder;       /* DC bus under voltage level */
    float_t fltUDcBusTrip;        /* DC bus voltage level to start braking */
    float_t fltSpeedOver;         /* Over speed level */
    float_t fltSpeedMin;          /* Minimum speed level */
    float_t fltSpeedNom;          /* Nominal speed */
    float_t fltUqBemf;            /* Blocked rotor U level */
    uint16_t ui16BlockedPerNum;   /* Number of period to set blocked rotor fault */
} middef_fault_thresholds_t;

/* Measurement type enumeration. */
typedef enum _mid_meas_type_t
{
    kMID_PolePairs        = 0U,
    kMID_ElectricalParams = 1U,
    kMID_MechanicalParams = 2U,
} mid_meas_type_t;

/* Float Motor parameters for internal MID structures and calculations. */
typedef struct _mid_motor_params
{
    uint32_t  ui32Pp; /* Number of pole-pairs. [-] */
    float_t   fltRs;  /* Stator resistance. [Ohm] */
    float_t   fltLd;  /* Direct-axis inductance. [H] */
    float_t   fltLq;  /* Quadrature axis inductance. [H] */
    float_t   fltUdt; /* Dead time voltage drop of the power stage [V]. */
    float_t   fltKe;  /* Electrical constant. [V·s/rad]  */
    float_t   fltKt;  /* Torque constant. [N·m/A]  */
    float_t   fltJ;   /* Moment of inertia. [kg·m^2] */
    float_t   fltA;   /* Static friction torque. [N·m] */
    float_t   fltB;   /* Friction coefficient. [N·m·s/rad] */
} mid_motor_params_t;

/* Motor parameters for user setting. */
typedef struct _mid_motor_params_user
{
    uint32_t  ui32Pp; /* Number of pole-pairs. [-] */
    float_t   fltRs;  /* Stator resistance. [Ohm] */
    float_t   fltLd;  /* Direct-axis inductance. [H] */
    float_t   fltLq;  /* Quadrature axis inductance. [H] */
    float_t   fltUdt; /* Dead time voltage drop of the power stage [V]. */
    float_t   fltKe;  /* Electrical constant. [V·s/rad]  */
    float_t   fltKt;  /* Torque constant. [N·m/A]  */
    float_t   fltJ;   /* Moment of inertia. [kg·m^2] */
    float_t   fltA;   /* Static friction torque. [N·m] */
    float_t   fltB;   /* Friction coefficient. [N·m·s/rad] */
} mid_motor_params_user_t;

/* MID FOC structure */
typedef struct _mid_pmsm_foc
{
    GDFLIB_FILTER_IIR1_T_FLT  sUDcBusFilter;  /* Dc bus voltage filter */
    GMCLIB_3COOR_T_F16        sIABCFrac;      /* Measured 3-phase current (FRAC)*/
    GMCLIB_3COOR_T_FLT        sIABC;          /* Measured 3-phase current [A] */
    GMCLIB_2COOR_ALBE_T_FLT   sIAlBe;         /* Alpha/Beta current */
    GMCLIB_3COOR_T_F16        sDutyABC;       /* Applied duty cycles ABC */
    GMCLIB_2COOR_ALBE_T_FLT   sUAlBeReq;      /* Required Alpha/Beta voltage */
    GMCLIB_2COOR_ALBE_T_F16   sUAlBeCompFrac; /* Compensated to DC bus Alpha/Beta voltage (FRAC) */
    uint16_t                  ui16SectorSVM;  /* SVM sector */
    float_t                   fltUDcBus;      /* DC bus voltage */
    frac16_t                  f16UDcBus;      /* DC bus voltage */
    float_t                   fltUDcBusFilt;  /* Filtered DC bus voltage */
} mid_pmsm_foc_t;

/* Measurement status. */
typedef struct _mid_status_t
{
    mid_sm_app_state_t eMIDState;            /* Actual MID state-machine status. */
    uint32_t           ui32AllFinishedMeas;  /* All finished measurements. */
    uint32_t           ui32ActFinishedMeas;  /* Actual finished measurement. */
    uint32_t           ui32FaultMID;         /* MID fault flags. */
} mid_status_t;

/* Measurement global structure. */
typedef struct _mid_struct_t
{
    mid_meas_type_t     eMeasurementType;   /* Measurement type. */
    mid_motor_params_t  sMotorParams;       /* Motor Parameters. */
    mid_status_t        sMIDMeasStatus;     /* Measurement status. */
    bool_t              bMIDStart;          /* MID trigger variable. */
} mid_struct_t;

/* MID and FOC global structure. */
typedef struct _mid_pmsm_t
{
    mid_pmsm_foc_t            sFocPMSM;             /* Field Oriented Control structure */
    middef_fault_t            sFaultIdCaptured;     /* Captured faults (must be cleared manually) */
    middef_fault_t            sFaultIdPending;      /* Fault pending structure */
    middef_fault_thresholds_t sFaultThresholds;     /* Fault thresholds */
    bool_t                    bFaultClearMan;       /* Manual fault clear detection */
    uint16_t                  ui16CounterState;     /* Main state counter */
    uint16_t                  ui16TimeCalibration;  /* Calibration time count number */
    frac16_t                  f16AdcAuxSample;      /* Auxiliary ADC sample  */
} mid_pmsm_t;

/* MID application command enum. */
typedef enum _foc_mid_cmd
{
    kMID_Cmd_Stop      = 0U,  /* STOP command. */
    kMID_Cmd_Start     = 1U,  /* START command. */
    kMID_Cmd_Executing = 2U   /* EXECUTING command. */
} mid_app_cmd_t;
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The MID control ctructure. */
extern mid_struct_t g_sMID;

#endif /* _MID_DEF_H_ */
