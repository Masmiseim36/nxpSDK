/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * SPDX-License-Identifier: BSD-3-Clause
* 
*
****************************************************************************//*!
*
* @brief  Look-up table algorithm using linear interpolation
* 
*******************************************************************************/
#ifndef _GFLIB_LUT1D_FLT_H_
#define _GFLIB_LUT1D_FLT_H_

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
#define GFLIB_Lut1D_FLT_C(fltX, pfltTable, psParam)                            \
        GFLIB_Lut1D_FLT_FC(fltX, pfltTable, psParam)  
#define GFLIB_Lut1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)             \
        GFLIB_Lut1DInit_FLT_FCi(fltMin, fltMax, pfltTable, psParam)  
    
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    float_t fltMin;         /* Assign the first value of X-values, set by user using Init function */ 
    float_t fltMax;         /* Assign the last  value of X-values, set by user using Init function */  
    float_t fltIntInv;      /* Table inverse interval in float, calculated by GFLIB_Lut1DInit_FLT*/ 
    uint16_t u16TableSize;  /* Real table size (no shift) from interval: <2; 65535> */
}   GFLIB_LUT1D_T_FLT;

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern float_t GFLIB_Lut1D_FLT_FC(float_t fltX, 
                                  const float_t *pfltTable, 
                                  const GFLIB_LUT1D_T_FLT *psParam);

/***************************************************************************//*!
* @brief    The function initializes internal variables for constants of LUT
*           interpolation function
*
* @params   in  float_t fltMin - the first value of X-values
*           in  float_t fltMax - the last  value of X-values
*           in  uint16_t ui16TabSize - Size of LUT table
*           ptr GFLIB_LUT1D_T_FLT *psParam - Pointer to parameter structure
*
* @return   N/A
* 
*******************************************************************************/
static inline void GFLIB_Lut1DInit_FLT_FCi(float_t fltMin, float_t fltMax,
                                           uint16_t u16TableSize,
                                           GFLIB_LUT1D_T_FLT *psParam)
{   
    psParam->fltIntInv = MLIB_Div_FLT((float_t)(u16TableSize-1),MLIB_Sub_FLT(fltMax, fltMin));     
    psParam->u16TableSize = u16TableSize;
    psParam->fltMin = fltMin;
    psParam->fltMax = fltMax;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUT1D_FLT_H_ */
