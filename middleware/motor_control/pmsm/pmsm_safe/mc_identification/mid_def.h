/*
 * Copyright 2020 NXP.
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_pmsm_appconfig.h"
#include "m1_state_routines.h"
#include "mcdrv_periph_init.h"

MISRAC_DISABLE /* MISRA check disabled (IAR IDE only). */

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

MISRAC_ENABLE /* MISRA check enabled (IAR IDE only). */

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MID parameters measurement oprions. */
#define MID_EL_RS (1UL <<  0U)
#define MID_EL_LD (1UL <<  1U)
#define MID_EL_LQ (1UL <<  2U)
#define MID_EL_KE (1UL <<  3U)
#define MID_EL_ME (1UL <<  4U)

/* MID Fault numbers. */
#define MID_FAULT_ABORT             (1U)
#define MID_FAULT_NO_MOTOR          (2U)
#define MID_FAULT_TOO_HIGH_RS       (4U)
#define MID_FAULT_MECH_TIMEOUT      (8U)

#define MID_WARN_DC_CUR_NOT_REACHED (1U)
#define MID_WARN_AC_CUR_NOT_REACHED (2U)
#define MID_WARN_RS_OUT_OF_RANGE    (4U)
#define MID_WARN_LS_OUT_OF_RANGE    (8U)
#define MID_WARN_KE_OUT_OF_RANGE    (16U)

/* Frequency ramp time [s]. */
#define MID_SPEED_RAMP_TIME         (1.0F)

/* MID characteristic parameters. */
#define MID_CHAR_CURRENT_POINT_NUMBERS  (65U)

/* Fast control loop period */
#define MID_FAST_LOOP_TS    ((float_t)1.0/(float_t)(M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ))
#define MID_FAST_LOOP_FREQ  (M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Maximal generated frequency - decided to be FAST_LOOP_TS / 10 */
#define MID_FREQ_MAX        ((float_t)MID_FAST_LOOP_FREQ / 10.0F)

/* Time quantities in units of fast loop ticks. */
#define MID_TIME_50MS       (50U   * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_100MS      (100U  * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_300MS      (300U  * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_600MS      (600U  * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_1200MS     (1200U * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_2400MS     (2400U * M1_PWM_FREQ / 1000U / M1_FOC_FREQ_VS_PWM_FREQ)

/* Maximal measuring signal levels. */
#define MID_I_MEAS_MAX      (M1_I_MAX)                                  /* [A] */
#define MID_FREQ_MEAS_MAX   (MID_FREQ_MAX)                              /* [Hz] */
#define MID_N_MEAS_MAX      (1500.0F * 3.0F * 2.0F * FLOAT_PI / 60.0F)  /* [rpm mech] */
#define MID_N_MEAS_MIN      (500.0F  * 3.0F * 2.0F * FLOAT_PI / 60.0F)  /* [rpm mech] */

/* Rs measurement MA filters lambda - window size inverse value */
#define MID_RS_FILT_MA_LAMBDA           (1.0F/20.0F)

/* Ke measurement MA filters lambda - window size inverse value */
#define MID_KE_FILT_MA_LAMBDA           (1.0F/10.0F)

/* Maximum phase voltage value from available DCbus. Used for max voltage
   amplitude calculation at Ls measurement */
#define MID_LS_K_MODULATION_RATIO       (0.5F)

/* Motor not connected current threshold. */
#define MID_K_I_50MA                    (0.05F)

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
#define MID_RS_MISSING      (1UL << 0U)
#define MID_LD_MISSING      (1UL << 1U)
#define MID_LQ_MISSING      (1UL << 2U)
#define MID_KE_MISSING      (1UL << 3U)
#define MID_PP_MISSING      (1UL << 4U)

/* States of machine enumeration. */
typedef enum _mid_sm_app_state_t{
    kMID_Start         = 0U,
    kMID_PwrStgCharact = 1U,
    kMID_Rs            = 2U,
    kMID_Ld            = 3U,
    kMID_Lq            = 4U,
    kMID_Pp            = 5U,
    kMID_Ke            = 6U,
    kMID_Mech          = 7U,
    kMID_Hall          = 8U,
    kMID_Stop          = 9U
} mid_sm_app_state_t;

/* MID Configuration result. */
typedef enum _mid_config_result{
    kMID_ConfigSuccessful        = 0U,
    kMID_AlignCfgOutOfRange      = 1U,
    kMID_RsMeasCfgOutOfRange     = 2U,
    kMID_LsMeasCfgOutOfRange     = 3U,
    kMID_KeMeasCfgOutOfRange     = 4U,
    kMID_PpMeasCfgOutOfRange     = 5U,
    kMID_PwrStgMeasCfgOutOfRange = 6U,
    kMID_MechMeasCfgOutOfRange   = 7U,
    kMID_PpParamOutOfRange       = 8U
} mid_config_result_t;

/* Pointer to function with a pointer to state machine control structure. */
typedef void (*mid_pfcn_void_pms)(void);

/* MID Alignment structure. */
typedef struct _mid_align_t
{
    float_t     fltCurrentAlign;    /* Alignment current. */
    uint16_t    ui16Active;         /* Inidicates whether Rs is being measured (true) or not (false). */
    uint16_t    ui16LoopCounter;    /* Serves for timing to determine e.g. 600ms. */
    uint16_t    ui16AlignDuration;  /* Alignment duration. */
} mid_align_t;

/* MID Power Stage characterization structure. */
typedef struct _mid_get_char_t
{
    GDFLIB_FILTER_MA_T_FLT  sUdReqMA32Filter;       /* Ud required filter. */
    GDFLIB_FILTER_MA_T_FLT  sIdfbckMA32Filter;      /* Id feedback filter. */
    float_t                 fltRs;                  /* Stator resistance of connected motor. */
    float_t                 fltIdCalib;             /* Range of the characterisation is from -fltIdCharMax to +fltIdCharMax. */
    float_t                 fltIdReqActual;         /* Actual current at which is the characteristic being measured at the moment. */
    float_t                 fltIdIncrement;         /* Id increment after each 300ms. */
    uint16_t                ui16Active;             /* Inidicates whether Transfer characteristic is being measured (true) or not (false). */
    uint16_t                ui16LoopCounter;        /* Serves for timing to determine e.g. 600ms. */
    uint16_t                ui16LUTIndex;           /* Lookup table index, counts up to 65 until a whole characteristic is measured. */
    uint16_t                ui16NumOfChPnts;        /* number of points in LUT. */
} mid_get_char_t;

/* MID Rs measurement structure. */
typedef struct _mid_get_rs_t
{
    GDFLIB_FILTER_MA_T_FLT  sUdReqMA32Filter;       /* Ud required filter. */
    GDFLIB_FILTER_MA_T_FLT  sIdfbckMA32Filter;      /* Id feedback filter. */
    float_t                 fltIdMeas;              /* User defined measuring DC current. */
    float_t                 fltRs;                  /* Measured Rs return. */
    uint16_t                ui16Active;             /* Inidicates whether Rs is being measured (true) or not (false). */
    uint16_t                ui16LoopCounter;        /* Serves for timing to determine e.g. 600ms. */
} mid_get_rs_t;

/* MID Ls measurement structure. */
typedef struct _mid_get_ls_t
{
    GFLIB_INTEGRATOR_T_A32  sFreqIntegrator;        /* Speed integrator coefficients. */
    float_t                 fltFreqMax;             /* Frequency scale. */
    float_t                 fltFreqStart;           /* Starting measuring frequency. */
    float_t                 fltFreqDecrement;       /* Frequency decrement when tuning the measuring signal. */
    float_t                 fltFreqMin;             /* Minimal measuring frequency. */
    float_t                 fltFreqActual;          /* Actual Measuring Frequency. */
    float_t                 fltIdAmplitudeReq;      /* Required amplitude of measuring sine current. */
    float_t                 fltIdAmplitude;         /* Amplitude of measuring sine current. */
    float_t                 fltUdMax;               /* Maximal amplitude of measuring voltage set as 1/2 DCbus before measurement. */
    float_t                 fltUdAmplitude;         /* Amplitude of measuring sine voltage. */
    float_t                 fltUdIncrement;         /* Voltage increment when tuning the measuring signal. */
    float_t                 fltRs;                  /* Measured Rs set before measurement. */
    float_t                 fltLs;                  /* Measured Ls return. */
    uint16_t                ui16Active;             /* Inidicates whether Ls is being measured (true) or not (false). */
    uint16_t                ui16LoopCounter;        /* Serves for timing to determine e.g. 300ms. */
    frac16_t                f16Angle;               /* Angle for generating the measuring sine signal. */
    bool_t                  bAmplitudeOK;           /* Indicates that amplitude of the measuring signal was set. */
    bool_t                  bFrequencyOK;           /* Indicates that frequency of the measuring signal was set. */
} mid_get_ls_t;

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
    uint16_t                ui16Active;         /* Inidicates whether Ke is being measured (true) or not (false). */
    uint16_t                ui16LoopCounter;    /* Serves for timing to determine e.g. 300ms. */
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
    uint16_t                ui16Active;         /* Indicates whether Ke is being measured (true) or not (false). */
    uint16_t                ui16PpDetermined;   /* Indicates whether the user already set pp in MCAT (true) or not yet (false). */
    uint16_t                ui16WaitingSteady;  /* Indicates that motor is waiting in steady state (when electrical position is zero). */
    uint16_t                ui16LoopCounter;    /* Serves for timing to determine e.g. 300ms. */
    frac16_t                f16PosElCurrent;    /* Current value of electrical position. */
    frac16_t                f16PosElLast;       /* Last value of electrical position. */
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
    uint16_t            ui16Active;             /* Indicates whether Mech parameters are being measured (true) or not (false). */
    uint16_t            ui16MeasNr;             /* Measurement number (there are several tries and the results are averaged). */

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
    uint16_t  ui16Active;             /* Indicates whether Hall sensors' placement is being calibrated (true) or not (false). */
} mid_calib_hall_t;

/* Measurement type enumeration. */
typedef enum _mid_meas_type_t
{
    kMID_Calibration      = 0U,
    kMID_PolePairs        = 1U,
    kMID_ElectricalParams = 2U,
    kMID_MechanicalParams = 3U,
    kMID_HallSensors      = 4U
} mid_meas_type_t;

/* Float Motor parameters for internal MID structures and calsulations. */
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
    uint32_t    ui32ScheduledMeas;      /* Scheduled electrical measurements. */

    /* MID alignment: */
    float_t     fltAlignId;             /* DC alignment current [A]. */

    /* Rs measurement: */
    float_t     fltRsIdMeas;            /* DC measurement current [A]. */

    /* Ls measurement: */
    float_t     fltLsIdAmplitudeReq;    /* AC measurement signal current amplitude [A]. */
    float_t     fltLsUdIncrement;       /* AC measurement signal voltage increment [V]. */
    float_t     fltLsFreqStart;         /* AC measurement signal initial frequency [Hz]. */
    float_t     fltLsFreqDecrement;     /* AC measurement signal frequency decrement [Hz]. */
    float_t     fltLsFreqMin;           /* AC measurement signal minimal frequency [Hz]. */

    /* Ke measurement: */
    float_t     fltKeIdReqOpenLoop;     /* Open-loop current for startup and spin [A]. */
    float_t     fltKeFreqElReq;         /* Desired motor electrical speed [Hz]. */

    /* Pp assistant: */
    float_t     fltPpIdReqOpenLoop;     /* Open-loop current for startup and spin [A]. */
    float_t     fltPpFreqElReq;         /* Motor electrical speed [Hz]. */

    /* PwrStg characterization: */
    float_t     fltPwrStgRs;            /* Connected Resistance [ohm]. */
    float_t     fltPwrStgIdCalib;       /* Characterization current range [A]. */

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
    mid_get_char_t      sMIDPwrStgChar;     /* Structure for MID_getTransChar(). */
    mid_get_rs_t        sMIDRs;             /* Structure for MID_getRs(). */
    mid_get_ls_t        sMIDLs;             /* Structure for MID_getLs(). */
    mid_get_ke_t        sMIDKe;             /* Structure for MID_getKe(). */
    mid_get_pp_t        sMIDPp;             /* Structure for MID_getPp(). */
    mid_get_mech_t      sMIDMech;           /* Structure for MID_getMech(). */
    mid_calib_hall_t    sMIDHall;           /* Structure for MID_calibHall(). */
    mid_meas_type_t     eMeasurementType;   /* Measurement type. */
    mid_sm_app_state_t  eMIDState;          /* MID state machine actual state. */
    mid_motor_params_t  sMotorParams;       /* Motor Parameters. */
    float_t             fltSpeedAngScale;   /* Speed scale (mechanical rpm -> electrical rad/s) */
    uint32_t            ui32ScheduledMeas;  /* Array for schedulled eletrical measurements. */
    uint32_t            ui32FinishedMeas;   /* Array for finished eletrical measurements. */
    uint16_t            ui16FaultMID;       /* MID fault code. */
    uint16_t            ui16WarnMID;        /* MID warning code. */
    bool_t              bMIDStart;          /* MID trigger variable. */
} mid_struct_t;

/*******************************************************************************
 * Variables
 ******************************************************************************/
/* The MID control ctructure. */
extern mid_struct_t g_sMID;

#endif /* _MID_DEF_H_ */
