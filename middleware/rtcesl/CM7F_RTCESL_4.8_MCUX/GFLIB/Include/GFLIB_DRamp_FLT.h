/*******************************************************************************
*
* Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
* Copyright 2016-2021, 2024 NXP
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
*
****************************************************************************//*!
*
* @brief  Dynamic ramp
* 
*******************************************************************************/
#ifndef _GFLIB_DRAMP_FLT_H_
#define _GFLIB_DRAMP_FLT_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_FP.h"
#include "gflib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_DRampInit_FLT_Ci(fltInitVal, psParam)                            \
        GFLIB_DRampInit_FLT_FCi(fltInitVal, psParam)
#define GFLIB_DRamp_FLT_C(fltTarget, fltInstant, pbStopFlag, psParam)          \
        GFLIB_DRamp_FLT_FC(fltTarget, fltInstant, pbStopFlag, psParam)
#define GFLIB_DRamp_FLT_CRam(fltTarget, fltInstant, pbStopFlag, psParam)       \
        GFLIB_DRamp_FLT_FCRam(fltTarget, fltInstant, pbStopFlag, psParam)

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    float_t fltRampUp;      /* Ramp-up increment  */
    float_t fltRampDown;    /* Ramp-down increment */
    float_t fltRampUpSat;   /* Ramp-up increment used in case of saturation */
    float_t fltRampDownSat; /* Ramp-down increment used in case of saturation */
    float_t fltState;       /* Previous ramp value */
    bool_t  bReachFlag;     /* Flag is set to 1 if the desired value is achieved */
} GFLIB_DRAMP_T_FLT;

/***************************************************************************//*!
* @brief    The function initializes the actual value of DRamp_FLT.
*
* @param    ptr  GFLIB_DRAMP_T_FLT *psParam
*                  - fltRampUp: Ramp-up increment
*                  - fltRampDown: Ramp-down increment
*                  - fltRampUpSat: Ramp-up increment used in case of saturation
*                  - fltRampDownSat: Ramp-down increment used in case of saturation
*                  - fltState: Previous ramp value
*                  - bReachFlag: Flag is set to 1 if the desired value is achieved
* @param    in   float_t fltInitVal - Initial value
* @remarks  The initialization value is stored into the psParam->fltState and the
*           bReachFlag flag is cleared.
*******************************************************************************/
RAM_FUNC_LIB 
RTCESL_INLINE static inline void GFLIB_DRampInit_FLT_FCi(float_t fltInitVal, GFLIB_DRAMP_T_FLT *psParam)
{
    psParam->fltState = fltInitVal;
    psParam->bReachFlag = (bool_t)0;
}
 
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_DRamp_FLT_FC(float_t fltTarget, float_t fltInstant, 
                                  const bool_t *pbStopFlag, GFLIB_DRAMP_T_FLT *psParam);

RAM_FUNC_LIB 
extern float_t GFLIB_DRamp_FLT_FCRam(float_t fltTarget, float_t fltInstant, 
                                     const bool_t *pbStopFlag, GFLIB_DRAMP_T_FLT *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_DRAMP_FLT_H_ */
