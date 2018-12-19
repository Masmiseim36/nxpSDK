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
#ifndef _MLIB_CONVSC_FLT_ASMI_H_
#define _MLIB_CONVSC_FLT_ASMI_H_

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
#define MLIB_ConvSc_FLTsf_Asmi(f16Val, fltSc) MLIB_ConvSc_FLTsf_FAsmi(f16Val, fltSc)
#define MLIB_ConvSc_FLTlf_Asmi(f32Val, fltSc) MLIB_ConvSc_FLTlf_FAsmi(f32Val, fltSc)     
#define MLIB_ConvSc_F16ff_Asmi(fltVal, fltSc) MLIB_ConvSc_F16ff_FAsmi(fltVal, fltSc)
#define MLIB_ConvSc_F32ff_Asmi(fltVal, fltSc) MLIB_ConvSc_F32ff_FAsmi(fltVal, fltSc) 
#define MLIB_ConvSc_A32ff_Asmi(fltVal, fltSc) MLIB_ConvSc_A32ff_FAsmi(fltVal, fltSc)
#define MLIB_ConvSc_FLTaf_Asmi(a32Val, fltSc) MLIB_ConvSc_FLTaf_FAsmi(a32Val, fltSc)
    
/***************************************************************************//*!
* 16-bit fractional to float convert function
*    
* @param    in  float_t f16Val
*                 - Argument in frac16_t <-1;1)
*               float_t fltSc
*                 - Argument in float_t
*
* @return   This function returns
*                 - float_t value
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as 16-bit fractional and float data types and output value is
*           considered as single precision floating point data type. The input value
*           represents the values in unity model. The second value represents the scale
*           factor and is considered as single precision float data type and determine the
*           scale conversion
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline float_t MLIB_ConvSc_FLTsf_FAsmi(register frac16_t f16Val,
                                              register float_t  fltSc)
{
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/32768.0f;
        __asm volatile{ vmov fltOut, f16Val                 /* fltOut = f16Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv    /* fltOut = fltOut / 32768.0f */
                        vmul.f32 fltOut, fltOut, fltSc };   /* fltOut = fltOut * fltSc */
    #else
        __asm volatile( "vmov %0, %1 \n"                    /* fltOut = f16Val */
                        "vcvt.f32.s16 %0, %0, #15 \n"       /* Converts between floating point and fixed point */
                        "vmul.f32 %0, %0, %2 \n"            /* fltOut = fltOut * fltSc */
                        : "=&t"(fltOut): "l"(f16Val), "t"(fltSc));
    #endif
    return (fltOut);
}  
RTCESL_INLINE_OPTIM_RESTORE 
                                                             
/***************************************************************************//*!
* 32-bit fractional to float convert function
*    
* @param    in  float_t f32Val
*                 - Argument in frac32_t <-1;1)
*               float_t fltSc
*                 - Argument in float_t
*
* @return   This function returns
*                 - float_t value
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as 32-bit fractional and float data types and output value is
*           considered as single precision floating point data type. The input value
*           represents the values in unity model. The second value represents the scale
*           factor and is considered as single precision float data type and determine the
*           scale conversion
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline float_t MLIB_ConvSc_FLTlf_FAsmi(register frac32_t f32Val,
                                              register float_t fltSc)
{        
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/2147483648.0f;
        __asm volatile{ vmov fltOut, f32Val                 /* fltOut = f32Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv    /* fltOut = fltOut / 2147483648.0f */
                        vmul.f32 fltOut, fltOut, fltSc };   /* fltOut = fltOut * fltSc */
    #else
        __asm volatile( "vmov %0, %1 \n"                    /* fltOut = f32Val */
                        "vcvt.f32.s32 %0, %0, #31 \n"       /* Converts between floating point and fixed point */
                        "vmul.f32 %0, %0, %2 \n"            /* fltOut = fltOut * fltSc */
                        : "=&t"(fltOut): "l"(f32Val), "t"(fltSc));
    #endif
    return (fltOut);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
* 32-bit accumulator to float convert function
*    
* @param    in  float_t f32Acc
*                 - Argument in acc32_t [-65536.0, 65536.0)
*               float_t fltSc
*                 - Argument in float_t
*
* @return   This function returns
*                 - float_t value
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as 32-bit accumulator and float data types and output value is
*           considered as single precision floating point data type. The input value
*           represents the values in unity model. The second value represents the scale
*           factor and is considered as single precision float data type and determine the
*           scale conversion.
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline float_t MLIB_ConvSc_FLTaf_FAsmi(register acc32_t a32Val,
                                              register float_t fltSc)
{        
    register float_t fltOut=0.0f;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
    register float_t fltConv = 1.0f/32768.0f;
        __asm volatile{ vmov fltOut, a32Val                 /* fltOut = a32Val */
                        vcvt.f32.s32 fltOut, fltOut         /* Converts from integer to float */
                        vmul.f32 fltOut, fltOut, fltConv    /* fltOut = fltOut / 32768.0f */
                        vmul.f32 fltOut, fltOut, fltSc };   /* fltOut = fltOut * fltSc */
    #else
        __asm volatile( "vmov %0, %1 \n"                    /* fltOut = a32Val */
                        "vcvt.f32.s32 %0, %0, #15 \n"       /* Converts between floating point and fixed point */
                        "vmul.f32 %0, %0, %2 \n"            /* fltOut = fltOut * fltSc */
                        : "=&t"(fltOut): "l"(a32Val), "t"(fltSc));
    #endif
    return (fltOut);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
* float to 16-bit fractional  convert function
*    
* @param    in  fltVal  Input value in single precision floating point format
*                       to be converted.
*               fltSc   Scale factor in single precision floating point format.
*                
* @return   This function returns
*               - 16-bit fractional value
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as single precision floating point data type and output value
*           is considered as 16-bit fractional data type. The second value represents the
*           scale factor and is considered as single precision floating point data type.
*           The output value represents the values in unity model. The output saturation
*           is implemented in this function, thus in case the input value is outside
*           the [-1,1) interval, the output value is limited to the boundary value.
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac16_t MLIB_ConvSc_F16ff_FAsmi(register float_t fltVal,
                                               register float_t fltSc)
{
  register frac16_t f16Out=0;

  #if defined(__CC_ARM)                                   /* For ARM Compiler */
      register float_t fltConv = 32768.0f;
      __asm volatile{ vdiv.f32 fltVal, fltVal, fltSc      /* fltVal = fltVal / fltSc */
                      vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 32768.0f */
                      vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                      vmov f16Out, fltVal                 /* f16Out = fltVal */
                      ssat f16Out, #16, f16Out};          /* f16Out >> 16 */
  #else
      __asm volatile( "vdiv.f32 %1, %1, %2 \n"            /* fltVal = fltVal / fltSc */
                      "vcvt.s16.f32 %1, %1, #15 \n"       /* Converts between floating point and fixed point */
                      "vmov %0, %1 \n"                    /* f16Out = fltVal */
                      : "=l"(f16Out), "+t"(fltVal): "t"(fltSc));
  #endif
  return (f16Out);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
* float to 32-bit fractional  convert function
*    
* @param    in  fltVal  Input value in single precision floating point format
*                       to be converted.
*               fltSc   Scale factor in single precision floating point format.
*                
* @return   This function returns
*               - 32-bit fractional value
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as single precision floating point data type and output value
*           is considered as 32-bit fractional data type. The second value represents the
*           scale factor and is considered as single precision floating point data type.
*           The output value represents the values in unity model. The output saturation
*           is implemented in this function, thus in case the input value is outside
*           the [-1,1) interval, the output value is limited to the boundary value.
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline frac32_t MLIB_ConvSc_F32ff_FAsmi(register float_t fltVal,
                                               register float_t fltSc)
{      
    register frac32_t f32Out=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        register float_t fltConv = 2147483648.0f;
        __asm volatile{ vdiv.f32 fltVal, fltVal, fltSc      /* fltVal = fltVal * fltSc */
                        vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 2147483648.0f */
                        vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                        vmov f32Out, fltVal };              /* f32Out = fltVal */
    #else
        __asm volatile( "vdiv.f32 %1, %1, %2 \n"            /* fltVal = fltVal * fltSc */
                        "vcvt.s32.f32 %1, %1, #31 \n"       /* Converts between floating point and fixed point */
                        "vmov %0, %1 \n"                    /* f32Out = fltVal */
                        : "=l"(f32Out), "+t"(fltVal): "t"(fltSc));
    #endif
    return (f32Out);
}
RTCESL_INLINE_OPTIM_RESTORE 

/***************************************************************************//*!
* float to 32-bit accumulator convert function
*    
* @param    in  fltVal  Input value in single precision floating point format
*                       to be converted.
*               fltSc   Scale factor in single precision floating point format.
*                
* @return   This function returns
*             - 32-bit accumulator value [-65536.0, 65536.0)
*       
* @remarks  This inline function returns converted input value. The input value
*           is considered as single precision floating point data type and output value
*           is considered as 32-bit accumulator data type. The second value represents the
*           scale factor and is considered as single precision floating point data type.
*           The output value represents the values in unity model. The output saturation
*           is implemented in this function, thus in case the input value is outside
*           the [-65536.0, 65536.0) interval, the output value is limited to the boundary value.
*           
*******************************************************************************/
RTCESL_INLINE_OPTIM_SAVE
RTCESL_INLINE_OPTIM_SET 
static inline acc32_t MLIB_ConvSc_A32ff_FAsmi(register float_t fltVal,
                                              register float_t fltSc)
{      
    register acc32_t a32Out=0;

    #if defined(__CC_ARM)                                   /* For ARM Compiler */
        register float_t fltConv = 32768.0f;
        __asm volatile{ vdiv.f32 fltVal, fltVal, fltSc      /* fltVal = fltVal * fltSc */
                        vmul.f32 fltVal, fltVal, fltConv    /* fltVal = fltVal * 32768.0f */
                        vcvt.s32.f32 fltVal, fltVal         /* Converts from float to integer */
                        vmov a32Out, fltVal };              /* a32Out = fltVal */
    #else
        __asm volatile( "vdiv.f32 %1, %1, %2 \n"            /* fltVal = fltVal * fltSc */
                        "vcvt.s32.f32 %1, %1, #15 \n"       /* Converts between floating point and fixed point */
                        "vmov %0, %1 \n"                    /* a32Out = fltVal */
                        : "=l"(a32Out), "+t"(fltVal): "t"(fltSc));
    #endif
    return (a32Out);
}
RTCESL_INLINE_OPTIM_RESTORE 

#if defined(__cplusplus)
}
#endif

#endif  /* _MLIB_CONVSC_FLT_ASMI_H_ */
