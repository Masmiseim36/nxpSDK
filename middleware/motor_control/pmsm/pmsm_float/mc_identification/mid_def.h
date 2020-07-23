/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_pmsm_appconfig.h"
#include "mcdrv.h"

/* RTCESL fix libraries */
#include "mlib.h"
#include "gflib.h"
#include "gdflib.h"
#include "gmclib.h"
#include "amclib.h"

/* RTCESL float libraries */
#include "mlib_FP.h"
#include "gflib_FP.h"
#include "gdflib_FP.h"
#include "gmclib_FP.h"
#include "amclib_FP.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* MID Fault numbers */
#define MID_FAULT_ABORT (1)
#define MID_FAULT_NO_MOTOR (2)
#define MID_FAULT_TOO_HIGH_RS (4)
#define MID_FAULT_MECH_TIMEOUT (8)

#define MID_WARN_DC_CUR_NOT_REACHED (1)
#define MID_WARN_AC_CUR_NOT_REACHED (2)
#define MID_WARN_RS_OUT_OF_RANGE (4)
#define MID_WARN_LS_OUT_OF_RANGE (8)
#define MID_WARN_KE_OUT_OF_RANGE (16)

/* Current controllers' coefficients ensuring slow response for variable parameters */
#define MID_KP_GAIN (0.0884654752228F)
#define MID_KI_GAIN (0.0119161655697F)

/* Frequency ramp time */
#define MID_SPEED_RAMP_TIME (1.0)

/* MID characteristic parameters */
#define MID_CHAR_CURRENT_POINT_NUMBERS (65)

/* Time quantities in units of fast loop ticks */
#define MID_TIME_50MS (0.050 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_100MS (0.100 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_300MS (0.300 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_600MS (0.600 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_1200MS (1.200 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)
#define MID_TIME_2400MS (2.400 * M1_PWM_FREQ / M1_FOC_FREQ_VS_PWM_FREQ)

/* Maximum phase voltage value from available DCbus */
#define MID_K_MODULATION_RATIO (0.5773503)
#define MID_K_I_50MA (0.05)

/* Number of mechanical param measurements */
#define MID_MECH_MEAS_NR (5)

/* MID I/O structure - WARNING all pointers must be initialized */
typedef struct
{
    frac16_t *pf16PosElExt; /* pointer to external electrical position [frac <-1;1)] */
    frac16_t *pf16PosElEst; /* pointer to estimated electrical position [frac <-1;1)] */
    float_t *pfltId;        /* pointer to actual value of d-axis current [A] */
    float_t *pfltIq;        /* pointer to actual value of q-axis current [A] */
    float_t *pfltIdReq;     /* pointer to required d-axis current [A] */
    float_t *pfltIqReq;     /* pointer to required q-axis current [A] */
    float_t *pfltUdReq;     /* pointer to required d-axis voltage [V] */
    float_t *pfltUqReq;     /* pointer to required q-axis voltage [V] */
    float_t *pfltUDCbus;    /* pointer to filtered DC-bus voltage voltage [V] */
    float_t *pfltEd;        /* pointer to bemf d-axis voltage [V] */
    float_t *pfltEq;        /* pointer to bemf q-axis voltage [V] */
    float_t *pfltSpeedEst;  /* pointer to estimated speed [el. rad/s] */
    float_t *pfltSpeedReq;  /* pointer to required speed [el. rad/s] */
} mid_io_t;

/* MID Alignment structure */
typedef struct
{
    uint16_t ui16Active;        /* Inidicates whether Rs is being measured (true) or not (false) */
    uint16_t ui16LoopCounter;   /* Serves for timing to determine e.g. 600ms */
    float_t *pfltIdReq;         /* Pointer to required current Id (input to controllers) */
    float_t fltCurrentAlign;    /* Alignment current */
    uint16_t ui16AlignDuration; /* Alignment duration */
} mid_align_t;

/* MID Power Stage characterization structure */
typedef struct
{
    GDFLIB_FILTER_MA_T_FLT sUdReqMA32Filter;  /* Ud required filter */
    GDFLIB_FILTER_MA_T_FLT sIdfbckMA32Filter; /* Id feedback filter */
    uint16_t ui16Active;      /* Inidicates whether Transfer characteristic is being measured (true) or not (false) */
    uint16_t ui16LoopCounter; /* Serves for timing to determine e.g. 600ms */
    float_t fltRs;            /* Stator resistance of connected motor */
    float_t *pfltIdReq;       /* Pointer to required current Id (PI current controller's input) */
    float_t *pfltUdReq;       /* Pointer to required voltage Ud (PI current controller's output) */
    float_t *pfltIdfbck;      /* Pointer to actual real measured current Id */
    float_t *pfltUDCbus;      /* Pointer to DCbus voltage */
    float_t fltIdCalib;       /* Range of the characterisation is from -fltIdCharMax to +fltIdCharMax */
    float_t fltIdReqActual;   /* Actual current at which is the characteristic being measured at the moment */
    float_t fltIdIncrement;   /* Id increment after each 300ms */
    float_t fltUdErrorLookUp[MID_CHAR_CURRENT_POINT_NUMBERS]; /* Ud error voltages */
    uint16_t ui16LUTIndex;    /* Lookup table index, counts up to 65 until a whole characteristic is measured */
    uint16_t ui16NumOfChPnts; /* number of points in LUT */
} mid_get_char_t;

/* MID Rs measurement structure */
typedef struct
{
    GDFLIB_FILTER_MA_T_FLT sUdReqMA32Filter;  /* Ud required filter */
    GDFLIB_FILTER_MA_T_FLT sIdfbckMA32Filter; /* Id feedback filter */
    GFLIB_LUT1D_T_FLT sUerrLUTparams;         /* Uerr LUT parameters */
    uint16_t ui16Active;                      /* Inidicates whether Rs is being measured (true) or not (false) */
    uint16_t ui16LoopCounter;                 /* Serves for timing to determine e.g. 600ms */
    float_t *pfltIdReq;                       /* Pointer to required current Id (PI current controller's input) */
    float_t fltIdMeas;                        /* User defined measuring DC current */
    float_t *pfltUdReq;                       /* Pointer to required voltage Ud (PI current controller's output) */
    float_t *pfltIdfbck;                      /* Pointer to actual real measured current Id */
    float_t fltRs;                            /* Measured Rs return */
    float_t *pfltUdErrorLookUp;    /* Pointer to Look Up table with the transfer characteristic coefficients */
    float_t fltUdErrorLookUpRange; /* Ud error LUT range (Id current in amps) */
} mid_get_rs_t;

/* MID Ls measurement structure */
typedef struct
{
    GFLIB_INTEGRATOR_T_A32 sFreqIntegrator; /* Speed integrator coefficients */
    uint16_t ui16Active;                    /* Inidicates whether Ls is being measured (true) or not (false) */
    uint16_t ui16LoopCounter;               /* Serves for timing to determine e.g. 300ms */
    int16_t i16AmplitudeOK;                 /* Indicates that amplitude of the measuring signal was set */
    int16_t i16FrequencyOK;                 /* Indicates that frequency of the measuring signal was set */
    float_t *pfltUdReq;                     /* Pointer to required voltage Ud which is applied to the motor */
    float_t *pfltIdfbck;                    /* Pointer to actual real measured current Id */
    float_t fltFreqMax;                     /* Frequency scale */
    float_t fltFreqStart;                   /* Starting measuring frequency */
    float_t fltFreqDecrement;               /* Frequency decrement when tuning the measuring signal */
    float_t fltFreqMin;                     /* Minimal measuring frequency */
    float_t fltFreqActual;                  /* Actual Measuring Frequency */
    frac16_t f16Angle;                      /* Angle for generating the measuring sine signal */
    float_t fltIdAmplitudeReq;              /* Required amplitude of measuring sine current */
    float_t fltIdAmplitude;                 /* Amplitude of measuring sine current */
    float_t fltUdMax;       /* Maximal amplitude of measuring voltage set as 1/2 DCbus before measurement */
    float_t fltUdAmplitude; /* Amplitude of measuring sine voltage */
    float_t fltUdIncrement; /* Voltage increment when tuning the measuring signal */
    float_t fltRs;          /* Measured Rs set before measurement */
    float_t fltLs;          /* Measured Ls return */
    float_t fltLd;          /* For storage Ld */
    float_t fltLq;          /* For storage Lq */
} mid_get_ls_t;

/* MID Ke measurement structure */
typedef struct
{
    uint16_t ui16Active;        /* Inidicates whether Ke is being measured (true) or not (false) */
    uint16_t ui16MCATObsrvDone; /* Inidicates whether MCAT has calculated Befm observer (true) or not yet (false) */
    uint16_t ui16LoopCounter;   /* Serves for timing to determine e.g. 300ms */
    frac16_t *pf16PosEl;        /* Pointer to electrical position for Park transformations */
    float_t *pfltUdReq;         /* Pointer to required voltage Ud which is applied to the motor */
    float_t *pfltIdReq;         /* Pointer to required current Id (PI current controller's input) */
    float_t *pfltIdfbck;        /* Pointer to actual real measured current Id */
    float_t *pfltEd;            /* Pointer to Befm voltage in d-axis calculated by Bemf observer */
    float_t *pfltEq;            /* Pointer to Befm voltage in q-axis calculated by Bemf observer */
    float_t fltIdReqOpenLoop;   /* Openloop current */
    float_t fltFreqMax;         /* Frequency scale */
    float_t fltFreqElReq;       /* Required Electrical Speed */
    float_t fltFreqElRamp;      /* Ramped f16SpeedElReq, this speed is integrated to get position */
    float_t fltKe;              /* Resulting electrical constant */
    GFLIB_RAMP_T_FLT sFreqElRampParam;      /* Ramp Up + Down coefficients for f16Speed */
    GFLIB_INTEGRATOR_T_A32 sFreqIntegrator; /* Speed integrator coefficients */
    GDFLIB_FILTER_MA_T_FLT sEdMA32Filter;   /* Bemf Ed MA filter */
    GDFLIB_FILTER_MA_T_FLT sEqMA32Filter;   /* Bemf Eq MA filter */
} mid_get_ke_t;

/* MID Pp assistant structure */
typedef struct
{
    uint16_t ui16Active;        /* Indicates whether Ke is being measured (true) or not (false) */
    uint16_t ui16PpDetermined;  /* Indicates whether the user already set pp in MCAT (true) or not yet (false) */
    uint16_t ui16WaitingSteady; /* Indicates that motor is waiting in steady state (when electrical position is zero) */
    uint16_t ui16LoopCounter;   /* Serves for timing to determine e.g. 300ms */
    frac16_t *pf16PosEl;        /* Pointer to electrical position for Park transformations */
    float_t *pfltIdReq;         /* Pointer to required current Id (PI current controller's input) */
    frac16_t f16PosElCurrent;   /* Current value of electrical position */
    frac16_t f16PosElLast;      /* Last value of electrical position */
    float_t fltIdReqOpenLoop;   /* Openloop current */
    float_t fltFreqMax;         /* Frequency scale */
    float_t fltFreqElReq;       /* Required Electrical Speed */
    float_t fltFreqElRamp;      /* Ramped f16SpeedElReq, this speed is integrated to get position */
    GFLIB_RAMP_T_FLT sFreqElRampParam;      /* Ramp Up + Down coefficients for f16Speed */
    GFLIB_INTEGRATOR_T_A32 sFreqIntegrator; /* Speed integrator coefficients */
} mid_get_pp_t;

/* Open-loop startup structure */
typedef struct
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
} mid_ol_startup_t;

/* MID Mechanical params measurement structure */
typedef struct
{
    enum
    {
        kMID_MechInit       = 0,
        kMID_MechStartUp    = 1,
        kMID_MechAccelerate = 2,
        kMID_MechDecelerate = 3,
        kMID_MechEndOfMeas  = 4
    } eState;

    mid_ol_startup_t sStartup; /* Open-loop startup structure */

    uint16_t ui16Active;           /* Indicates whether Mech parameters are being measured (true) or not (false) */
    uint16_t ui16MeasNr;           /* Measurement number (there are several tries and the results are averaged) */
    uint32_t ui32LoopCounter;      /* Serves for timing to determine e.g. 300ms */
    uint32_t ui32AccelLoopCounter; /* Acceleration time */
    uint32_t ui32DecelLoopCounter; /* Deceleration time */
    uint32_t ui32TimeMeasMax;      /* Acceleration measurement time limit */
    uint32_t ui32TimeSettleMax;    /* Deceleration measurement time limit */
    float_t *pfltIdReq;            /* Pointer to required current Id (PI current controller's input) */
    float_t *pfltIdfbck;           /* Pointer to actual real measured current Id */
    float_t *pfltIqReq;            /* Pointer to required current Iq (PI current controller's input) */
    float_t *pfltIqfbck;           /* Pointer to actual real measured current Iq */
    float_t *pfltSpeedEst;         /* Pointer to Estimated speed */
    frac16_t *pf16PosElExt;        /* pointer to external electrical position [frac <-1;1)] */
    frac16_t *pf16PosElEst;        /* pointer to estimated electrical position [frac <-1;1)] */
    float_t fltTorqueEst;          /* Estimated torque */
    float_t fltIqStartup;          /* Motor startup current*/
    float_t fltIqAccelerate;       /* Motor accelerate current*/
    float_t fltIqDecelerate;       /* Motor decelerate current*/
    float_t fltSpeedThrsAccel;     /* Speed when the motor starts to accelerate */
    float_t fltSpeedThrsDecel;     /* Speed when the motor starts to decelerate */
    float_t fltSpeedThrsInteg;     /* Speed when the motor starts to integrate speed and torque */
    float_t fltSpeedFilt;          /* Low pass filtered estimated speed */
    float_t fltSpeedMax;           /* Maximal speed reached after acceleration */
    float_t fltTorqueFilt;         /* Low pass filtered torque */
    float_t fltLPFCoeff;           /* Low pass filter constant */
    float_t fltSpeedInteg;         /* Integrated speed */
    float_t fltTorqueInteg;        /* Integrated torque */
    float_t fltPp;                 /* Motor number of pole-pairs */
    float_t fltKt;                 /* Torque constant */
    float_t fltTauMech;            /* Mechanical time constant */
    float_t fltJ;                  /* Measured inertia */
    float_t fltB;                  /* Measured friction */
    float_t fltInertiaConst;       /* Measured inertia constant (Iq needed to accelerate with 1 rad/(s^2)) */
} mid_get_mech_t;

/* MID Hall sensors calibration structure */
typedef struct
{
    bool_t *bCalibrationStartStop; /* Start/stop the calibration to calibrate at stable speed */
    uint16_t ui16Active; /* Indicates whether Hall sensors' placement is being calibrated (true) or not (false) */
    uint32_t ui32CalibrationTime;    /* Calibration time */
    uint32_t ui32CalibrationCounter; /* Calibration time counter */
    float_t *pfltSpeedReq;           /* Pointer to required speed */
    float_t *pfltSpeedEst;           /* Pointer to estimated speed */
    float_t fltCalibrationSpeedReq;  /* Required motor speed during calibration */
} mid_calib_hall_t;

/* measurement global structure */
typedef struct
{
    mid_io_t sIO;                  /* Poiters to I/O quantities */
    mid_align_t sMIDAlignment;     /* Structure for MID_alignment() */
    mid_get_char_t sMIDPwrStgChar; /* Structure for MID_getTransChar() */
    mid_get_rs_t sMIDRs;           /* Structure for MID_getRs() */
    mid_get_ls_t sMIDLs;           /* Structure for MID_getLs() */
    mid_get_ke_t sMIDKe;           /* Structure for MID_getKe() */
    mid_get_pp_t sMIDPp;           /* Structure for MID_getPp() */
    mid_get_mech_t sMIDMech;       /* Structure for MID_getMech() */
    mid_calib_hall_t sMIDHall;     /* Structure for MID_calibHall() */

    uint16_t ui16EnableMeasurement; /* Enables measurement in superior machine */

    enum
    {
        kMID_Calibration      = 0,
        kMID_PolePairs        = 1,
        kMID_ElectricalParams = 2,
        kMID_MechanicalParams = 3,
        kMID_HallSensors      = 4
    } eMeasurementType;

    uint16_t ui16FaultMID;
    uint16_t ui16WarnMID;

    float_t fltFreqMIDLoop;

} mid_struct_t;

extern mid_struct_t g_sMID;

#endif /* _MID_DEF_H_ */
