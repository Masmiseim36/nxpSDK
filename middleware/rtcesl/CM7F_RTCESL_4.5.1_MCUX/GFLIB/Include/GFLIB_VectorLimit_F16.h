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
* @brief  Vector limit functions
* 
*******************************************************************************/
#ifndef _GFLIB_VECTORLIMIT_F16_H_
#define _GFLIB_VECTORLIMIT_F16_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "gflib.h"  
 
/*******************************************************************************
* Macros 
*******************************************************************************/  
#define GFLIB_VectorLimit_F16_C(psVectorIn, f16Lim, psVectorOut)               \
        GFLIB_VectorLimit_F16_FC(psVectorIn, f16Lim, psVectorOut)
#define GFLIB_VectorLimit_F16_CRam(psVectorIn, f16Lim, psVectorOut)            \
        GFLIB_VectorLimit_F16_FCRam(psVectorIn, f16Lim, psVectorOut)		
#define GFLIB_VectorLimit1_F16_C(psVectorIn, f16Lim, psVectorOut)              \
        GFLIB_VectorLimit1_F16_FC(psVectorIn, f16Lim, psVectorOut)
#define GFLIB_VectorLimit1_F16_CRam(psVectorIn, f16Lim, psVectorOut)           \
        GFLIB_VectorLimit1_F16_FCRam(psVectorIn, f16Lim, psVectorOut)		
        
/*******************************************************************************
* Types
*******************************************************************************/  
typedef struct
{
    frac16_t f16A;   /* First argument */
    frac16_t f16B;   /* Second argument */
} GFLIB_VECTORLIMIT_T_F16;
  
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
extern bool_t GFLIB_VectorLimit_F16_FC(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                       frac16_t f16Lim, 
                                       GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
RAM_FUNC_LIB 
extern bool_t GFLIB_VectorLimit_F16_FCRam(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                          frac16_t f16Lim, 
                                          GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
									   
extern bool_t GFLIB_VectorLimit1_F16_FC(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                       frac16_t f16Lim, 
                                       GFLIB_VECTORLIMIT_T_F16 *psVectorOut);									   									   
RAM_FUNC_LIB 
extern bool_t GFLIB_VectorLimit1_F16_FCRam(const GFLIB_VECTORLIMIT_T_F16 *psVectorIn, 
                                          frac16_t f16Lim, 
                                          GFLIB_VECTORLIMIT_T_F16 *psVectorOut);
#if defined(__cplusplus)
}
#endif

#endif /* _GFLIB_VECTORLIMIT_F16_H_ */
