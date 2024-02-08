/******************************************************************************
*
 * Copyright 2021 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
******************************************************************************/
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
#define MCAA_EstimRLInit_FLT_C(u32SamplingFreq, psParam, psCtrl)               \
        MCAA_EstimRLInit_FLT_FC(u32SamplingFreq, psParam, psCtrl)
#define MCAA_EstimRL_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, sParam, psUAlBeReq) \
        MCAA_EstimRL_FLT_FC(fltUDcBus, psIAlBeFbck, psCtrl, sParam, psUAlBeReq)

/*******************************************************************************
* Types
*******************************************************************************/
/* MCAA_EstimRLInit return value enum. */
typedef enum
{
  RET_INIT_OK = 0,              /* Initialization successful. */
  RET_INIT_ERROR = 1,           /* Invalid inputs. */
} MCAA_ESTIMRLINIT_RET_T_FLT;

/* MCAA_EstimRL return value enum. */
typedef enum
{
  RET_IN_PROGRESS = 0,          /* Parameter estimation is in progress. The MCAA_EstimRL must be called again in the next sampling period. */
  RET_DONE = 1,                 /* Parameter estimation has finished. */
  RET_ERROR = 2,                /* Parameter estimation has failed. */
} MCAA_ESTIMRL_RET_T_FLT;

/* MCAA_EstimRL internal state enum. */
typedef enum
{
  STATE_UNINITIALIZED = 0,      /* RL estimator is not initialized. */
  STATE_MEAS_LD,                /* Measurement of d-axis inductance. */
  STATE_MEAS_LQ,                /* Measurement of q-axis inductance. */
  STATE_LD_TO_LQ,               /* Transition between axes. */
  STATE_POSTPROCESS_LD,         /* Postprocessing of accumulated values for d-axis measurement. */
  STATE_POSTPROCESS_LQ,         /* Postprocessing of accumulated values for q-axis measurement. */
  STATE_UNWIND,                 /* Gradual turn-off of the DC current at the end of measurement. */
  STATE_DONE,                   /* Measurement finished. */
  STATE_ERROR,                  /* Failure. */
} MCAA_ESTIMRL_STATE_T_FLT;

/* DC bus ripple eliminator structure. */
typedef struct{
  float_t fltArgDcBusMsr;       /* Measured DC bus voltage. */
  float_t fltModIndex;          /* Inverse Modulation Index. */
}GMCLIB_ELIMDCBUSRIP_T_FLT;

/* Internal state of the RL estimator structure. */
typedef struct
{
  MCAA_ESTIMRL_STATE_T_FLT pState;        /* RL estimator state */
  GMCLIB_2COOR_ALBE_T_FLT pIAlBeAc;     /* Alpha/beta components of the estimated AC current */
  GMCLIB_2COOR_SINCOS_T_FLT pSinCosEst; /* Sine/Cosine of the estimated current phase */
  GMCLIB_2COOR_DQ_T_FLT pIDQAcRaw;      /* d/q components of the estimated AC current */
  GMCLIB_2COOR_DQ_T_FLT pIDQAcFilt;     /* Filtered d/q components of the estimated AC current */
  GDFLIB_FILTER_MA_T_FLT pDAxisFilter;  /* Parameters structure of the MA filter in the d axis in the PLL */
  GDFLIB_FILTER_MA_T_FLT pQAxisFilter;  /* Parameters structure of pDcFilter in the PLL */
  GDFLIB_FILTER_MA_T_FLT pDcFilter;     /* Parameters structure of the MA filter filtering the DC current in PLL */
  GDFLIB_FILTER_MA_T_FLT pStdFilter;    /* Parameters structure of the MA filter filtering the standard deviation of current noise */
  GMCLIB_ELIMDCBUSRIP_T_FLT pElimDcPar;  /* Parameters structure of the DC bus ripple eliminator */
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

/****************************************************************************
* Exported function prototypes
****************************************************************************/
extern MCAA_ESTIMRLINIT_RET_T_FLT MCAA_EstimRLInit_FLT_FC(uint32_t u32SamplingFreq,
                                                          MCAA_ESTIMRL_INIT_T_FLT *psParam,
                                                          MCAA_ESTIMRL_T_FLT *const psCtrl);
extern MCAA_ESTIMRL_RET_T_FLT MCAA_EstimRL_FLT_FC(float_t fltUDcBus,
                                                  const GMCLIB_2COOR_ALBE_T_FLT *const pIAlBeFbck,
                                                  MCAA_ESTIMRL_T_FLT *const psCtrl,
                                                  MCAA_ESTIMRL_RUN_T_FLT *psParam,
                                                  GMCLIB_2COOR_ALBE_T_FLT *const pUAlBeReq);

/****************************************************************************
* Inline functions
****************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* MCAA_ESTIMRL_FLT_H_ */
