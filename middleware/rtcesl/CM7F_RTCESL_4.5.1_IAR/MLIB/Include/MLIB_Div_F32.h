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
* @brief  Division
* 
*******************************************************************************/
#ifndef _MLIB_DIV_F32_H_
#define _MLIB_DIV_F32_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"
#include "MLIB_Clb_F32.h"
#include "MLIB_BiShift_F32.h"  

/*******************************************************************************
* Macros 
*******************************************************************************/
#define MLIB_Div_F32ls_C(f32Num, f16Denom)      MLIB_Div_F32ls_FC(f32Num, f16Denom)
#define MLIB_Div_F32ls_CRam(f32Num, f16Denom)   MLIB_Div_F32ls_FCRam(f32Num, f16Denom)
   
#define MLIB_Div_F16ls_Ci(f32Num, f16Denom)     MLIB_Div_F16ls_FCi(f32Num, f16Denom)

#define MLIB_DivSat_F32ls_C(f32Num, f16Denom)      MLIB_DivSat_F32ls_FC(f32Num, f16Denom) 
#define MLIB_DivSat_F32ls_CRam(f32Num, f16Denom)   MLIB_DivSat_F32ls_FCRam(f32Num, f16Denom) 
   
#define MLIB_DivSat_F16ls_Ci(f32Num, f16Denom)  MLIB_DivSat_F16ls_FCi(f32Num, f16Denom)
 
#define MLIB_Div_F16ll_Asm(f32Num, f32Denom)    MLIB_Div_F16ll_FAsm(f32Num, f32Denom)
#define MLIB_Div_F16ll_AsmRam(f32Num, f32Denom)    MLIB_Div_F16ll_FAsmRam(f32Num, f32Denom)
  
#define MLIB_Div_F32_Asm(f32Num, f32Denom)      MLIB_Div_F32_FAsm(f32Num, f32Denom)
#define MLIB_Div_F32_AsmRam(f32Num, f32Denom)   MLIB_Div_F32_FAsmRam(f32Num, f32Denom)
  
#define MLIB_DivSat_F16ll_Asm(f32Num, f32Denom) MLIB_DivSat_F16ll_FAsm(f32Num, f32Denom) 
#define MLIB_DivSat_F16ll_AsmRam(f32Num, f32Denom) MLIB_DivSat_F16ll_FAsmRam(f32Num, f32Denom) 

#define MLIB_DivSat_F32_Asm(f32Num, f32Denom)   MLIB_DivSat_F32_FAsm(f32Num, f32Denom)
#define MLIB_DivSat_F32_AsmRam(f32Num, f32Denom)   MLIB_DivSat_F32_FAsmRam(f32Num, f32Denom)
    
/*******************************************************************************
* Exported function prototypes
*******************************************************************************/
RAM_FUNC_LIB 
extern frac32_t MLIB_Div_F32ls_FCRam(frac32_t f32Num, frac16_t f16Denom);  
  
extern frac32_t MLIB_Div_F32ls_FC(frac32_t f32Num, frac16_t f16Denom);   
 
RAM_FUNC_LIB 
extern frac32_t MLIB_DivSat_F32ls_FCRam(frac32_t f32Num, frac16_t f16Denom);  
 
extern frac32_t MLIB_DivSat_F32ls_FC(frac32_t f32Num, frac16_t f16Denom);   

RAM_FUNC_LIB 
extern frac32_t MLIB_Div_F32_FAsmRam(frac32_t f32Num, frac32_t f32Denom);

extern frac32_t MLIB_Div_F32_FAsm(frac32_t f32Num, frac32_t f32Denom);

RAM_FUNC_LIB 
extern frac16_t MLIB_Div_F16ll_FAsmRam(frac32_t f32Num, frac32_t f32Denom);

extern frac16_t MLIB_Div_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom);

RAM_FUNC_LIB 
extern frac16_t MLIB_DivSat_F16ll_FAsmRam(frac32_t f32Num, frac32_t f32Denom);

extern frac16_t MLIB_DivSat_F16ll_FAsm(frac32_t f32Num, frac32_t f32Denom);

RAM_FUNC_LIB 
extern frac32_t MLIB_DivSat_F32_FAsmRam(frac32_t f32Num, frac32_t f32Denom);

extern frac32_t MLIB_DivSat_F32_FAsm(frac32_t f32Num, frac32_t f32Denom);

/***************************************************************************//*!
* @brief  32-bit numerator, 16-bit denominator inputs 16-output 4-quadrant
*         division function
*
* @param  in  frac32_t f32Num  - Numerator in <-1;1) in frac32_t
*             frac16_t f16Denom- Denominator in <-1;1) in frac16_t
*
* @return This function returns- frac16_t value <-1;1)
*       
* @remarks  This function divides two fractional inputs:
*           result = f32Num / f16Denom.
*           The function does not saturate the output.
*           If the denominator is 0, the output is 0x7FFF or 0x8000
*           depending on the numerator's sign. For the both are 0, the output is
*           0x7FFF.
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_Div_F16ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return((f32Num >= 0) ? INT16_MAX : INT16_MIN);
    }
    else
    {
        return(frac16_t)((frac32_t)(f32Num / (frac32_t)f16Denom) >> 1);
    }
}
 
/***************************************************************************//*!
* @brief  32-bit numerator, 16-bit denominator inputs 16-output 4-quadrant
*         division function
*
* @param  in  frac32_t f32Num  - Numerator in <-1;1) in frac32_t
*             frac16_t f16Denom- Denominator in <-1;1) in frac16_t                      
*
* @return This function returns - frac16_t value <-1;1)
*       
* @remarks  This function divides two fractional inputs:
*           result = f32Num / f16Denom.
*           The function saturates the output if |f32Num| > |f16Denom|
*           to 0x7FFF or 0x8000 depending on the signs. 
*
*******************************************************************************/
RAM_FUNC_LIB 
static inline frac16_t MLIB_DivSat_F16ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    register frac32_t f32Return;

    if (f16Denom == (frac16_t)0)
    {
        return ((f32Num >= (frac32_t)0) ? (frac16_t)(INT16_MAX) : (frac16_t)(INT16_MIN));
    }
    else
    {
        f32Return = (f32Num / (frac32_t)f16Denom) >> 1;
        return(frac16_t)(f32Return > (frac16_t)(INT16_MAX) ? (frac16_t)(INT16_MAX) : f32Return < (frac16_t)(INT16_MIN) ? (frac16_t)(INT16_MIN) : f32Return);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_F32_H_ */
