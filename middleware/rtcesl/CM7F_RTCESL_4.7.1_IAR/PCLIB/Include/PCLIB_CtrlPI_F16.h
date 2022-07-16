/*******************************************************************************
*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2021 NXP
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
****************************************************************************/
#ifndef _PCLIB_CTRLPI_F16_H_
#define _PCLIB_CTRLPI_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib.h"
#include "pclib_types.h"

/*******************************************************************************
* Macros 
*******************************************************************************/  
#define PCLIB_CtrlPIInit_F16_C(psParam)       PCLIB_CtrlPIInit_F16_FC(psParam)
#define PCLIB_CtrlPIInit_F16_CRam(psParam)       PCLIB_CtrlPIInit_F16_FCRam(psParam)
#define PCLIB_CtrlPI_F16_C(f16InErr, psParam) PCLIB_CtrlPI_F16_FC(f16InErr, psParam) 
#define PCLIB_CtrlPI_F16_CRam(f16InErr, psParam) PCLIB_CtrlPI_F16_FCRam(f16InErr, psParam) 

/*******************************************************************************
* Types
*******************************************************************************/
typedef struct
{
    frac16_t f16Kp;                     /* Proportional Gain. */  
    frac16_t f16Ki;                     /* Integral Gain. */
    frac16_t f16IntegralUpperLimit;     /* Integral Upper limit */
    frac16_t f16IntegralLowerLimit;     /* Integral Lower limit */
    frac16_t f16PreviousIntegralOutput; /* Previous Integral output (n-1) */ 
    frac16_t f16UpperLimit;             /* Control Loop Output Upper Limit */
    frac16_t f16LowerLimit;             /* Control Loop Output Lower Limit */
} PCLIB_CTRL_PI_T_F16;

/****************************************************************************
* Exported function prototypes
****************************************************************************/
RAM_FUNC_LIB
extern void PCLIB_CtrlPIInit_F16_FCRam(PCLIB_CTRL_PI_T_F16 *psParam);

extern void PCLIB_CtrlPIInit_F16_FC(PCLIB_CTRL_PI_T_F16 *psParam);

RAM_FUNC_LIB
extern frac16_t PCLIB_CtrlPI_F16_FCRam(frac16_t f16InErr, PCLIB_CTRL_PI_T_F16 *psParam);

extern frac16_t PCLIB_CtrlPI_F16_FC(frac16_t f16InErr, PCLIB_CTRL_PI_T_F16 *psParam);

#if defined(__cplusplus)
}
#endif

#endif /* _PCLIB_CTRLPI_F16_H_ */
