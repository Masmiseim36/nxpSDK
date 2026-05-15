/*
* Copyright 2026 NXP
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
* @file       MCAA_PpAssist_FLT.h
*
* @version    1.0.0.0
*
* @date       26-January-2026
*
* @brief      Header file for mcaa_ppassist functions
*
******************************************************************************/
#ifndef MCAA_PP_ASSIST_H_
#define MCAA_PP_ASSIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib_FP.h"
#include "gmclib_FP.h"

/*******************************************************************************
* Macros
*******************************************************************************/
#define MCAA_PpAssistInit_FLT_C(psParam, psCtrl)               \
        MCAA_PpAssistInit_FLT_FC(psParam, psCtrl)
#define MCAA_PpAssist_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq) \
        MCAA_PpAssist_FLT_FC(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq)
            
/*******************************************************************************
 * Types
 ******************************************************************************/   
/* MCAA_PpAssistInit return value enum. */
typedef enum
{
  PPASSIST_RET_INIT_OK = 0,                    /* Initialization successful. */
  PPASSIST_RET_INIT_ERROR = 1,                 /* Invalid inputs. */
} MCAA_PPASSIST_INIT_RET_T_FLT;

/* MCAA_PpAssist return value enum. */
typedef enum
{
  PPASSIST_RET_IN_PROGRESS = 0,                /* Assistant is in progress. The MCAA_PpAssist must be called again in the next sampling period. */
  PPASSIST_RET_DONE = 1,                       /* Assistant has finished (user passed determined number of pole-pairs to the assistant's structure). */
  PPASSIST_RET_ERROR = 2,                      /* Assistant has failed. */
} MCAA_PPASSIST_RET_T_FLT;

/* Pp assistant fault enum, each bit of fault variable represents defined fault. */
typedef uint8_t ppassistdef_fault_t; 
typedef enum
{
  PPASSIST_FAULT_INIT_MISSING_I_D = 0,         /* Missing openloop d-axis current */
  PPASSIST_FAULT_INIT_MISSING_F_EL,            /* Missing required electrical speed */
  PPASSIST_FAULT_INIT_MISSING_FS,              /* Missing sampling frequency */
  PPASSIST_FAULT_INIT_DC_LIMIT,                /* Duty cycle in frac exceeds limit */
  PPASSIST_FAULT_RUN_UNINITIALIZED,            /* Invalid usage, must call init first */
} MCAA_PPASSIST_FAULT_T_FLT;

/* Pp assistant internal state enum. */
typedef enum
{
  PPASSIST_STATE_UNINITIALIZED = 0,            /* PpAssistant is not initialized. */
  PPASSIST_STATE_ROTATE,                       /* Rotor rotating. */
  PPASSIST_STATE_DONE,                         /* Assistant finished. */
  PPASSIST_STATE_ERROR,                        /* Failure. */
} MCAA_PPASSIST_STATE_T_FLT;

/* Pp assistant structure. */
typedef struct
{
  MCAA_PPASSIST_STATE_T_FLT pState;             /* Status of pole-pair assistant */
  ppassistdef_fault_t       pFault;             /* Fault of pole-pair assist */
  GFLIB_RAMP_T_FLT          sFreqElRampParam;   /* Ramp Up + Down coefficients for f16Speed. */
  GFLIB_INTEGRATOR_T_A32    sFreqIntegrator;    /* Speed integrator coefficients. */
  float_t                   fltFreqElReq;       /* Required Electrical Speed. */
  float_t                   fltIdReqOpenLoop;   /* Openloop current. */
  float_t                   fltFreqMax;         /* Frequency scale. */
  float_t                   fltFreqElRamp;      /* Ramped f16SpeedElReq, this speed is integrated to get position. */
  uint16_t                  ui16PpDetermined;   /* Indicates whether the user already set pp in MCAT (true) or not yet (false). */
  uint16_t                  ui16WaitingSteady;  /* Indicates that motor is waiting in steady state (when electrical position is zero). */
  uint16_t                  ui16LoopCounter;    /* Serves for timing to determine e.g. 300ms. */
  frac16_t                  f16ThEl;            /* Rotor angular position */
  frac16_t                  f16ThElCurrent;     /* Current value of electrical position. */
  frac16_t                  f16ThElLast;        /* Last value of electrical position. */
  uint32_t                  ui32ZeroPosTime;    /* Time spent in zero position after overflow [s] */
  /* Current control */                       
  GMCLIB_2COOR_DQ_T_FLT     pUDQReq;            /* Required DQ voltage */
  GFLIB_CTRL_PI_P_AW_T_FLT  pPIpAWD;            /* D-axis ControllerPIpAW paremeters structure. */
  GFLIB_CTRL_PI_P_AW_T_FLT  pPIpAWQ;            /* Q-axis ControllerPIpAW paremeters structure. */
  GMCLIB_2COOR_DQ_T_FLT     pIDQReq;            /* Required DQ current structure. */
  GMCLIB_2COOR_DQ_T_FLT     pIDQFbck;           /* Feedback DQ current structure. */
  GMCLIB_2COOR_SINCOS_T_FLT pThTransform;       /* Sine and cosine of rotor angular position */
  float_t                   fltDutyCycleLimit;  /* Maximum allowable duty cycle in frac [-] */
} MCAA_PPASSIST_T_FLT;

/* Pp assistant init structure. */
typedef struct
{
  uint32_t  u32SamplingFreq;    /* Sampling frequency [1/s]. */
  float_t   fltIdReqOpenLoop;   /* Openloop current [A]. */
  float_t   fltFreqElReq;       /* Required electrical speed [Hz]. */
  /* Optional (advanced) parameters */
  float_t   fltRampTime;        /* Frequency ramp time [s]. */
  float_t   fltZeroPosTime;     /* Steady position time [s]. */
  float_t   fltUMax;            /* Maximal motor voltage [V]. */
  float_t   fltDutyCycleLimit;  /* Maximum allowable duty cycle in frac [-]. */
  float_t   fltDPiPropGain;     /* Proportional gain of the D-axis current loop controller [-]. */
  float_t   fltDPiIntegGain;    /* Integral gain of the D-axis current loop controller [-]. */
  float_t   fltQPiPropGain;     /* Proportional gain of the Q-axis current loop controller [-]. */
  float_t   fltQPiIntegGain;    /* Integral gain of the Q-axis current loop controller [-]. */
} MCAA_PPASSIST_INIT_T_FLT;

/*******************************************************************************
 * Exported function prototypes
 ******************************************************************************/
extern MCAA_PPASSIST_INIT_RET_T_FLT MCAA_PpAssistInit_FLT_FC(MCAA_PPASSIST_INIT_T_FLT *psParam,
                                                             MCAA_PPASSIST_T_FLT *const pcCtrl);
     
extern MCAA_PPASSIST_RET_T_FLT MCAA_PpAssist_FLT_FC(float_t fltUDcBus, 
                                                    const GMCLIB_2COOR_ALBE_T_FLT *const pIAlBeFbck,
                                                    MCAA_PPASSIST_T_FLT *psCtrl,
                                                    GMCLIB_2COOR_ALBE_T_FLT *const pUAlBeReq);

/*******************************************************************************
* Inline functions
*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif/* MCAA_PP_ASSIST_H_ */