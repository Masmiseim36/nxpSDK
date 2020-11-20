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
****************************************************************************//*!
*
* @brief  Main GDFLIB header file for devices without FPU.
* 
*******************************************************************************/
#ifndef _GDFLIB_H_
#define _GDFLIB_H_

#if defined(__cplusplus)
extern "C" {
#endif
  
/*******************************************************************************
* Includes
*******************************************************************************/
#include "GDFLIB_FilterExp_F32.h" 
#include "GDFLIB_FilterIIR1_F32.h"
#include "GDFLIB_FilterIIR1_F32_Asmi.h"
#include "GDFLIB_FilterIIR2_F32.h"
#include "GDFLIB_FilterIIR3_F32.h"
#include "GDFLIB_FilterIIR4_F32.h"
#include "GDFLIB_FilterMA_A32.h"
   
/*******************************************************************************
* Macros 
*******************************************************************************/
#if (defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) /* placed to RAM */
#define GDFLIB_FilterIIR3_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR3_F16_CRam(f16InX, psParam)      
#define GDFLIB_FilterIIR4_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR4_F16_CRam(f16InX, psParam)
#define GDFLIB_FilterIIR2_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR2_F16_AsmRam(f16InX, psParam)          
#else /* placed to ROM */
#define GDFLIB_FilterIIR3_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR3_F16_C(f16InX, psParam)      
#define GDFLIB_FilterIIR4_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR4_F16_C(f16InX, psParam)
#define GDFLIB_FilterIIR2_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR2_F16_Asm(f16InX, psParam)  
#endif /*(defined(RAM_OPTIM_HIGH) || defined(RAM_OPTIM_MEDIUM) || defined(RAM_OPTIM_LOW)) */
          
#define GDFLIB_FilterExpInit_F16(f16InX, psParam)                              \
        GDFLIB_FilterExpInit_F16_Ci(f16InX, psParam)             
#define GDFLIB_FilterExp_F16(f16InX, psParam)                                  \
        GDFLIB_FilterExp_F16_Ci(f16InX, psParam)    
#define GDFLIB_FilterIIR1Init_F16(psParam)                                     \
        GDFLIB_FilterIIR1Init_F16_Ci(psParam)                                  
#define GDFLIB_FilterIIR1_F16(f16InX, psParam)                                 \
        GDFLIB_FilterIIR1_F16_Asmi(f16InX, psParam)
#define GDFLIB_FilterIIR2Init_F16(psParam)                                     \
        GDFLIB_FilterIIR2Init_F16_C(psParam)                                  
#define GDFLIB_FilterIIR3Init_F16(psParam)                                     \
        GDFLIB_FilterIIR3Init_F16_C(psParam)                                                             
#define GDFLIB_FilterIIR4Init_F16(psParam)                                     \
        GDFLIB_FilterIIR4Init_F16_C(psParam)                                  
#define GDFLIB_FilterMAInit_F16(f16InitVal, psParam)                           \
        GDFLIB_FilterMAInit_F16_Ci(f16InitVal, psParam)                        
#define GDFLIB_FilterMA_F16(f16InX, psParam)                                   \
        GDFLIB_FilterMA_F16_Ci(f16InX, psParam)
 
#if defined(__cplusplus)
}
#endif

#endif /* _GDFLIB_H_ */
