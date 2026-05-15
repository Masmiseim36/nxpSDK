/*
* Copyright 2025-2026 NXP
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
/**
*
* @file       MCAA_EstimBJ_FLT.h
*
* @version    1.0.0.0
*
* @date       6-October-2025
*
* @brief      Header file for mcaa_estimbj functions
*
******************************************************************************/
#ifndef MCAA_ESTIMBJ_FLT_H_
#define MCAA_ESTIMBJ_FLT_H_

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "amclib_FP.h"
#include "gflib_FP.h"
#include "gmclib_FP.h"
#include "mlib_FP.h"    

/*******************************************************************************
* Macros
*******************************************************************************/
#define MCAA_EstimBJInit_FLT_C(psParam, psCtrl)               \
        MCAA_EstimBJInit_FLT_FC(psParam, psCtrl)
#define MCAA_EstimBJ_FLT_C(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq) \
        MCAA_EstimBJ_FLT_FC(fltUDcBus, psIAlBeFbck, psCtrl, psUAlBeReq)

/*******************************************************************************
 * Types
 ******************************************************************************/   
/* MCAA_EstimBJ_Init return value enum. */
typedef enum
{
  ESTIMBJ_RET_INIT_OK = 0,                    /* Initialization successful. */
  ESTIMBJ_RET_INIT_ERROR = 1,                 /* Invalid inputs. */
} MCAA_ESTIMBJ_INIT_RET_T_FLT;

/* MCAA_EstimBJ return value enum. */
typedef enum
{
  ESTIMBJ_RET_IN_PROGRESS = 0,                /* Parameter estimation is in progress. The MCAA_EstimBJ must be called again in the next sampling period. */
  ESTIMBJ_RET_DONE = 1,                       /* Parameter estimation has finished. */
  ESTIMBJ_RET_ERROR = 2,                      /* Parameter estimation has failed. */
} MCAA_ESTIMBJ_RET_T_FLT;

/* MCAA_EstimBJ internal state enum. */
typedef enum
{
  ESTIMBJ_STATE_UNINITIALIZED = 0,            /* BJ estimator is not initialized. */
  ESTIMBJ_STATE_ROTOR_ALIGN,                  /* Rotor alignment. */
  ESTIMBJ_STATE_OL_STARTUP,                   /* Open loop startup. */
  ESTIMBJ_STATE_FLUX_ESTIM,                   /* Closed loop control. */
  ESTIMBJ_STATE_FREQ_SELECT,                  /* Selection of injected frequency. */
  ESTIMBJ_STATE_PARAM_EST_MEAS,               /* Measurement of mechanical parameters. */
  ESTIMBJ_STATE_DONE,                         /* Measurement finished. */
  ESTIMBJ_STATE_ERROR,                        /* Failure. */
} MCAA_ESTIMBJ_STATE_T_FLT;

/* BJ estimator fault enum, each bit of fault variable represents defined fault. */
typedef uint16_t estimbjdef_fault_t; 
typedef enum
{
  ESTIMBJ_FAULT_INIT_MISSING_PP = 0,          /* Pole-pairs are not in valid range. */
  ESTIMBJ_FAULT_INIT_MISSING_RL_PARAMS,       /* Missing electrical parameters. */
  ESTIMBJ_FAULT_INIT_MISSING_I_MEAS,          /* Missing measurement current. */
  ESTIMBJ_FAULT_INIT_MISSING_N_NOM,           /* Missing nominal speed. */
  ESTIMBJ_FAULT_INIT_MISSING_FS,              /* Missing sampling frequency. */
  ESTIMBJ_FAULT_INIT_I_OL_LIMIT,              /* Open-loop current exceeds measurement current. */
  ESTIMBJ_FAULT_INIT_N_LIMIT,                 /* Nominal speed out of limits. */
  ESTIMBJ_FAULT_INIT_N_REQ_1_LIMIT,           /* Required speed for point 1 exceeds nominal speed. */
  ESTIMBJ_FAULT_INIT_N_REQ_2_LIMIT,           /* Required speed for point 2 exceeds nominal speed. */
  ESTIMBJ_FAULT_RUN_UNINITIALIZED,            /* Invalid usage, must call init first. */
  ESTIMBJ_FAULT_RUN_SS_TIMEOUT,               /* Could not reach steady state. */
  ESTIMBJ_FAULT_RUN_F_INJ,                    /* Could not found correct injected frequency. */
} MCAA_ESTIMBJ_FAULT_T_FLT;

/* Exponentially weighted moving average filter (EWMA) structure */
typedef struct{
  float_t  fltAcc;                          /* Float filter accumulator. */
  float_t  fltLambda;                       /* Float value of averaging, defines smoothing to changes. In range (0;1) .*/
}ESTIMBJ_FILTER_EWMA_T_FLT;

/* CurrentLoop configuration structure. */
typedef struct
{
  GFLIB_CTRL_PI_P_AW_T_FLT pPIpAWD;           /* D-axis ControllerPIpAW parameters structure. */
  GFLIB_CTRL_PI_P_AW_T_FLT pPIpAWQ;           /* Q-axis ControllerPIpAW parameters structure. */
  GMCLIB_2COOR_DQ_T_FLT *pIDQReq;             /* Pointer to the structure with the required current. */
  GMCLIB_2COOR_DQ_T_FLT *pIDQFbck;            /* Pointer to the structure with the feedback current. */
} MCAA_CURRENT_LOOP_T_FLT;

/* Internal state of the BJ estimator structure. No user-editable values.*/
typedef struct
{
  MCAA_ESTIMBJ_STATE_T_FLT pState;            /* Mechanical parameters estimator state */
  estimbjdef_fault_t pFault;                  /* Faults of mechanical estimation internal state machine */
  bool_t bTwoPointMeas;                       /* Indicator of the advanced mode measurement */
  uint32_t u32AlignTimer;                     /* Rotor alignment timer */
  uint32_t u32TimerPreset;                    /* Rotor alignment timer */
  bool_t bFoundInjFreq;                       /* Indicator that injected frequency was found */
  float_t fltIReq;                            /* Required current in open loop */
  float_t fltNDcReq1;                         /* Required DC speed point 1 for measurement */
  float_t fltNDcReq2;                         /* Required speed for measurement point 2 ("Advanced mode") */
  float_t fltNAcReq;                          /* Required amplitude of the injected AC speed */
  float_t fltNMax;                            /* Maximal speed */
  float_t fltPP;                              /* Number of pole pairs */
  float_t fltTs;                              /* Sampling period */
  float_t fltFInj;                            /* Injected frequency */
  float_t fltFInjMax;                         /* Maximum possible injection frequency */                          
  float_t fltFInjMin;                         /* Minimum possible injection frequency */
  float_t fltFInjInc;                         /* Injection frequency calculation increment */
  float_t fltFInjDec;                         /* Injection frequency calculation decrement */
  float_t fltNInj;                            /* Injected angular frequency */
  /* Open Loop */                             
  float_t fltNOl;                             /* Open loop speed */
  float_t fltThOl;                            /* Open loop position */
  GFLIB_RAMP_T_FLT pOLRamp;                   /* Parameter structure for open loop ramp */
  GFLIB_INTEGRATOR_T_A32 pOLInteg;            /* Parameter structure for open loop integrator */
  ESTIMBJ_FILTER_EWMA_T_FLT pIqMAFilter;      /* Parameter structure for open loop MA filter */
  GMCLIB_2COOR_SINCOS_T_FLT pThRealTransform; /* Structure to contain sine and cosine value of real rotor position */
  GMCLIB_2COOR_DQ_T_FLT pIDQRealFbck;         /* Real DQ feedback currents */
  /* Speed modulation */                      
  float_t fltNDcReq;                          /* Required DC speed */
  float_t fltNAcMaxReq;                       /* Required amplitude of AC speed */
  float_t fltIqDcReq;                         /* Required Q current based on DC speed regulator */
  float_t fltIqAcReq;                         /* Required Q current amplitude based on AC speed regulator */
  float_t fltIqAcMaxReq;                      /* Required Q current based on AC speed regulator */
  GFLIB_RAMP_T_FLT pNDcReqRamp;               /* Ramp for required DC speed */
  GFLIB_RAMP_T_FLT pNAcMaxReqRamp;            /* Ramp for required AC speed */
  GFLIB_CTRL_PI_P_AW_T_FLT pNDcPI;            /* DC speed controller */
  GFLIB_CTRL_PI_P_AW_T_FLT pNAcMaxPI;         /* AC speed controller */ 
  float_t fltNAcMaxErr;                       /* Input error for AC speed controller */
  float_t fltNDcErr2;                         /* Input error for DC speed controller */
  float_t fltNDcErr;                          /* DC speed error */
  frac32_t f32Theta;                          /* Modulated AC signals phase */
  /* Current control */                       
  GMCLIB_2COOR_DQ_T_FLT pUDQReq;              /* Required DQ voltage */
  MCAA_CURRENT_LOOP_T_FLT pCurrentLoop;       /* Parameter structure for current loop */  
  GMCLIB_2COOR_DQ_T_FLT pIDQReqCLoop;         /* Required DQ current */
  GMCLIB_2COOR_DQ_T_FLT pIDQFbckCLoop;        /* Measured DQ current */  
  float_t fltThEl;                            /* Rotor angular position */
  GMCLIB_2COOR_SINCOS_T_FLT pThTransform;     /* Sine and cosine of rotor angular position */
  /* PLL */                                   
  float_t fltDcRaw;                           /* Demodulated DC speed */
  GMCLIB_2COOR_ALBE_T_FLT pNABAcRaw;          /* Demodulated AC speed */
  GMCLIB_2COOR_SINCOS_T_FLT pPllSinCosEst;    /* Structure to store sine and cosine of PLL's internal phase */
  float_t fltDcFilt;                          /* Filtered demodulated DC speed */
  float_t fltAcFilt;                          /* Filtered demodulated AC speed */
  float_t fltNABEstErr;                       /* PLL error */
  ESTIMBJ_FILTER_EWMA_T_FLT pPllDMAFilter;    /* MA filter for D component of AC speed */
  ESTIMBJ_FILTER_EWMA_T_FLT pPllQMAFilter;    /* MA filter for Q component of AC speed */
  ESTIMBJ_FILTER_EWMA_T_FLT pPllDcMAFilter;   /* MA filter for DC speed */
  float_t fltPllPiPropGain;                   /* Proportional gain of PLL's P controller */
  frac32_t f32ThAcEst;                        /* PLL's internal phase with correction from P controller */
  frac32_t f32ThAcInc;                        /* PLL's internal phase */
  float_t fltAcOffset;                        /* Detected offset of AC speed */
  /* Flux estimator */                        
  float_t fltFluxEstimRs;                     /* Stator resistance [Ohm] for flux estimator */
  float_t fltFluxEstimLs;                     /* Stator inductance [H] for flux estimator */
  float_t fltLd;                              /* D inductance of the motor */
  float_t fltLq;                              /* Q inductance of the motor */
  float_t fltNEl;                             /* State variable to store motor speed */
  float_t fltThErr;                           /* Position error for ATO */
  float_t fltThEstim;                         /* Estimated angular position filtered by ATO */
  float_t fltNEstim;                          /* Estimated speed filtered by ATO */
  float_t fltTmpThErr;                        /* Position error for ATO */
  float_t fltTmpThEstim;                      /* Estimated angular position filtered by ATO */
  float_t fltTmpNEstim;                       /* Estimated speed filtered by ATO */
  AMCLIB_TRACK_OBSRV_T_FLT  pTrackObsrv;      /* Parameter structure for ATO */
  float_t fltKeRaw;                           /* Estimated rotor flux */
  float_t fltUdt;                         /* Dead time voltage drop */
  ESTIMBJ_FILTER_EWMA_T_FLT pFluxMAFilter;    /* Parameter structure for MA filter for flux filtering */
  /* Steady state */                          
  uint32_t u32SSTimeoutCnt;                   /* Steady state timeout counter */
  uint32_t u32SSBandCnt;                      /* Steady state in band counter */
  uint32_t u32SSTimeoutTime;                  /* Time to reach steady state before timeout */
  uint32_t u32SSTime;                         /* Steady state set time */
  uint32_t u32SSTimeMin;                      /* Minimal time needed for stable behavior */
  uint32_t u32SSTimeTrans;                    /* Transition time during selection of injected frequency */
  float_t fltSSNormRatio;                     /* Normalization ratio for FreeMASTER */
  float_t fltSSBandHalfMax;                   /* Half of maximal deviation from mean steady value */
  float_t fltSSBandHalf;                      /* Half of allowed deviation from mean steady value */
  float_t fltSSInParam;                       /* Observed variable in steady state */
  /* Measured data */                         
  float_t fltInertia;                         /* Actual value of moment of inertia */
  float_t fltMeDcArr[2];                      /* Array to store measured values of Me */
  float_t fltNDcArr[2];                       /* Array to store measured values of W */
  float_t fltInertiaArr[2];                   /* Array to store measured values of J */
  float_t fltGainFOC;                         /* Correction coefficient for FOC gain */
  float_t fltInvGainATO;                      /* Correction coefficient for ATO gain */
  float_t fltPhShiftCorr;                     /* Correction coefficient for FOC and ATO phase shift */
  uint16_t u16MeasNum;                        /* Number of current measurement */
} MCAA_ESTIMBJ_INNERST_T_FLT;

/* Initialization parameters of the BJ estimator structure. */
typedef struct
{
  uint32_t u32SamplingFreq;                   /* Sampling frequency [1/s]. */
  uint32_t ui32Pp;                            /* Number of pole-pairs [-]. */
  float_t fltIMeas;                           /* Measurement current [A]. */
  float_t fltNN;                              /* Nominal speed [rpm]. */
  float_t fltLd;                              /* D-axis inductance [H]. */
  float_t fltLq;                              /* Q-axis inductance [H]. */
  float_t fltRs;                              /* Stator resistance [Ohm]. */
  float_t fltUdt;                             /* Dead time voltage drop of the power stage [V]. */
  bool_t  bEstimFriction;                     /* Enable "Advanced" mode (friction estimation) [-]. */
  /* Optional (advanced) parameters */
  float_t fltAlignTime;                       /* Time needed for rotor alignment [s]. */
  float_t fltIReqOl;                          /* Required d-axis current for open loop startup [A]. */
  float_t fltNStepDc;                         /* Required DC speed step for ramp [delta rpm/s]. */
  float_t fltNStepAc;                         /* Required AC speed step for ramp [delta rpm/s]. */
  float_t fltNDcReq1;                         /* Required speed for measurement point 1 [rpm]. */
  float_t fltNDcReq2;                         /* Required speed for measurement point 2 ("Advanced mode") [rpm]. */
  float_t fltNAcReq;                          /* Required amplitude of the injected AC speed [rpm]. */
  float_t fltFInjMax;                         /* Maximum possible injection frequency [Hz] */                          
  float_t fltFInjMin;                         /* Minimum possible injection frequency [Hz] */
  float_t fltFInjStep;                        /* Injection frequency calculation step [Hz] */
  float_t fltSSTimeoutTime;                   /* Time to reach steady state before timeout [s]. */
  float_t fltSSTimeMin;                       /* Minimal time needed for stable behavior [s]. */
  float_t fltSSTimeTrans;                     /* Transition time during selection of injected frequency [s]. */
  float_t fltDcPiPropGain;                    /* Proportional gain of the speed loops DC controller [-]. */
  float_t fltDcPiIntegGain;                   /* Integral gain of the speed loops DC controller [-]. */
  float_t fltAcPiPropGain;                    /* Proportional gain of the speed loops AC controller [-]. */
  float_t fltAcPiIntegGain;                   /* Integral gain of the speed loops AC controller [-]. */
} MCAA_ESTIMBJ_INIT_T_FLT;

/* BJ Estimator configuration structure. */
typedef struct
{
  MCAA_ESTIMBJ_INNERST_T_FLT pInnerState;       /* Internal state of the BJ estimator structure. No user-editable values. */
  float_t fltKe;                                /* Estimated BEMF constant (electrical constant) [V·s/rad] */
  float_t fltKt;                                /* Estimated torque constant [N·m/A]. */
  float_t fltJ;                                 /* Estimated moment of inertia [kg·m^2]. */
  float_t fltA;                                 /* Estimated static friction torque [N·m]. In "basic" mode returns zero. */
  float_t fltB;                                 /* Estimated friction coefficient [N·m·s/rad]. In "basic" mode returns zero. */
} MCAA_ESTIMBJ_T_FLT; 

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
MCAA_ESTIMBJ_RET_T_FLT MCAA_EstimBJ_FLT_FC(float_t fltUDcBus,
                                           const GMCLIB_2COOR_ALBE_T_FLT *const pIAlBeFbck,
                                           MCAA_ESTIMBJ_T_FLT *const psCtrl,
                                           GMCLIB_2COOR_ALBE_T_FLT *const pUAlBeReq);

MCAA_ESTIMBJ_INIT_RET_T_FLT MCAA_EstimBJInit_FLT_FC(MCAA_ESTIMBJ_INIT_T_FLT *psParam,
                                                    MCAA_ESTIMBJ_T_FLT *const psCtrl);

/*******************************************************************************
* Inline functions
*******************************************************************************/

#ifdef __cplusplus
}
#endif

#endif	/* MCAA_ESTIMBJ_FLT_H_ */