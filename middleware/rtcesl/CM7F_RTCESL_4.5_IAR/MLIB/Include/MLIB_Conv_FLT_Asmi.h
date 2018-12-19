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

