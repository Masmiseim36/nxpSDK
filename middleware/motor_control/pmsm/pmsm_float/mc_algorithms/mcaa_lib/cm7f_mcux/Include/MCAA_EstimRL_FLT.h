/*
* Copyright 2021, 2024-2026 NXP
*
* NXP Proprietary. This software is owned or controlled by NXP and may
* only be used strictly in accordance with the applicable license terms. 
* By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that
* you have read, and that you agree to comply with and are bound by,
* such license terms.  If you do not agree to be bound by the applicable
* license terms, then you may not retain, install, activate or otherwise
* use the software.
*
*/
/**
*
* @file       MCAA_EstimRL_FLT.h
*
* @version    1.0.0.0
*
* @date       1-June-2021
*
* @brief      Header file for mcaa_estimrl function
*
******************************************************************************/
#ifndef MCAA_ESTIMRL_FLT_H_
#define MCAA_ESTIMRL_FLT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "gdflib_FP.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MCAA_ESTIMRL_FSN (10000u) /* maximal sampling frequency [Hz] */
#define MCAA_ESTIMRL_FSN_FLT (10000.0) /* maximal sampling frequency [Hz] in floating-point type */
#define MCAA_ESTIMRL_ACF (500.0f) /* Frequency of the injected AC current [Hz] */
#define MCAA_ESTIMRL_TIMEOUT (2u) /* Convergence detector timeout [s] */
#define MCAA_ESTIMRL_DCB_THR (10u) /* DC bus limit counter threshold */
#define MCAA_ESTIMRL_IDC (0u) /* Row of pLdqTable containing the DC current */
#define MCAA_ESTIMRL_LD (1u) /* Row of pLdqTable containing the d-axis inductance */
#define MCAA_ESTIMRL_LQ (2u) /* Row of pLdqTable containing the q-axis inductance */
#define MCAA_ESTIMRL_ROWS (3u) /* Number of rows in pLdqTable */
#define MCAA_FLOAT_1_OVER_PI (0.3183098861837907f) /* 1/pi */
#define MCAA_ESTIMRL_CD_LIML (-1000.0f) /* Convergence detector lower limit */
#define MCAA_ESTIMRL_CD_LIMH (-100.0f) /* Convergence detector coarse/fine tracking threshold */
#define MCAA_ESTIMRL_CD_COARSE_THR (0.1f) /* Convergence detector coarse threshold */
#define MCAA_ESTIMRL_CD_FINE_THR (3.90625e-03f) /* Convergence detector fine threshold */
#define MCAA_ESTIMRL_AC_IAMP (0.1f) /* AC current amplitude fraction of the maximal current */
#define MCAA_ESTIMRL_PLL_LAMBDA_DQ (0.05f) /* PLL d/q axis MA filter coefficient */
#define MCAA_ESTIMRL_PLL_LAMBDA_DC (0.05f) /* PLL DC current MA filter coefficient */
#define MCAA_ESTIMRL_PLL_LAMBDA_STD (0.01f) /* PLL current noise standard deviation MA filter coefficient */
#define MCAA_ESTIMRL_PLL_KP (50.0f) /* PLL phase controller proportional gain */
#define MCAA_ESTIMRL_DC_IRAMP (0.002f) /* DC current ramp factor */
#define MCAA_ESTIMRL_ACDC_KI (0.0001f) /* AC/DC voltage controller integral gain (for Idcmax = IDCMAX_NOMINAL) */
#define IDCMAX_NOMINAL (100.0f) /* Nominal value of the max. DC current for scaling of controller time constants */
#define UDCBUS_NOMINAL (12.0f) /* Nominal value of the DC bus voltage for scaling of controller time constants */
#define MCAA_ESTIMRL_DCB_ELIM_IDX (0.866025403784439f) /* DC bus eliminator inverse modulation index */
#define MCAA_ESTIMRL_AVG_MUL (300.0f) /* Averaging window length multiplier */
#define MCAA_ESTIMRL_AVG_MIN (0.4f) /* Averaging window minimum length [seconds] */
#define MCAA_ESTIMRL_AVG_MAX (1.0f) /* Maximum length of the noise averaging window [seconds]. Must be <= 1 for the fixed-point implementation. */

/* Default values for the advanced tuning parameters structure. */
#define ESTIMRL_ADV_TUNE_DEFAULT {\
  MCAA_ESTIMRL_ACF,\
  MCAA_ESTIMRL_TIMEOUT,\
  MCAA_ESTIMRL_DCB_THR,\
  MCAA_ESTIMRL_CD_LIML,\
  MCAA_ESTIMRL_CD_LIMH,\
  MCAA_ESTIMRL_CD_COARSE_THR,\
  MCAA_ESTIMRL_CD_FINE_THR,\
  MCAA_ESTIMRL_AC_IAMP,\
  MCAA_ESTIMRL_PLL_LAMBDA_DQ,\
  MCAA_ESTIMRL_PLL_LAMBDA_DC,\
  MCAA_ESTIMRL_PLL_LAMBDA_STD,\
  MCAA_ESTIMRL_PLL_KP,\
  MCAA_ESTIMRL_DC_IRAMP,\
  MCAA_ESTIMRL_ACDC_KI,\
  IDCMAX_NOMINAL,\
  UDCBUS_NOMINAL,\
  MCAA_ESTIMRL_DCB_ELIM_IDX,\
  MCAA_ESTIMRL_AVG_MUL,\
  MCAA_ESTIMRL_AVG_MIN,\
  MCAA_ESTIMRL_AVG_MAX}
  
#define MCAA_EstimRLInit_FLT_C(psParam, psCtrl, psAdvTune)    \
        MCAA_EstimRLInit_FLT_FC(psParam, psCtrl, psAdvTune)
#define MCAA_EstimRL_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, sParam, psAdvTune, psUAlBeReq) \
        MCAA_EstimRL_FLT_FC(fltUDcBus, psIAlBeFbck, psCtrl, sParam, psAdvTune, psUAlBeReq)

/*******************************************************************************
* Types
*******************************************************************************/
/* MCAA_EstimRLInit return value enum. */
typedef enum
{
  ESTIMRL_RET_INIT_OK = 0,              /* Initialization successful. */
  ESTIMRL_RET_INIT_ERROR = 1,           /* Invalid inputs. */
} MCAA_ESTIMRL_INIT_RET_T_FLT;

/* MCAA_EstimRL return value enum. */
typedef enum
{
  ESTIMRL_RET_IN_PROGRESS = 0,          /* Parameter estimation is in progress. The MCAA_EstimRL must be called again in the next sampling period. */
  ESTIMRL_RET_DONE = 1,                 /* Parameter estimation has finished. */
  ESTIMRL_RET_ERROR = 2,                /* Parameter estimation has failed. */
} MCAA_ESTIMRL_RET_T_FLT;

/* MCAA_EstimRL internal state enum. */
typedef enum
{
  ESTIMRL_STATE_UNINITIALIZED = 0,      /* RL estimator is not initialized. */
  ESTIMRL_STATE_MEAS_LD,                /* Measurement of d-axis inductance. */
  ESTIMRL_STATE_MEAS_LQ,                /* Measurement of q-axis inductance. */
  ESTIMRL_STATE_LD_TO_LQ,               /* Transition between axes. */
  ESTIMRL_STATE_POSTPROCESS_LD,         /* Postprocessing of accumulated values for d-axis measurement. */
  ESTIMRL_STATE_POSTPROCESS_LQ,         /* Postprocessing of accumulated values for q-axis measurement. */
  ESTIMRL_STATE_UNWIND,                 /* Gradual turn-off of the DC current at the end of measurement. */
  ESTIMRL_STATE_DONE,                   /* Measurement finished. */
  ESTIMRL_STATE_ERROR,                  /* Failure. */
} MCAA_ESTIMRL_STATE_T_FLT;

/* RL estimator fault enum, each bit of fault variable represents defined fault. */
typedef uint16_t estimrldef_fault_t; 
typedef enum
{
  ESTIMRL_FAULT_INIT_MISSING_FS = 0,    /* Missing sampling frequency */
  ESTIMRL_FAULT_INIT_FS_LIMIT,          /* Sampling frequency below limit */
  ESTIMRL_FAULT_INIT_I_DC_POS_LIMIT,    /* Invalid positive DC current */
  ESTIMRL_FAULT_INIT_I_DC_NEG_LIMIT,    /* Invalid negative DC current */
  ESTIMRL_FAULT_INIT_L_DQ_TABLE,        /* Invalid number of Ld/Lq table measurements */
  ESTIMRL_FAULT_INIT_I_DC_LD_POS_LIMIT, /* Ld measurement current exceedes maximal positive DC current */
  ESTIMRL_FAULT_INIT_I_DC_LD_NEG_LIMIT, /* Ld measurement current exceedes maximal negative DC current */
  ESTIMRL_FAULT_INIT_I_DC_LQ_POS_LIMIT, /* Lq measurement current exceedes maximal positive DC current */
  ESTIMRL_FAULT_INIT_I_DC_LQ_NEG_LIMIT, /* Lq measurement current exceedes maximal negative DC current */
  ESTIMRL_FAULT_INIT_FS_FINJ_RATIO,     /* Low sampling frequency (ratio to Hf current frequency) */
  ESTIMRL_FAULT_RUN_UNINITIALIZED,      /* Invalid usage, must call init first */
  ESTIMRL_FAULT_RUN_FS_FINJ_RATIO,      /* Low ratio between sampling frequency and Hf current frequency */
  ESTIMRL_FAULT_RUN_I_AC_FREQ,          /* Invalid AC current frequency */
} MCAA_ESTIMRL_FAULT_T_FLT;

/* Internal state of the RL estimator structure. */
typedef struct
{
  MCAA_ESTIMRL_STATE_T_FLT pState;      /* Status of electrical estimation internal state machine */
  estimrldef_fault_t pFault;            /* Faults of electrical estimation internal state machine */
  GMCLIB_2COOR_ALBE_T_FLT pIAlBeAc;     /* Alpha/beta components of the estimated AC current */
  GMCLIB_2COOR_SINCOS_T_FLT pSinCosEst; /* Sine/Cosine of the estimated current phase */
  GMCLIB_2COOR_DQ_T_FLT pIDQAcRaw;      /* d/q components of the estimated AC current */
  GMCLIB_2COOR_DQ_T_FLT pIDQAcFilt;     /* Filtered d/q components of the estimated AC current */
  GDFLIB_FILTER_MA_T_FLT pDAxisFilter;  /* Parameters structure of the MA filter in the d axis in the PLL */
  GDFLIB_FILTER_MA_T_FLT pQAxisFilter;  /* Parameters structure of the MA filter in the q axis in the PLL */
  GDFLIB_FILTER_MA_T_FLT pDcFilter;     /* Parameters structure of the MA filter filtering the DC current in PLL */
  GDFLIB_FILTER_MA_T_FLT pStdFilter;    /* Parameters structure of the MA filter filtering the standard deviation of current noise */
  float_t fltIDcMax;                    /* Maximum DC of current */
  float_t fltIDcNegMax;                 /* Maximum allowed negative DC current */
  float_t fltIDcMaxInv;                 /* Inverse of the maximum DC current */
  float_t fltIAcReq;                    /* Amplitude of the required AC current */
  float_t fltIAcReqInv;                 /* Inverse of the amplitude of the required AC current */
  float_t fltIDcDReq;                   /* Required DC current in D axis */
  float_t fltIDcDReqk_1;                /* Required DC current in previous step in D axis */
  float_t fltIDcQReq;                   /* Required DC current in Q axis */
  float_t fltIDcQReqk_1;                /* Required DC current in previous step in Q axis */
  float_t fltIDcReqStep;                /* Measurement step of the required DC current */
  float_t fltIAlBeEstErr;               /* Current estimation error */
  float_t fltPLLPropGain;               /* Gain of the P controller in the PLL */
  float_t fltPhaseShift;        /* Voltage/current phase shift */
  float_t fltUdcHalf;           /* Minimum DC bus voltage, halved and divided by modulation index */
  float_t fltFsInv;             /* Inverse of the sampling frequency */
  float_t fltImpConst;          /* Constant used for calculation of impedance */
  float_t fltLdk_1;             /* Estimated d-axis inductance in previous step */
  float_t fltLqk_1;             /* Estimated q-axis inductance in previous step */
  float_t fltIDcLd;             /* DC current level to use for measurement of scalar Ld */
  float_t fltIDcLq;             /* DC current level to use for measurement of scalar Lq */
  float_t fltUdtAcc;            /* Dead time voltage drop averaging accumulator */
  float_t fltRAcc;              /* Resistance averaging accumulator */
  float_t fltIDcRampCoef;       /* DC current ramp coefficient */
  float_t fltIDcRampCoefMax;    /* Maximum DC current ramp coefficient */
  float_t fltIDcDRampAcc;       /* DC current ramp accumulator in D axis */
  float_t fltIDcQRampAcc;       /* DC current ramp accumulator in Q axis */
  float_t fltUDcDAcc;           /* DC voltage integrator in D axis */
  float_t fltUDcDAccAvg;        /* Averaging accumulator for DC voltage in D axis */
  float_t fltUDcDAccAvgk_1;     /* Averaged DC voltage in D axis in previous step */
  float_t fltUDcQAcc;           /* DC voltage integrator in Q axis */
  float_t fltUAcAcc;            /* AC voltage integrator  */
  float_t fltUDcKi;             /* DC voltage integrator gain */
  float_t fltUAcKi;             /* AC voltage integrator gain */
  float_t fltUDcKiNominal;      /* Nominal DC voltage integrator gain */
  float_t fltUAcKiNominal;      /* Nominal AC voltage integrator gain */
  float_t fltPhAvg;             /* Averaging accumulator for phase shift */
  float_t fltIAcAvg;            /* Averaging accumulator for AC current */
  float_t fltUAcAvg;            /* Averaging accumulator for AC voltage */
  float_t fltAvgMul;            /* Averaging window length multiplier */
  float_t fltAvgMin;            /* Averaging window minimum length */
  float_t fltAvgNInv;           /* Reciprocal of the noise averaging inverse window length */
  float_t fltAvgNInvMaxDelta;   /* Difference of reciprocals of the noise averaging inverse window length (max/nominal) */
  float_t fltUDcBusMax;         /* Maximum observed DC bus voltage */
  float_t fltIDcD;              /* Estimated DC current in D axis */
  float_t fltIDcQ;              /* Estimated DC current in Q axis */
  float_t fltIDcDAvg;           /* Averaging accumulator for the estimated DC current in D axis */
  float_t fltIDcDAvgk_1;        /* Averaged estimated DC current in D axis in the last step */
  float_t fltPhComp;            /* Compensation of phase error due to sampling lag */
  frac32_t f32FAc;              /* Normalized fraction of the AC current frequency to be integrated in each iteration */
  frac32_t f32ThAc;             /* High-frequency AC voltage phase; <-PI, PI) range normalized into <-1, 1) */
  frac32_t f32ThEst;            /* Estimated AC current phase; <-PI, PI) range normalized into <-1, 1) */
  int32_t s32ConvDetState;      /* Convergence detector state */
  int32_t s32ConvDetStateLimL;  /* Convergence detector lower limit */
  int32_t s32ConvDetStateLimH;  /* Convergence detector coarse/fine tracking threshold */
  uint32_t u32ConvDetToutCnt;    /* Convergence detector timeout counter */
  uint32_t u32ConvDetToutReload; /* Convergence detector counter reload value */
  uint32_t u32AvgCnt;            /* Noise averaging counter */
  uint32_t u32AvgN;              /* Noise averaging window length */
  uint32_t u32TblCounter;       /* Ld/Lq table counter */
  uint32_t u32AvgCntMax;         /* Maximum length of the noise averaging window */
  uint16_t u16LdqNumMeas;       /* Number of Ld/Lq table measurements */
  uint16_t u16RampSteady;       /* Indicator of the steady state of the voltage controller ramp */
  uint16_t u16DcbLimitFlag;     /* DC bus voltage limitation indicator */
  uint16_t u16DcbLimitFlagk_1;  /* DC bus voltage limitation indicator in previous step */
  uint16_t u16DcbLimitCnt;      /* DC bus voltage limitation counter */
  uint16_t u16LdqNumMeasR;      /* Number of measurements of R */
  uint8_t u8RotorFixed;         /* 0 == rotor is not mechanically fixed, 1 == rotor is mechanically fixed */
} MCAA_ESTIMRL_INNERST_T_FLT;

/* Estimator configuration structure. */
typedef struct
{
  MCAA_ESTIMRL_INNERST_T_FLT pInnerState;  /* Internal state of the RL estimator. No user-editable values. */
  float_t fltLd;                /* Estimated d-axis inductance at zero DC current [H]. */
  float_t fltLq;                /* Estimated q-axis inductance at maximal DC current [H]. */
  float_t fltR;                 /* Estimated resistance [Ohm]. */
  float_t fltUdt;               /* Estimated dead time voltage drop of the power stage [V]. */
} MCAA_ESTIMRL_T_FLT;

/* Initialization parameters of the RL estimator structure. */
typedef struct
{
  uint32_t u32SamplingFreq;     /* Sampling frequency [1/s]. */
  float_t fltIDcMax;            /* Maximum DC current [A]. */
  float_t fltIDcLd;             /* DC current used for Ld measurement [A]. */
  float_t fltIDcLq;             /* DC current used for Lq measurement [A]. */
  float_t fltIDcNegMax;         /* Maximum allowed negative d-axis DC current [A]. Note that too large negative DC current can cause permanent damage to the motor. Setting fltIDcNegMax to a safe value prevents irreversible demagnetization of the PMSM's magnets. The value of fltIDcNegMax must be negative or zero. */
  uint16_t u16LdqNumMeas;       /* Number of measured d-axis DC current levels. If this parameter is not 1, the function will perform automatic measurement for a range of DC currents. Otherwise, only one measurement will be performed. */
} MCAA_ESTIMRL_INIT_T_FLT;

/* Run-time parameters of the RL estimator structure. */
typedef struct
{
  float_t *pLdqTable;   /* Pointer to a table with measured values. The table has 3 rows and u16LdqNumMeas columns stored in column-major order. Parameter u16LdqNumMeas of the init function MCAA_EstimRLInit_FLT determines the number of used DC current levels. The array pointed to by pLdqTable must provide room for 3*u16LdqNumMeas 32-bit values. The first row of the table contains the DC current levels, the second row contains the estimated d-axis inductances, and the third row contains the estimated q-axis inductances. The inductances are calculated in Henrys. */
  float_t fltIDcDReq;   /* Required DC current in the d-axis */
  float_t fltIDcQReq;   /* Required DC current in the q-axis */
  float_t fltIAcReq;    /* Amplitude of the required AC current */
  uint16_t u16FAc;      /* AC current frequency */
  uint8_t u8LdqSwitch;  /* Switches between Ld (u8LdqSwitch = 0) and Lq (u8LdqSwitch = 1) measurement */
} MCAA_ESTIMRL_RUN_T_FLT;

/* Parameters for advanced tuning algorithm. */
typedef struct
{
  float_t  fltACf;        // MCAA_ESTIMRL_ACF (500.0f) /* Frequency of the injected AC current [Hz] */
  uint16_t u16Timeout;    // MCAA_ESTIMRL_TIMEOUT (2u) /* Convergence detector timeout [s] */
  uint16_t  u16DCBusThr;  // MCAA_ESTIMRL_DCB_THR (10u) /* DC bus limit counter threshold */
  float_t  fltCdLimL;     // MCAA_ESTIMRL_CD_LIML (-1000.0f) /* Convergence detector lower limit */
  float_t  fltCdLimH;     // MCAA_ESTIMRL_CD_LIMH (-100.0f) /* Convergence detector coarse/fine tracking threshold */
  float_t  fltCdCoarseThr;        // MCAA_ESTIMRL_CD_COARSE_THR (0.1f) /* Convergence detector coarse threshold */ 
  float_t  fltCdFineThr;          // MCAA_ESTIMRL_CD_FINE_THR (3.90625e-03f) /* Convergence detector fine threshold */
  float_t  fltACIamp;     // MCAA_ESTIMRL_AC_IAMP (0.1f) /* AC current amplitude fraction of the maximal current */
  float_t  fltPllLambdaDQ;        // MCAA_ESTIMRL_PLL_LAMBDA_DQ (0.05f) /* PLL d/q axis MA filter coefficient */
  float_t  fltPllLambdaDC;        // MCAA_ESTIMRL_PLL_LAMBDA_DC (0.05f) /* PLL DC current MA filter coefficient */
  float_t  fltPllLambdaStd;       // MCAA_ESTIMRL_PLL_LAMBDA_STD (0.01f) /* PLL current noise standard deviation MA filter coefficient */
  float_t  fltPllKp;              // MCAA_ESTIMRL_PLL_KP (50.0f) /* PLL phase controller proportional gain */
  float_t  flt_DCIRamp;   // MCAA_ESTIMRL_DC_IRAMP (0.002f) /* DC current ramp factor */
  float_t  fltACDCKi;     // MCAA_ESTIMRL_ACDC_KI (0.0001f) /* AC/DC voltage controller integral gain (for Idcmax = IDCMAX_NOMINAL) */
  float_t  fltIDCMaxNom;  // IDCMAX_NOMINAL (100.0f) /* Nominal value of the max. DC current for scaling of controller time constants */
  float_t  fltUDCBusNom;  // UDCBUS_NOMINAL (12.0f) /* Nominal value of the DC bus voltage for scaling of controller time constants */
  float_t  fltDCBusElimIdx;       // MCAA_ESTIMRL_DCB_ELIM_IDX (0.866025403784439f) /* DC bus eliminator inverse modulation index */
  float_t  fltAvgMul;     // MCAA_ESTIMRL_AVG_MUL (300.0f) /* Averaging window length multiplier */
  float_t  fltAvgMin;     // MCAA_ESTIMRL_AVG_MIN (0.4f) /* Averaging window minimum length [seconds] */
  float_t  fltAvgMax;     // MCAA_ESTIMRL_AVG_MAX (1.0f) /* Maximum length of the noise averaging window [seconds]. Must be <= 1 for the fixed-point implementation. */
} MCAA_ESTIMRL_ADV_TUNE_T;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern MCAA_ESTIMRL_INIT_RET_T_FLT MCAA_EstimRLInit_FLT_FC(MCAA_ESTIMRL_INIT_T_FLT *psParam,
                                                           MCAA_ESTIMRL_T_FLT *const psCtrl,
                                                           MCAA_ESTIMRL_ADV_TUNE_T *psAdvTune);

extern MCAA_ESTIMRL_RET_T_FLT MCAA_EstimRL_FLT_FC(float_t fltUDcBus,
                                                  const GMCLIB_2COOR_ALBE_T_FLT *const pIAlBeFbck,
                                                  MCAA_ESTIMRL_T_FLT *const psCtrl,
                                                  MCAA_ESTIMRL_RUN_T_FLT *psParam,
                                                  MCAA_ESTIMRL_ADV_TUNE_T *psAdvTune,
                                                  GMCLIB_2COOR_ALBE_T_FLT *const pUAlBeReq);

/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* MCAA_ESTIMRL_FLT_H_ */