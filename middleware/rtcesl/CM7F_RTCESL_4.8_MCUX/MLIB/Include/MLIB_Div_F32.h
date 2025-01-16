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
RTCESL_INLINE static inline frac16_t MLIB_Div_F16ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    if (f16Denom == 0)
    {
        return((f32Num >= 0) ? INT16_MAX : INT16_MIN);
    }
    else
    {
        return(frac16_t)MLIB_Sh1R_F32_Ci(f32Num / (frac32_t)f16Denom);
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
RTCESL_INLINE static inline frac16_t MLIB_DivSat_F16ls_FCi(register frac32_t f32Num, register frac16_t f16Denom)
{
    register frac32_t f32Return;

    if (f16Denom == (frac16_t)0)
    {
        return ((f32Num >= (frac32_t)0) ? (frac16_t)(INT16_MAX) : (frac16_t)(INT16_MIN));
    }
    else
    {
        f32Return = MLIB_Sh1R_F32_Ci(f32Num / (frac32_t)f16Denom);
        return(frac16_t)(f32Return > (frac16_t)(INT16_MAX) ? (frac16_t)(INT16_MAX) : f32Return < (frac16_t)(INT16_MIN) ? (frac16_t)(INT16_MIN) : f32Return);
    }
}
 
#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_DIV_F32_H_ */
