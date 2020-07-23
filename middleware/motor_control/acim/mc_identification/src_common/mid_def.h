/*
 * Copyright 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _MID_DEF_H_
#define _MID_DEF_H_

#include "m1_acim_appconfig.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "mlib_FP.h"
#include "calc_power.h"

#include "mcdrv.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* user-defined constants */
/* stator resistance measurement */
#define MID_RS_KP_GAIN (2.0F)       /* current controller proportional gain [-]*/
#define MID_RS_KI_GAIN (0.15F)      /* current controller integral gain [-]*/
#define MID_RS_MIN_CURR (0.05F)     /* minimal expected current when motor is connected [A] */
#define MID_RS_MEAS_MIN_LVL (0.05F) /* minimal voltage and current level for precise measurement [-] */
#define MID_RS_TIME_MEAS (1.0F)     /* duration of measurement [s] */
#define MID_RS_TIME_SETTLE (0.5F)   /* time after measurement for current to settle [s] */

/* no-load test */
#define MID_NL_TIME_URAMP_0KW5 (1.0F) /* voltage ramp-up time for motors under 0.5kW [s] */
#define MID_NL_TIME_URAMP_1KW5 (3.0F) /* voltage ramp-up time for motors under 1.5kW [s] */
#define MID_NL_TIME_URAMP_3KW0 (6.0F) /* voltage ramp-up time for motors under 3.0kW [s] */
#define MID_NL_TIME_MEAS (5.0F)       /* measurement duration [s] */
#define MID_NL_TIME_SETTLE (2.5F)     /* settle time after measurement [s] */

/* blocked-rotor test */
#define MID_BR_IRMS_KP_GAIN_0KW5 (1.00F)  /* rms current controller p-gain for motors under 0.5kW [-] */
#define MID_BR_IRMS_KI_GAIN_0KW5 (0.10F)  /* rms current controller i-gain for motors under 0.5kW [-] */
#define MID_BR_IRMS_KP_GAIN_1KW5 (0.50F)  /* rms current controller p-gain for motors under 1.5kW [-] */
#define MID_BR_IRMS_KI_GAIN_1KW5 (0.01F)  /* rms current controller i-gain for motors under 1.5kW [-] */
#define MID_BR_IRMS_KP_GAIN_3KW0 (0.50F)  /* rms current controller p-gain for motors under 3.0kW [-] */
#define MID_BR_IRMS_KI_GAIN_3KW0 (0.005F) /* rms current controller i-gain for motors under 3.0kW [-] */
#define MID_BR_TIME_MEAS (6.0F)           /* duration of measurement [s] */

/* mechanical parameters measurement */
#define MID_MECH_SPDEL_MIN_PCT (0.2F)   /* minimal speed for start of estimation [-] */
#define MID_MECH_SPDEL_MAX_PCT (0.5F)   /* speed to stop the acceleration [-]*/
#define MID_MECH_ID_DECEL_PCT (0.2F)    /* percentage of startup d-axis current for deceleration period [-]*/
#define MID_MECH_TIME_MEASMAX (5.0F)    /* maximal time of measurement [s] */
#define MID_MECH_TIME_SETTLEMAX (30.0F) /* maximal time for speed to drop [s] */

/* power stage characterization */
#define MID_PWRCHR_CURR_POINT_NUM \
    (65) /* number of points to be measured during characterization [33,65,129,...]. Must match MCAT settings. */
#define MID_PWRCHR_MEASTIME (0.25F) /* time for measurement of single point */
#define MID_PWRCHR_FILTCOEFF \
    (0.0309F) /* error voltage filter coefficient, calculated as 1-exp(-2*pi*smapling_period*filt_freq) */

/* parameter calculation */
#define MID_CALC_FW_BANDWIDTH (0.25F)    /* frequency bandwidth of flux-control loop */
#define MID_CALC_ID_MIN2MAX_RATIO (0.3F) /* d-axis current minimum to maximum ratio */
#define MID_CALC_SPDREQ_MAX (1.0F)       /* maximal required speed in percentage of nominal speed */
#define MID_CALC_SPDREQ_MIN (0.2F)       /* minimal required speed in percentage of nominal speed */

/* do not change following constants */
/* MID fault codes */
#define MID_FAULT_ABORT (1)
#define MID_FAULT_NO_MOTOR (2)
#define MID_FAULT_TOO_HIGH_RS (4)
#define MID_FAULT_MECH_TIMEOUT (8)
/* MID warning codes */
#define MID_WARN_DC_CUR_NOT_REACHED (1)
#define MID_WARN_U_RMS_NOT_REACHED (2)
#define MID_WARN_I_RMS_NOT_REACHED (4)
#define MID_WARN_TOO_LOW_PREC (8)

/* MID motor parameters structure */
typedef struct
{
    /* known motor parameters */
    float_t fltPwrN;
    float_t fltIphN;
    float_t fltUphN;
    float_t fltFreqN;
    float_t fltPp;

    /* estimated parameters */
    float_t fltRs;      /* estimated stator resistance [ohm] */
    float_t fltRr;      /* estimated rotor resistance [ohm] */
    float_t fltLs;      /* estimated stator inductance [H] */
    float_t fltLr;      /* estimated rotor inductance [H] */
    float_t fltLm;      /* estimated mutual inductance [H] */
    float_t fltJ;       /* estimated moment of inertia [kgm2] */
    float_t fltTauMech; /* estimated mechanical time constant [s]*/
} mid_acim_prms_a1_t;

/* ACIM control algorithm structure */
typedef struct
{
    /* current controller parameters */
    float_t fltIdPIKp; /* proportional gain of d-axis PI controller */
    float_t fltIdPIKi; /* integral gain of d-axis PI controller */
    float_t fltIqPIKp; /* proportional gain of q-axis PI controller */
    float_t fltIqPIKi; /* integral gain of q-axis PI controller */

    /* speed controller parameters */
    float_t fltSpdPIKp;     /* proportional gain of speed PI controller */
    float_t fltSpdPIKi;     /* integral gain of speed PI controller */
    float_t fltSpdMeReqMin; /* required speed minimum */
    float_t fltSpdMeReqMax; /* required speed maximum */
    float_t fltIMax;        /* maximal q-axis stator current */

    /* flux control parameters */
    float_t fltFWPIKp;  /* proportional gain of flux-weakening PI controller */
    float_t fltFWPIKi;  /* integral gain of flux-weakening PI controller */
    float_t fltIdStart; /* startup d-axis current */
    float_t fltIdMin;   /* minimal d-axis current */
    float_t fltIdMax;   /* maximal d-axis current */

    /* MRAS parameters */
    float_t fltMRASPsiRA1Gain; /* constant determined by: Tau_r / (Tau_r+Ts) */
    float_t fltMRASPsiRB1Gain; /* constant determined by: fltPsiRA1Gain * Ts * Lm / Tau_r */
    float_t fltGainSpd;        /* mechanical speed calculation constant */

    /* RFO parameters */
    float_t fltPsiRA1Gain;      /* constant determined by: Tau_r / (Tau_r+Ts) */
    float_t fltPsiRB1Gain;      /* constant determined by: Ts*Lm / (Tau_r+Ts) */
    float_t fltPsiSA1Gain;      /* constant determined by: Tau_1 / (Tau_r+Ts) */
    float_t fltPsiSA2Gain;      /* constant determined by: Tau_1*Ts / (Tau_r+Ts) */
    float_t fltKrInvGain;       /* constant determined by: Lr / Lm */
    float_t fltKrLsTotLeakGain; /* constant determined by: Ls * (1-Lm*Lm/(Ls*Lr)) */
    float_t fltRsEst;           /* estimated stator resistance [ohm] */
    float_t fltGainTrq;         /* torque calculation constant */

    /* open-loop scalar control */
    float_t fltGainSpdMe2PosEl; /* scalar integration gain */
    float_t fltGainRpm2Volt;    /* scalar V/rpm gain */
    bool_t bFlagDTComp;         /* dead-time compensation flag */

} mid_acim_alg_prms_a1_t;

/* MID I/O structure - WARNING all pointers must be initialized */
typedef struct
{
    GMCLIB_3COOR_T_FLT *psIABC; /* pointer to phase currents [A] */
    float_t *pfltTorque;        /* pointer to torque estimation [Nm] */
    float_t *pfltSpeedEl;       /* pointer to mechanical speed estimation [rad/s] */
    float_t *pfltFreqReq;       /* pointer to required scalar control frequency [Hz] */
    float_t *pfltId;            /* pointer to actual value of d-axis current [A] */
    float_t *pfltIdReq;         /* pointer to required d-axis current [A] */
    float_t *pfltIq;            /* pointer to actual value of d-axis current [A] */
    float_t *pfltIqReq;         /* pointer to required q-axis current [A] */
    float_t *pfltUdReq;         /* pointer to required d-axis voltage [A] */
    float_t *pfltUDCbus;        /* pointer to filtered DC-bus voltage voltage [V] */
} mid_io_a1_t;

/* power stage characterization */
typedef struct
{
    float_t fltICal;    /* range of the characterization is from -fltICal to +fltICal [A] */
    float_t fltIReqAct; /* actual current at which is the characteristic being measured at the moment [A] */
    float_t pfltUErrLUT[MID_PWRCHR_CURR_POINT_NUM]; /* error voltages [V] */
    float_t fltIInc;                                /* current increment after each 300ms [A] */
    float_t fltRs;                                  /* stator resistance of calibration motor [ohm] */
    uint16_t ui16LUTId;       /* look-up table index, counts until a whole characteristic is measured */
    uint16_t ui16NumOfChPnts; /* number of points in LUT */
    uint16_t ui16Active;      /* indicates whether transfer characteristic is being measured (true) or not (false) */
} mid_get_char_a1_t;

/* stator resistance measurement */
typedef struct
{
    float_t fltIMeas;        /* user defined measuring DC current [A] */
    float_t fltILPF;         /* filtered measured DC current [A] */
    uint32_t ui32TimeMeas;   /* measurement duration */
    uint32_t ui32TimeSettle; /* current settling duration */
    uint16_t ui16Active;     /* indicates whether Rs is being measured (true) or not (false) */
} mid_get_rs_a1_t;

/* no-load measurement */
typedef struct
{
    GFLIB_RAMP_T_FLT sUMeasRmpPar; /* measurement voltage ramp */
    float_t fltUMeas;              /* scalar control stator voltage [V] */
    float_t fltUMeasMax;           /* maximal reached scalar control stator voltage [V] */
    float_t fltFMeas;              /* scalar control frequency [Hz] */
    float_t fltUrms;               /* measured rms of stator voltage [V] */
    float_t fltIrms;               /* measured rms of stator current [V] */
    float_t fltP;                  /* measured real power on motor [W] */
    float_t fltQ;                  /* measured reactive power on motor [var] */
    uint32_t ui32TimeMeas;         /* measurement duration */
    uint32_t ui32TimeSettle;       /* rotor settling duration */
    uint16_t ui16Active;           /* indicates whether no-load test is active (true) or not (false) */
} mid_test_noload_a1_t;

/* blocked-rotor test */
typedef struct
{
    GFLIB_CTRL_PI_P_AW_T_FLT sIrmsPIPar; /* stator current PI controller */
    float_t fltIrmsMeas;                 /* rms of measurement current [A] */
    float_t fltUMeas;                    /* required stator voltage [V] */
    float_t fltFMeas;                    /* scalar control frequency [Hz] */
    float_t fltUrms;                     /* stator average phase rms voltage [V] */
    float_t fltIrms;                     /* stator average phase rms current [A] */
    float_t fltP;                        /* average stator reql power [W] */
    float_t fltQ;                        /* average stator reactive power [var] */
    uint32_t ui32TimeMeas;               /* time of measurement */
    uint16_t ui16Active;                 /* indicates whether blocked rotor test is active (true) or not (false) */
    bool_t bIrmsPISatFlg;                /* PI controller saturation flag */
} mid_test_blocked_a1_t;

/* MID mechanical parameters structure */
typedef struct
{
    enum
    {
        kMID_MechInit       = 0,
        kMID_MechStartUp    = 1,
        kMID_MechAccelerate = 2,
        kMID_MechDecelerate = 3,
        kMID_MechEndOfMeas  = 4
    } eState;                   /* mechanical parameters measurement state-machine */
    float_t fltTrqAccl;         /* torque generated during acceleration phase */
    float_t fltIqAccl;          /* q-axis current required value */
    float_t fltSpdElEnd;        /* end-of-acceleration speed */
    float_t fltSpdElStart;      /* minimal speed to enter acceleration */
    float_t fltSpdElMax;        /* start of mechanical time constant measurement */
    float_t fltSpdLPF;          /* speed low pass filter */
    float_t fltTrqLPF;          /* torque low pass filter */
    float_t fltLPFCoeff;        /* speed and torque low pass filter coeff */
    float_t fltTrqInteg;        /* torque inetgrator */
    float_t fltSpdInteg;        /* speed inetgrator */
    float_t fltGainTrq;         /* torque constant */
    float_t fltFreqSLoop;       /* speed loop bandwidth frequency */
    uint32_t ui32TimeMeasMax;   /* acceleration measurement time limit */
    uint32_t ui32TimeSettleMax; /* deceleration measurement time limit */
    uint16_t ui16Active;        /* indicates whether measurement is active (true) or not (false) */

} mid_mech_a1_t;

/* measurement global structure */
typedef struct
{
    mid_acim_prms_a1_t sPar;
    mid_acim_alg_prms_a1_t sAlgNew, sAlgBck;
    mid_io_a1_t sIO;
    mid_get_char_a1_t sPwrStgCh;
    mid_get_rs_a1_t sRs;
    mid_test_noload_a1_t sNoLoad;
    mid_test_blocked_a1_t sBlocked;
    mid_mech_a1_t sMech;
    mid_calc_power_a1_t sCalcPwr;

    enum
    {
        kMID_CalcWait    = 0,
        kMID_CalcWorking = 1,
        kMID_CalcDone    = 2
    } eCalcElPar,
        eCalcMechPar;

    float_t fltFreqCLoop;
    float_t fltFreqSLoop;

    uint32_t ui32LoopCntr;
    uint16_t ui16FaultMID;
    uint16_t ui16WarningMID;
    uint16_t ui16EnableMeas;

    bool_t bAbort;         /* sets abort fault flag */
    bool_t bRotBlocked;    /* blocked rotor flag (set by MCAT) */
    bool_t bCalib;         /* calibration flag */
    bool_t bMeasSuccDone;  /* calibration flag */
    bool_t bParRestoreOld; /* restore previous parameters command */
    bool_t bParUpdateNew;  /* update parameters command */
} mid_struct_a1_t;

extern mid_struct_a1_t g_sMID;

#endif /* _MID_DEF_H_ */
