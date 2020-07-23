/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2019 NXP
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
*
****************************************************************************//*!
*
* @brief  Periodic look-up table algorithm using linear interpolation
* 
*******************************************************************************/
#ifndef _GFLIB_LUTPER1D_FLT_H_
#define _GFLIB_LUTPER1D_FLT_H_

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
#define GFLIB_LutPer1D_FLT_C(fltX, pfltTable, psParam)                         \
        GFLIB_LutPer1D_FLT_FC( fltX, pfltTable, psParam)
#define GFLIB_LutPer1D_FLT_CRam(fltX, pfltTable, psParam)                      \
        GFLIB_LutPer1D_FLT_FCRam( fltX, pfltTable, psParam)		
#define GFLIB_LutPer1DInit_FLT_Ci(fltMin, fltMax, pfltTable, psParam)          \
        GFLIB_LutPer1DInit_FLT_FCi(fltMin, fltMax, pfltTable, psParam)  
            
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    float_t fltMin;         /* Assign the first value of X-values, set by user using Init function */ 
    float_t fltMax;         /* Assign the last  value of X-values, set by user using Init function */  
    float_t fltIntInv;      /* Table inverse interval in float, calculated by GFLIB_Lut1DInit_FLT*/ 
    uint16_t u16TableSize;  /* Real table size (no shift) from interval: <2; 65535> */
} GFLIB_LUTPER1D_T_FLT;  

/*******************************************************************************
* Exported function prototypes
*******************************************************************************/ 
extern float_t GFLIB_LutPer1D_FLT_FC(float_t fltX, 
                                     const float_t *pfltTable, 
                                     const GFLIB_LUTPER1D_T_FLT *psParam);
RAM_FUNC_LIB 
extern float_t GFLIB_LutPer1D_FLT_FCRam(float_t fltX, 
                                        const float_t *pfltTable, 
                                        const GFLIB_LUTPER1D_T_FLT *psParam);

/***************************************************************************//*!
* @brief    The function initializes internal variables for constants of periodic LUT
*           interpolation function
*
* @params   in  float_t fltMin - the first value of X-values
*           in  float_t fltMax - the last  value of X-values
*           in  uint16_t ui16TabSize - Size of periodic LUT table
*           ptr GFLIB_LUTPER1D_T_FLT *psParam - Pointer to parameter structure
*
* @return   N/A
* 
*******************************************************************************/
RAM_FUNC_LIB 
static inline void GFLIB_LutPer1DInit_FLT_FCi(float_t fltMin, float_t fltMax,
                                              uint16_t u16TableSize,
                                              GFLIB_LUTPER1D_T_FLT *psParam)
{   
    psParam->fltIntInv = MLIB_Div_FLT((float_t)(u16TableSize),MLIB_Sub_FLT(fltMax, fltMin));     
    psParam->u16TableSize = u16TableSize;
    psParam->fltMin = fltMin;
    psParam->fltMax = fltMax;
}

#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_LUTPER1D_FLT_H_ */
