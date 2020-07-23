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
* @brief  Conversion functions
* 
*******************************************************************************/
#ifndef _MLIB_CONV_FLT_ASMI_H_
#define _MLIB_CONV_FLT_ASMI_H_

#if defined(__cplusplus)
extern "C" {
#endif

/*******************************************************************************
* Includes
*******************************************************************************/
#include "mlib_types.h"  

/*******************************************************************************
* Macros 
*******************************************************************************/
#define MLIB_Conv_F16f_Asmi(fltVal) MLIB_Conv_F16f_FAsmi(fltVal)
#define MLIB_Conv_F32f_Asmi(fltVal) MLIB_Conv_F32f_FAsmi(fltVal)
#define MLIB_Conv_A32f_Asmi(fltVal) MLIB_Conv_A32f_FAsmi(fltVal)  
#define MLIB_Conv_FLTs_Asmi(f16Val) MLIB_Conv_FLTs_FAsmi(f16Val)
#define MLIB_Conv_FLTl_Asmi(f32Val) MLIB_Conv_FLTl_FAsmi(f32Val)
#define MLIB_Conv_FLTa_Asmi(a32Val) MLIB_Conv_FLTa_FAsmi(a32Val)

/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in   Input value in 16-bit fractional format to be converted.
*
* @return   This function returns
*             - Converted input value in single precision floating point format.
*       
* @remarks  This inline function returns converted input value. 
*           The input value is considered as 16-bit fractional data type and 
*           output value is considered as single precision floating point data type.
*           The input value represents the values in unity model. The output
*           saturation is not implemented in this function.
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline float_t MLIB_Conv_FLTs_FAsmi(register frac16_t f16Val)
{
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/32768.0f;
        __asm volatile{ vmov fltOut, f16Val                 /* fltOut = f16Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv }; /* fltOut = fltOut / 32768.0f */
    #else
        __asm volatile( "vmov %0, %1 \n"                    /* fltOut = f16Val */
                        "vcvt.f32.s16 %0, %0, #15 \n"       /* Converts between floating point and fixed point */
                        : "=t"(fltOut): "l"(f16Val));
    #endif
    return (fltOut);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in  Input value in 32-bit fractional format to be converted.
*
* @return   This function returns
*             - Converted input value in single precision floating point format.
*       
* @remarks  This inline function returns converted input value. 
*           The input value is considered as 32-bit fractional data type and 
*           output value is considered as single precision floating point data type.
*           The input value represents the values in unity model. The output
*           saturation is not implemented in this function.
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline float_t MLIB_Conv_FLTl_FAsmi(register frac32_t f32Val)
{
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/2147483648.0f;
        __asm volatile{ vmov fltOut, f32Val                 /* fltOut = f32Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv }; /* fltOut = fltOut / 2147483648.0f */
    #else
        __asm volatile( "vmov  %0, %1 \n"                   /* fltOut = f32Val */
                        "vcvt.f32.s32 %0, %0, #31 \n"       /* Converts between floating point and fixed point */
                        : "=t"(fltOut): "l"(f32Val));
    #endif
    return (fltOut);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in  Input value in 32-bit accumulator format to be converted.
*
* @return   This function returns
*             - Converted input value in single precision floating point format.
*       
* @remarks  This inline function returns converted input value. 
*           The input value is considered as 32-bit accumulator data type and
*           output value is considered as single precision floating point data type.
*           The input value represents the values in unity model. The output
*           saturation is not implemented in this function.
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline float_t MLIB_Conv_FLTa_FAsmi(register acc32_t a32Val)
{
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/32768.0f;
        __asm volatile{ vmov fltOut, a32Val                 /* fltOut = a32Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv }; /* fltOut = fltOut / 32768.0f */
    #else
        __asm volatile( "vmov %0, %1 \n"                    /* fltOut = a32Val */
                        "vcvt.f32.s32 %0, %0, #15 \n"       /* Converts between floating point and fixed point */
                        : "=t"(fltOut): "l"(a32Val));
    #endif
    return (fltOut);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in  fltVal  Input value in single precision floating point format to be converted.
*
* @return   This function returns
*             - Converted input value in 16-bit fractional format.
*       
* @remarks  This inline function returns converted input value. The input value is 
*           considered as single precision floating  point data type and output value 
*           is considered as 16-bit fractional data type. The output value  represents
*           the values in unity model. The output saturation is implemented in this function,
*           thus in case the input value is outside the [-1,1) interval, the output
*           value is limited to the boundary value.
*               
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac16_t MLIB_Conv_F16f_FAsmi(register float_t fltVal)
{
    register frac16_t f16Out;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 32768.0f;
        __asm volatile{ vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 32768.0f */
                        vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                        vmov f16Out, fltVal                 /* f16Out = fltVal */
                        ssat f16Out, #16, f16Out};          /* Saturation */
    #else
        __asm volatile( "vcvt.s16.f32 %1, %1, #15 \n"       /* Converts between floating point and fixed point */
                        "vmov %0, %1 \n"                    /* f16Out = fltVal */
                        : "=l"(f16Out): "t"(fltVal));
    #endif
    return (f16Out);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in  fltVal  Input value in single precision floating point format to be converted.
*
* @return   This function returns
*             - Converted input value in 32-bit fractional format.
*       
* @remarks  This inline function returns converted input value. The input value is 
*           considered as single precision floating  point data type and output value 
*           is considered as 32-bit fractional data type. The output value  represents
*           the values in unity model. The output saturation is implemented in this function,
*           thus in case the input value is outside the [-1,1) interval, the output
*           value is limited to the boundary value.
*               
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline frac32_t MLIB_Conv_F32f_FAsmi(register float_t fltVal)
{
    register frac32_t f32Out=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 2147483648.0f;
        __asm volatile{ vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 2147483648.0f */
                        vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                        vmov f32Out, fltVal };              /* f32Out = fltVal */
    #else
        __asm volatile( "vcvt.s32.f32 %1, %1, #31 \n"       /* Converts between floating point and fixed point */
                        "vmov %0, %1 \n"                    /* f32Out = fltVal */
                        : "=l"(f32Out): "t"(fltVal));
    #endif
    return (f32Out);
}
RTCESL_INLINE_OPTIM_RESTORE 
         
/***************************************************************************//*!
*
* @brief    This function converts the input value to different representation without scale.
*
* @param    in  fltVal  Input value in single precision floating point format to be converted.
*
* @return   This function returns
*             - Converted input value in 32-bit accumulator format [-65536.0, 65536.0).
*       
* @remarks  This inline function returns converted input value. The input value is 
*           considered as single precision floating  point data type and output value 
*           is considered as 32-bit accumulator data type. The output value  represents
*           the values in unity model. The output saturation is implemented in this function,
*           thus in case the input value is outside the [-65536.0, 65536.0) interval, the output
*           value is limited to the boundary value.
*               
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
RAM_FUNC_LIB 
static inline acc32_t MLIB_Conv_A32f_FAsmi(register float_t fltVal)
{
    register acc32_t a32Out=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 32768.0f;
        __asm volatile{ vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 32768.0f */
                        vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                        vmov a32Out, fltVal };              /* a32Out = fltVal */
    #else
        __asm volatile( "vcvt.s32.f32 %1, %1, #15 \n"       /* Converts between floating point and fixed point */
                        "vmov %0, %1 \n"                    /* a32Out = fltVal */
                        : "=l"(a32Out): "t"(fltVal));
    #endif
    return (a32Out);
}          
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif /* _MLIB_CONV_FLT_ASMI_H_ */

