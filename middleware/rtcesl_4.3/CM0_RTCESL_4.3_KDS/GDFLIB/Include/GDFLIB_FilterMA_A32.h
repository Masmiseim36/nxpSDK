/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
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
 */

/*******************************************************************************
*
* @brief  Moving average filter 
* 
*******************************************************************************/
#ifndef _GDFLIB_FILTERMA_A32_H_
#define _GDFLIB_FILTERMA_A32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gdflib_types.h"
#include "mlib.h"

/*******************************************************************************
* Macros 
*******************************************************************************/
#define GDFLIB_FilterMAInit_F16_Ci(f16InitVal, psParam)                       \
        GDFLIB_FilterMAInit_F16_FCi(f16InitVal, psParam)                      

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct{
    acc32_t  a32Acc;   /* Filter accumulator. */
    uint16_t u16Sh;    /* Number of samples for averaging, filter sample window [0,15].*/
}GDFLIB_FILTER_MA_T_A32;


/***************************************************************************//*!
* @brief  The function initializes the actual values of FilterMA_F16.
*
* @param  in  frac16_t f16InitVal  - Initial value
*         ptr  GDFLIB_FILTER_MA_T_A32 *psParam - pointer to filter structure 
*
*******************************************************************************/
static inline void GDFLIB_FilterMAInit_F16_FCi(frac16_t f16InitVal,
                                               GDFLIB_FILTER_MA_T_A32 *psParam)
{
    psParam->a32Acc = (acc32_t)MLIB_ShLSat_F32(f16InitVal, psParam->u16Sh) - (acc32_t)f16InitVal; 
}
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_FILTERMA_A32_H_ */
