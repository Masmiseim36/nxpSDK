/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_pmsm_appconfig.h"
#include "mc_periph_init.h"
#include "pmsm_control.h"

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
/* MID parameters measurement options. */
#define MID_EL_RL (1UL <<  0U)
#define MID_EL_KE (1UL <<  1U)
#define MID_EL_ME (1UL <<  2U)
#define MID_EL_PP (1UL <<  3U)

/* MID Fault numbers. */
#define MID_FAULT_RL_ESTIM          (1U)
#define MID_FAULT_MECH_TIMEOUT      (2U)

#define MID_WARN_KE_OUT_OF_RANGE    (1U)

/* Sets the fault bit defined by faultid in the faults variable */
#define MID_FAULT_SET(faults, faultid) ((faults) |= ((middef_fault_t)1 << (faultid)))

/* Clears all fault bits in the faults variable */
#define MID_FAULT_CLEAR_ALL(faults) ((faults) = 0U)

/* Check if a fault bit is set in the faults variable, 0 = no fault */
#define MID_FAULT_ANY(faults) ((faults) > 0U)

#define MID_FAULT_I_DCBUS_OVER 0  /* OverCurrent fault flag */
#define MID_FAULT_U_DCBUS_UNDER 1 /* Undervoltage fault flag */
#define MID_FAULT_U_DCBUS_OVER 2  /* Overvoltage fault flag */

/* Frequency ramp time [s]. */
#define MID_SPEED_RAMP_TIME         (1.0F)

/* Fast control loop period */
#define MID_FAST_LOOP_TS    ((float_t)1.0/(float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_FAST_LOOP_FREQ  (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Maximal generated frequency - decided to be FAST_LOOP_TS / 10 */
#define MID_FREQ_MAX        ((float_t)MID_FAST_LOOP_FREQ / 10.0F)

/* Time quantities in units of fast loop ticks. */
#define MID_TIME_50MS (uint32_t)(0.05F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_TIME_100MS (uint32_t)(0.1F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_TIME_300MS (uint32_t)(0.3F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_TIME_600MS (uint32_t)(0.6F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_TIME_1200MS (uint32_t)(1.2F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_TIME_2400MS (uint32_t)(2.4F * (float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))

/* Maximal measuring signal levels. */
#define MID_I_MEAS_MAX      (M1_I_MAX)                                  /* [A] */
#define MID_FREQ_MEAS_MAX   (MID_FREQ_MAX)                              /* [Hz] */
#define MID_N_MEAS_MAX      (1500.0F * 3.0F * 2.0F * FLOAT_PI / 60.0F)  /* [rpm mech] */
#define MID_N_MEAS_MIN      (500.0F  * 3.0F * 2.0F * FLOAT_PI / 60.0F)  /* [rpm mech] */

/* Ke measurement MA filters lambda - window size inverse value */
#define MID_KE_FILT_MA_LAMBDA           (1.0F/10.0F)

/* Number of mechanical param measurements. */
#define MID_MECH_MEAS_NR                (3U)

/* Mech measurement LFP speed and torque filters - 200Hz */
#define MID_MECH_LP_FILT                (0.1180886217F)

/* Mech measurement Open-loop startup Merging Coefficient */
#define MID_MECH_STARTUP_MERGE_COEFF    (100.0F)

/* Mech measurement Open-loop startup Ramp coefficient [rpm/s]. */
#define MID_MECH_STARTUP_RAMP_COEFF     (500.0F)

/* Speed IIR filter bandwidth [Hz]. */
#define MID_SPEED_IIR_BANDWITH  (400.0F)

/* IIR scaling coefficient (defined by IIR implementation). */
#define MID_SPEED_IIR_COEFF     (2.0F)

/* Spped IIR filter coefficients. */
#define MID_SPEED_FILTER_IIR_B0 ((2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS)/(2.0F+(2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS))/MID_SPEED_IIR_COEFF)
#define MID_SPEED_FILTER_IIR_B1 ((2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS)/(2.0F+(2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS))/MID_SPEED_IIR_COEFF)
#define MID_SPEED_FILTER_IIR_A1 (-(2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS-2.0F)/(2.0F+(2.0F*FLOAT_PI*MID_SPEED_IIR_BANDWITH*MID_FAST_LOOP_TS))/MID_SPEED_IIR_COEFF)

/* Bemf observer attenuation and bandwidth */
#define MID_BEMF_ATT    (1.0F)
#define MID_BEMF_F0     (280.0F)

/* Bemf observer parameters calculation coefficients. */
/*
    BEMF_DQ_att = 1
    BEMF_DQ_f0 = 280 Hz
    Equations:  BemfDQ_Kpz = 2*BEMF_DQ_att*2*Math.PI*BEMF_DQ_f0*Ld-Rs
                BemfDQ_Kpz = 2*1*2*Pi*280*Ld-Rs
                BemfDQ_Kpz = 3518.58*Ld-Rs
                BemfDQ_Kpz = MID_BEMF_COEFF_KPZ*Ld-Rs
                BemfDQ_Kiz = Ld*Math.pow(2*Math.PI*BEMF_DQ_f0,2)*Ts
                BemfDQ_Kiz = Ld*Math.pow(2*Math.PI*280,2)*Ts
                BemfDQ_Kiz = Ld*3095107.94*Ts
                BemfDQ_Kiz = Ld*MID_BEMF_COEFF_KIZ*Ts
*/
#define MID_BEMF_COEFF_KPZ    (2.0F*MID_BEMF_ATT*2.0F*FLOAT_PI*MID_BEMF_F0)
#define MID_BEMF_COEFF_KIZ    (2.0F*FLOAT_PI*MID_BEMF_F0*2.0F*FLOAT_PI*MID_BEMF_F0)

/* Tracking observer attenuation and bandwidth */
#define MID_TO_ATT    (1.0F)
#define MID_TO_F0     (25.0F)

/* Tracking observer parameters calculation coefficients. */
/*
    TO_att = 1
    TO_f0 = 25 Hz
    Equations:  TO_Kpz = 2*TO_att*2*Math.PI*TO_f0
                TO_Kpz = 2*1*2*Pi*25
                TO_Kpz = 314
                TO_Kpz = MID_TO_COEFF_KPZ
                TO_Kiz = Math.pow(2*Math.PI*TO_f0,2)*Ts
                TO_Kiz = Math.pow(2*Math.PI*25,2)*Ts
                TO_Kiz = 24674.01*Ts
                TO_Kiz = MID_TO_COEFF_KIZ*Ts
                TO_Theta = Ts/Math.PI
                TO_Theta = Ts*MID_TO_COEFF_THETA
*/
#define MID_TO_COEFF_KPZ    (2.0F*MID_TO_ATT*2.0F*FLOAT_PI*MID_TO_F0)
#define MID_TO_COEFF_KIZ    (2.0F*FLOAT_PI*MID_TO_F0*2.0F*FLOAT_PI*MID_TO_F0)
#define MID_TO_COEFF_THETA  (1.0F/FLOAT_PI)

/* MID mesurement pre-check. */
#define MID_RL_INIT_FAIL    (1UL << 0U)
#define MID_RS_MISSING      (1UL << 1U)
#define MID_LD_MISSING      (1UL << 2U)
#define MID_LQ_MISSING      (1UL << 3U)
#define MID_KE_MISSING      (1UL << 4U)
#define MID_PP_MISSING      (1UL << 5U)

/* MID Configuration result. */
#define MID_CFG_SUCCESSFUL                (0U)
#define MID_ALIGN_CFG_OUT_OF_RANGE        (1UL << 0U)
#define MID_RS_MEAS_CFG_OUT_OF_RANGE      (1UL << 1U)
#define MID_LS_MEAS_CFG_OUT_OF_RANGE      (1UL << 2U)
#define MID_KE_MEAS_CFG_OUT_OF_RANGE      (1UL << 3U)
#define MID_PP_MEAS_CFG_OUT_OF_RANGE      (1UL << 4U)
#define MID_MECH_MEAS_CFG_OUT_OF_RANGE    (1UL << 5U)
#define MID_PP_PARAM_OUT_OF_RANGE         (1UL << 6U)

/* States of machine enumeration. */
typedef enum _mid_sm_app_state_t{
    kMID_Start         = 0U,
    kMID_RL            = 1U,
    kMID_Pp            = 2U,
    kMID_Ke            = 3U,
    kMID_Mech          = 4U,
    kMID_Hall          = 5U,
    kMID_Stop          = 6U,
    kMID_Fault         = 7U,
    kMID_Calib         = 8U,
} mid_sm_app_state_t;

/* Pointer to function with a pointer to state machine control structure. */
typedef void (*mid_pfcn_void_pms)(void);

/* Device fault typedef. */
typedef uint16_t middef_fault_t;

/* Device fault thresholds. */
typedef struct _middef_fault_thresholds_t
{
    float_t fltUDcBusOver;      /* DC bus over voltage level */
    float_t fltUDcBusUnder;     /* DC bus under voltage level */
    float_t fltUDcBusTrip;      /* DC bus voltage level to start braking */
    float_t fltSpeedOver;       /* Over speed level */
    float_t fltSpeedMin;        /* Minimum speed level */
    float_t fltSpeedNom;        /* Nominal speed */
    float_t fltUqBemf;          /* Blocked rotor U level */
    uint16_t ui16BlockedPerNum; /* Number of period to set blocked rotor fault */
} middef_fault_thresholds_t;

/* MID Alignment structure. */
typedef struct _mid_align_t
{
    float_t     fltCurrentAlign;    /* Alignment current. */
    uint16_t    ui16LoopCounter;    /* Serves for timing to determine e.g. 600ms. */
    uint16_t    ui16AlignDuration;  /* Alignment duration. */
    bool_t      bActive;            /* Indicates whether Rs is being measured (true) or not (false). */
} mid_align_t;

/* MID Ke measurement structure. */
typedef struct _mid_get_ke_t
{
    GFLIB_RAMP_T_FLT        sFreqElRampParam;   /* Ramp Up + Down coefficients for f16Speed. */
    GFLIB_INTEGRATOR_T_A32  sFreqIntegrator;    /* Speed integrator coefficients. */
    GDFLIB_FILTER_MA_T_FLT  sEdMA32Filter;      /* Bemf Ed MA filter. */
    GDFLIB_FILTER_MA_T_FLT  sEqMA32Filter;      /* Bemf Eq MA filter. */
    float_t                 fltIdReqOpenLoop;   /* Openloop current. */
    float_t                 fltFreqMax;         /* Frequency scale. */
    float_t                 fltFreqElReq;       /* Required Electrical Speed */
    float_t                 fltFreqElRamp;      /* Ramped f16SpeedElReq, this speed is integrated to get position. */
    float_t                 fltKe;              /* Resulting electrical constant. */
    uint16_t                ui16LoopCounter;    /* Serves for timing to determine e.g. 300ms. */
    bool_t                  bActive;            /* Indicates whether Ke is being measured (true) or not (false). */
} mid_get_ke_t;

/* MID Pp assistant structure. */
typedef struct _mid_get_pp_t
{
    GFLIB_RAMP_T_FLT        sFreqElRampParam;   /* Ramp Up + Down coefficients for f16Speed. */
    GFLIB_INTEGRATOR_T_A32  sFreqIntegrator;    /* Speed integrator coefficients. */
    float_t                 fltIdReqOpenLoop;   /* Openloop current. */
    float_t                 fltFreqMax;         /* Frequency scale. */
    float_t                 fltFreqElReq;       /* Required Electrical Speed. */
    float_t                 fltFreqElRamp;      /* Ramped f16SpeedElReq, this speed is integrated to get position. */
    uint16_t                ui16PpDetermined;   /* Indicates whether the user already set pp in MCAT (true) or not yet (false). */
    uint16_t                ui16WaitingSteady;  /* Indicates that motor is waiting in steady state (when electrical position is zero). */
    uint16_t                ui16LoopCounter;    /* Serves for timing to determine e.g. 300ms. */
    frac16_t                f16PosElCurrent;    /* Current value of electrical position. */
    frac16_t                f16PosElLast;       /* Last value of electrical position. */
    bool_t                  bActive;            /* Indicates whether Pp is being measured (true) or not (false). */
} mid_get_pp_t;

/* Open-loop startup structure. */
typedef struct _mid_ol_startup_t
{
    GFLIB_INTEGRATOR_T_A32  sSpeedIntegrator;           /* Speed integrator structure. */
    GFLIB_RAMP_T_FLT        sSpeedRampOpenLoopParams;   /* Parameters of startup speed ramp. */
    float_t                 fltSpeedReq;                /* Required speed. */
    float_t                 fltSpeedMax;                /* Maximum speed scale. */
    float_t                 fltSpeedRampOpenLoop;       /* Open loop startup speed ramp. */
    float_t                 fltSpeedCatchUp;            /* merging speed threshold. */
    float_t                 fltCurrentStartup;          /* required Iq current during open loop start up. */
    frac16_t                f16PosEst;                  /* Fractional electrical position. */
    frac16_t                f16CoeffMerging;            /* increment of merging weight for position merging. */
    frac16_t                f16RatioMerging;            /* merging weight coefficient. */
    frac16_t                f16PosGen;                  /* generated open loop position from the speed ramp integration. */
    frac16_t                f16PosMerged;               /* merged position. */
    uint16_t                ui16TimeStartUpFreeWheel; 	/* Free-wheel duration if start-up aborted by user input (required zero speed). */
    bool_t                  bOpenLoop;                  /* Position estimation loop is open. */
} mid_ol_startup_t;

/* MID Mechanical params measurement structure. */
typedef struct _mid_get_mech_t
{
    mid_ol_startup_t    sStartup;               /* Open-loop startup structure. */
    float_t             fltTorqueEst;           /* Estimated torque. */
    float_t             fltIqStartup;           /* Motor startup current. */
    float_t             fltMechMergingCoeff;    /* Motor startup merging coefficient. */
    float_t             fltIqAccelerate;        /* Motor accelerate current. */
    float_t             fltIqDecelerate;        /* Motor decelerate current. */
    float_t             fltSpeedThrsAccel;      /* Speed when the motor starts to accelerate. */
    float_t             fltSpeedThrsDecel;      /* Speed when the motor starts to decelerate. */
    float_t             fltSpeedThrsInteg;      /* Speed when the motor starts to integrate speed and torque. */
    float_t             fltSpeedFilt;           /* Low pass filtered estimated speed. */
    float_t             fltSpeedMax;            /* Maximal speed reached after acceleration. */
    float_t             fltTorqueFilt;          /* Low pass filtered torque. */
    float_t             fltLPFCoeff;            /* Low pass filter constant. */
    float_t             fltSpeedInteg;          /* Integrated speed. */
    float_t             fltTorqueInteg;         /* Integrated torque. */
    float_t             fltPp;                  /* Motor number of pole-pairs. */
    float_t             fltKt;                  /* Torque constant. */
    float_t             fltTauMech;             /* Mechanical time constant. */
    float_t             fltJ;                   /* Measured inertia. */
    float_t             fltB;                   /* Measured friction. */
    float_t             fltInertiaConst;        /* Measured inertia constant (Iq needed to accelerate with 1 rad/(s^2)). */
    uint32_t            ui32LoopCounter;        /* Serves for timing to determine e.g. 300ms. */
    uint32_t            ui32AccelLoopCounter;   /* Acceleration time. */
    uint32_t            ui32DecelLoopCounter;   /* Deceleration time. */
    uint32_t            ui32TimeMeasMax;        /* Acceleration measurement time limit. */
    uint32_t            ui32TimeSettleMax;      /* Deceleration measurement time limit. */
    uint16_t            ui16MeasNr;             /* Measurement number (there are several tries and the results are averaged). */
    bool_t              bActive;                /* Indicates whether Mech parameters are being measured (true) or not (false). */

    enum
    {
        kMID_MechInit       = 0U,
        kMID_MechStartUp    = 1U,
        kMID_MechAccelerate = 2U,
        kMID_MechDecelerate = 3U,
        kMID_MechEndOfMeas  = 4U
    } eState;
} mid_get_mech_t;

/* MID Hall sensors calibration structure. */
typedef struct _mid_calib_hall_t
{
    float_t   fltCalibrationSpeedReq; /* Required motor speed during calibration. */
    uint32_t  ui32CalibrationTime;    /* Calibration time. */
    uint32_t  ui32CalibrationCounter; /* Calibration time counter. */
    bool_t    bActive;                /* Indicates whether Hall sensors' placement is being calibrated (true) or not (false). */
} mid_calib_hall_t;

/* Measurement type enumeration. */
typedef enum _mid_meas_type_t
{
    kMID_PolePairs        = 0U,
    kMID_ElectricalParams = 1U,
    kMID_KeParam          = 2U,
    kMID_MechanicalParams = 3U,
    kMID_HallSensors      = 4U
} mid_meas_type_t;

/* Float Motor parameters for internal MID structures and calculations. */
typedef struct _mid_motor_params
{
    float_t     fltRs;  /* Stator resistance. [Ohm] */
    float_t     fltLd;  /* Direct-axis inductance. [H] */
    float_t     fltLq;  /* Quadrature axis inductance. [H] */
    float_t     fltKe;  /* Electrical constant. [V*s/rad]  */
    float_t     fltJ;   /* Moment of inertia. [kg*m^2] */
    float_t     fltB;   /* Friction coefficient. [Nm*s] */
    float_t     fltPp;  /* Number of pole-pairs. [-] */
} mid_motor_params_t;

/* Motor parameters for user setting. */
typedef struct _mid_motor_params_user
{
    float_t     fltRs;  /* Stator resistance. [Ohm] */
    float_t     fltLd;  /* Direct-axis inductance. [H] */
    float_t     fltLq;  /* Quadrature axis inductance. [H] */
    float_t     fltKe;  /* Electrical constant. [V*s/rad]  */
    float_t     fltJ;   /* Moment of inertia. [kg*m^2] */
    float_t     fltB;   /* Friction coefficient. [Nm*s] */
    uint32_t    ui32Pp; /* Number of pole-pairs. [-] */
} mid_motor_params_user_t;

/* Measurement status. */
typedef struct _mid_status_t
{
    mid_sm_app_state_t eMIDState;          /* Actual MID state-machine status. */
    uint32_t           ui32FinishedMeas;   /* Finished measurements. */
    float_t            fltSpeedAngScale;   /* Speed scale (mechanical rpm -> electrical rad/s) */
    uint16_t           ui16FaultMID;       /* MID fault code. */
    uint16_t           ui16WarnMID;        /* MID warning code. */
} mid_status_t;

/* Measurement configuration. */
typedef struct _mid_config_t
{
    /* MID alignment: */
    float_t     fltAlignId;             /* DC alignment current [A]. */

    /* Ke measurement: */
    float_t     fltKeIdReqOpenLoop;     /* Open-loop current for startup and spin [A]. */
    float_t     fltKeFreqElReq;         /* Desired motor electrical speed [Hz]. */

    /* Pp assistant: */
    float_t     fltPpIdReqOpenLoop;     /* Open-loop current for startup and spin [A]. */
    float_t     fltPpFreqElReq;         /* Motor electrical speed [Hz]. */

    /* Mechanical parametes measurement: */
    float_t     fltMechKt;              /* Motor torque constant [Nm/A]. */
    float_t     fltMechIqStartup;       /* Motor startup current [A]. */
    float_t     fltMechMergingCoeff;    /* OL startup merging ratio [%] */
    float_t     fltMechIqAccelerate;    /* Motor accelerate current [A]. */
    float_t     fltMechIqDecelerate;    /* Motor decelerate current [A]. */
    float_t     fltMechSpeedThrsAccel;  /* Speed when the motor starts to accelerate [rad/s electrical]. */
    float_t     fltMechSpeedThrsDecel;  /* Speed when the motor starts to decelerate [rad/s electrical]. */
    float_t     fltMechSpeedThrsInteg;  /* Speed when the motor starts to integrate speed and torque [rad/s electrical]. */
} mid_config_t;

/* Measurement global structure. */
typedef struct _mid_struct_t
{
    mid_align_t         sMIDAlignment;      /* Structure for MID_alignment(). */
    mid_get_ke_t        sMIDKe;             /* Structure for MID_getKe(). */
    mid_get_pp_t        sMIDPp;             /* Structure for MID_getPp(). */
    mid_get_mech_t      sMIDMech;           /* Structure for MID_getMech(). */
    mid_calib_hall_t    sMIDHall;           /* Structure for MID_calibHall(). */
    mid_meas_type_t     eMeasurementType;   /* Measurement type. */
    mid_sm_app_state_t  eMIDState;          /* MID state machine actual state. */
    mid_motor_params_t  sMotorParams;       /* Motor Parameters. */
    float_t             fltSpeedAngScale;   /* Speed scale (mechanical rpm -> electrical rad/s) */
    uint32_t            ui32FinishedMeas;   /* Array for finished eletrical measurements. */
    uint16_t            ui16FaultMID;       /* MID fault code. */
    uint16_t            ui16WarnMID;        /* MID warning code. */
    bool_t              bMIDStart;          /* MID trigger variable. */
} mid_struct_t;

/* MID and FOC global structure. */
typedef struct _mid_pmsm_t
{
  mcs_pmsm_foc_t sFocPMSM;                       /* Field Oriented Control structure */
  middef_fault_t sFaultIdCaptured;                /* Captured faults (must be cleared manually) */
  middef_fault_t sFaultIdPending;                 /* Fault pending structure */
  middef_fault_thresholds_t sFaultThresholds;     /* Fault thresholds */
  bool_t bFaultClearMan;                         /* Manual fault clear detection */
  uint16_t ui16CounterState;                     /* Main state counter */
  uint16_t ui16TimeCalibration;                  /* Calibration time count number */
  frac16_t f16AdcAuxSample;                      /* Auxiliary ADC sample  */
} mid_pmsm_t;

/* MID application command enum. */
typedef enum _foc_mid_cmd
{
    kMID_Cmd_Stop  = 0U,  /* STOP command. */
    kMID_Cmd_Ready = 1U,  /* READY command. */
    kMID_Cmd_Run   = 2U   /* START command. */
} mid_app_cmd_t;
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The MID control ctructure. */
extern mid_struct_t g_sMID;

#endif /* _MID_DEF_H_ */
